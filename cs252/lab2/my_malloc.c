#include <my_malloc.h>

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define ALLOCATION_FAIL (-1)
#define SBRK_FAILURE (1)
#define SBRK_SUCCESS (0)

/* Pointer to the location of the heap prior to any sbrk calls */

void * g_base = NULL;

/* Pointer to the head of the free list */

header  *g_freelist_head = NULL;

/* Mutex to ensure thread safety for the freelist */

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Pointer to the second fencepost in the most recently allocated chunk from
 * the OS. Used for coalescing chunks
 */

header * g_last_fence_post = NULL;

/*
 * Pointer to the next block in the freelist after the block that was last
 * allocated. If the block pointed to is removed by coalescing, this should be
 * updated to point to the next block after the removed block.
 */

header * g_next_allocate = NULL;

/*
 * Direct the compiler to run the init function before running main
 * this allows initialization of required globals
 */

static void init(void) __attribute__((constructor));

/*
 * Calculates the location of the left neighbor given a header.
 */

static inline header * left_neighbor(header * node) {
  return (header *) (((char *) node) - node->left_size - ALLOC_HEADER_SIZE);
} /* left_neighbor() */

/*
 * Calculates the location of the right neighbor given a header.
 */

static inline header * right_neighbor(header * node) {
  return (header *) (((char *) node) + ALLOC_HEADER_SIZE + TRUE_SIZE(node));
} /* right_neighbor() */

/*
 * Insert a block at the beginning of the freelist.
 * The block is located after its left header, h.
 */

static void insert_free_block(header * node) {
  node->prev = NULL;

  if (g_freelist_head != NULL) {
    g_freelist_head->prev = node;
  }

  node->next = g_freelist_head;
  g_freelist_head = node;
} /* insert_free_block() */

/*
 * Delete the argument 'node' from the list of free blocks.
 */

static void delete_freelist_node(header * node) {
  header * new_header = NULL;

  if ((node->prev == NULL) && (node->next != NULL)) {
    // node is the head of the free list

    node->next->prev = NULL;
    new_header = node->next;
    node->next = NULL;
  }
  else if ((node->prev != NULL) && (node->next == NULL)) {
    // node is the tail of the free list

    node->prev->next = NULL;
    new_header = g_freelist_head;
    node->prev = NULL;
  }
  else if ((node->prev != NULL) && (node->next != NULL)) {
    // node is in the middle of the free list

    node->prev->next = node->next;
    node->next->prev = node->prev;
    new_header = g_freelist_head;
    node->prev = NULL;
    node->next = NULL;
  }

  // if new_header is still NULL, 'node' is the only item in the free list

  g_freelist_head = new_header;
} /* delete_freelist_node() */

/*
 * Sets g_next_allocate to node's next pointer. If node is g_freelist_head, set
 * g_next_allocate to NULL. If node's next pointer is NULL, set g_next_allocate
 * to g_freelist_head (we treat the freelist as circular).
 */

static void set_g_next_allocate(header * node) {
  if (node->next == NULL) {
    if (g_freelist_head == node) {
      g_next_allocate = NULL;
    }
    else {
      g_next_allocate = g_freelist_head;
    }
  }
  else {
    // if there's a valid next node, set g_next_allocate equal to that.

    g_next_allocate = node->next;
  }
} /* set_g_next_allocate() */

/*
 * Iterate over all free and allocated blocks. Check for 2 consecutive free
 * blocks, making sure there are no unexpected gaps. If 2 consecutive free
 * blocks occur in sequence, coalesce them. Also, if there are 2 contiguous
 * chunks of heap memory, delete the middle fenceposts and coalesce accordingly.
 */

static void coalesce(header * prev_heap_last_fp, header * curr_heap_last_fp) {
  if (prev_heap_last_fp == NULL) {
    // no need to coalesce

    return;
  }
  // checks if chunks are adjacent

  int fencepost_subtraction = ((char *) curr_heap_last_fp -
                               (char *) prev_heap_last_fp);
  if (fencepost_subtraction % ARENA_SIZE != 0) {
    // if chunks are not adjacent, nothing can be coalesced

    return;
  }

  // header of first of the two sbrk'd chunks of heap mem

  header * prev_heap_last_block = (header *) (((char *) prev_heap_last_fp) -
      prev_heap_last_fp->left_size - ALLOC_HEADER_SIZE);

  // header of second of 2 sbrk'd chunks of heap mem

  header * curr_heap_last_block = (header *) (((char *) curr_heap_last_fp) -
    curr_heap_last_fp->left_size - ALLOC_HEADER_SIZE);

  if ((prev_heap_last_block->size % MIN_ALLOCATION) == (state) ALLOCATED) {
    delete_freelist_node(curr_heap_last_block);
    // Allocated chunk next to newly sbrk'd unallocated chunk

    prev_heap_last_fp->size = (((char *) curr_heap_last_fp) - ((char *)
          prev_heap_last_fp) - ALLOC_HEADER_SIZE) | (state) UNALLOCATED;
    insert_free_block(prev_heap_last_fp);
    curr_heap_last_fp->left_size = TRUE_SIZE(prev_heap_last_fp);
  }

  else if ((prev_heap_last_block->size % MIN_ALLOCATION) ==
           ((state) UNALLOCATED)) {
    delete_freelist_node(curr_heap_last_block);
    // Unallocated chunk next to newly sbrk'd unallocated chunk

    prev_heap_last_block->size = (TRUE_SIZE(prev_heap_last_block) +
      TRUE_SIZE(curr_heap_last_block) + 3 * ALLOC_HEADER_SIZE) | (state)
      UNALLOCATED;
    curr_heap_last_fp->left_size = TRUE_SIZE(prev_heap_last_block);
  }
} /* coalesce() */

/*
 * Coalesces the 2 (or 3) blocks passed into the function based on current
 * allocation status of each block. Coalesces towards the left as much as
 * possible.
 */

static void coalesce_blocks(header * left, header * middle, header * right)
{
  if (right != NULL) {
    // coalesce 3 blocks

    size_t new_size = TRUE_SIZE(left) + TRUE_SIZE(middle) + TRUE_SIZE(right) +
      2 * ALLOC_HEADER_SIZE;

    if (right == g_next_allocate) {
      set_g_next_allocate(right);
    }

    delete_freelist_node(right);

    // set next header's left_size correctly

    ((header *) (((char *) right) + TRUE_SIZE(right) +
      ALLOC_HEADER_SIZE))->left_size = new_size;

    left->size = new_size | (state) UNALLOCATED;
  }
  else if ((left->size % MIN_ALLOCATION) == (state) UNALLOCATED) {
    // left is unallocated and middle is allocated

    size_t new_size = TRUE_SIZE(left) + TRUE_SIZE(middle) + ALLOC_HEADER_SIZE;

    // set next header's left_size correctly

    ((header *) (((char *) middle) + TRUE_SIZE(middle) +
      ALLOC_HEADER_SIZE))->left_size = new_size;
    left->size = new_size | (state) UNALLOCATED;
  }
  else {
    // left is allocated and middle is unallocated

    size_t new_size = TRUE_SIZE(left) + TRUE_SIZE(middle) + ALLOC_HEADER_SIZE;

    // set next header's left_size correctly

    ((header *) (((char *) middle) + TRUE_SIZE(middle) +
      ALLOC_HEADER_SIZE))->left_size = new_size;
    if (middle == g_next_allocate) {
      set_g_next_allocate(middle);
    }
    delete_freelist_node(middle);
    left->size = new_size | (state) UNALLOCATED;
    insert_free_block(left);
  }
} /* coalesce_blocks() */

/*
 * Instantiates fenceposts at the left and right side of a memory chunk.
 * Fenceposts have a size of ALLOC_HEADER_SIZE.
 */

static void set_fenceposts(void * chunk, size_t size) {
  header * left_fence = (header *) chunk;
  header * right_fence = (header *) (((char *) chunk) +
                         (size - ALLOC_HEADER_SIZE));

  left_fence->size = (state) FENCEPOST;
  right_fence->size = (state) FENCEPOST;

  right_fence->left_size = size - 3 * ALLOC_HEADER_SIZE;
  g_last_fence_post = right_fence;
} /* set_fenceposts() */

/*
 * Constructor that runs before main() to initialize the library.
 */

static void init() {
  g_freelist_head = NULL;

  /* Initialize mutex for thread safety */

  pthread_mutex_init(&g_mutex, NULL);

  /* Manually set printf buffer so it won't call malloc */

  setvbuf(stdout, NULL, _IONBF, 0);

  /* Record the starting address of the heap */

  g_base = sbrk(0);
} /* init() */

/*
 * Allocate more heap space based on the size needed. Round up to the nearest
 * multiple of MIN_ALLOCATION and make sure that you have room for headers. If
 * there's an error getting more heap space, return 1. Otherwise, return 0.
 */

static int allocate_more_heap(size_t size) {
  if (size < 2 * sizeof(header *)) {
    size = 2 * sizeof(header *);
  }
  int needed_size = size + 3 * ALLOC_HEADER_SIZE;
  int more_heap_size = (((int) needed_size / (int) ARENA_SIZE + 1) * ((int)
        ARENA_SIZE));

  // used for coalesce()

  header * prev_heap_fencepost = g_last_fence_post;
  void * more_heap = sbrk(more_heap_size);

  if (more_heap == (void *) ALLOCATION_FAIL) {
    // sometimes sbrk() fails

    errno = ENOMEM;
    return SBRK_FAILURE;
  }

  set_fenceposts(more_heap, more_heap_size);
  header * more_heap_header = (header *) (((char *) more_heap) +
      ALLOC_HEADER_SIZE);
  more_heap_header->size = (more_heap_size - 3 * ALLOC_HEADER_SIZE) | ((state)
      UNALLOCATED);
  more_heap_header->left_size = 0;

  insert_free_block(more_heap_header);

  coalesce(prev_heap_fencepost, g_last_fence_post);

  return SBRK_SUCCESS;
} /* allocate_more_heap() */

/*
 * Splits the passed in block based on the passed in size. The returned value
 * will be the block that is now an appropriate size, based on the passed in
 * size. If the block can't be split, the original block is returned, but
 * allocated.
 */

static header * split_block(size_t size, header * block) {
  // delete_freelist_node deletes the node passed in and changes
  // g_freelist_head accordingly

  delete_freelist_node(block);

  if ((TRUE_SIZE(block) - size) >= (MIN_ALLOCATION + ALLOC_HEADER_SIZE)) {
    // Any space after 'size' memory is not needed

    header * remainder = (header *)
                         (((char *) block) + size + ALLOC_HEADER_SIZE);
    remainder->size = (TRUE_SIZE(block) - size - ALLOC_HEADER_SIZE) |
                      ((state) UNALLOCATED);

    //block's left_size will always be allocated when splitting, so
    //don't bother

    remainder->left_size = size;

    // This call sets prev and next

    insert_free_block(remainder);

    header * remainder_right_edge = (header *) (((char *) remainder) +
        remainder->size + ALLOC_HEADER_SIZE);

    remainder_right_edge->left_size = remainder->size;

    //block->size = (TRUE_SIZE(block) - TRUE_SIZE(remainder) -
    //  ALLOC_HEADER_SIZE) | (state) ALLOCATED;

    block->size = size | (state) ALLOCATED;
  }
  else {
    block->size = TRUE_SIZE(block) | (state) ALLOCATED;
  }

  return block;
} /* split_block() */

/*
 * Allocate the first available block able to satisfy the request
 * (starting the search at g_freelist_head)
 */

static header * first_fit(size_t size) {
  header * current_node = g_freelist_head;
  header * found_space = NULL;

  while (current_node != NULL) {
    if (TRUE_SIZE(current_node) >= size) {
      set_g_next_allocate(current_node);
      found_space = split_block(size, current_node);
      break;
    }
    current_node = current_node->next;
  }
  if (found_space == NULL) {
    int allocate_success = allocate_more_heap(size);
    if (allocate_success) {
      errno = ENOMEM;
      return NULL;
    }

    return first_fit(size);
  }
  else {
    return found_space;
  }
} /* first_fit() */

/*
 *  Allocate the first available block able to satisfy the request
 *  (starting the search at the next free header after the header that was most
 *  recently allocated)
 */

static header * next_fit(size_t size) {
  // keeps the original value of g_next_allocate

  header * original_gna = g_next_allocate;
  header * current_node = g_next_allocate;
  if (current_node == NULL) {
    current_node = g_freelist_head;
  }
  header * found_space = NULL;
  if ((current_node != NULL) && (current_node->size >= size)) {
    set_g_next_allocate(current_node);
    found_space = split_block(size, current_node);
    return found_space;
  }
  current_node = current_node->next;

  while (current_node != original_gna) {
    if (current_node == NULL) {
      // makes sure you circle back around the free list if you reach the end

      current_node = g_freelist_head;
      continue;
    }
    if (current_node->size >= size) {
      set_g_next_allocate(current_node);
      found_space = split_block(size, current_node);
      break;
    }
    current_node = current_node->next;
  }

  if (found_space == NULL) {
    int allocate_success = allocate_more_heap(size);
    if (allocate_success) {
      errno = ENOMEM;
      return NULL;
    }

    return next_fit(size);
  }
  else {
    return found_space;
  }
} /* next_fit() */

/*
 * Traverse the free list of blocks and find the smallest block able to
 * satisfy the request. If there are multiple blocks of the same size that are
 * the best fit, take the FIRST instance.
 */

static header * best_fit(size_t size) {
  header * current_pick = NULL;
  header * current_node = g_freelist_head;

  while (current_node != NULL) {
    if (current_node->size >= size) {
      if (current_pick == NULL) {
        current_pick = current_node;
      }
      else if (current_pick->size > current_node->size) {
        //current_node is the new best fit

        current_pick = current_node;
      }
    }
    current_node = current_node->next;
  }

  if (current_pick == NULL) {
    int allocate_success = allocate_more_heap(size);
    if (!allocate_success) {
      errno = ENOMEM;
      return NULL;
    }

    return best_fit(size);
  }
  else {
    set_g_next_allocate(current_pick);
    header * found_space = split_block(size, current_pick);
    return found_space;
  }
} /* best_fit() */

/*
 * Traverse the free list of blocks and find the largest block able to satisfy
 * the request. If there are multiple blocks of the same size that are the worst
 * fit, take the FIRST instance.
 */

static header * worst_fit(size_t size) {
  header * current_pick = NULL;
  header * current_node = g_freelist_head;

  while (current_node != NULL) {
    if (current_node->size >= size) {
      if (current_pick == NULL) {
        current_pick = current_node;
      }
      else if (current_pick->size < current_node->size) {
        //current_node is the new worst fit

        current_pick = current_node;
      }
    }
    current_node = current_node->next;
  }

  if (current_pick == NULL) {
    int allocate_success = allocate_more_heap(size);
    if (!allocate_success) {
      errno = ENOMEM;
      return NULL;
    }

    return worst_fit(size);
  }
  else {
    set_g_next_allocate(current_pick);
    header * found_space = split_block(size, current_pick);
    return found_space;
  }
} /* worst_fit() */

/*
 * Returns the address of the block to allocate
 * based on the specified algorithm.
 * If no block is available, returns NULL.
 */

static header * find_header(size_t size) {
  // if the g_freelist_head is NULL, there's no free block available.

  if (g_freelist_head == NULL) {
    if (size < 2 * sizeof(header *)) {
      size = 2 * sizeof(header *);
    }
    int needed_size = size + 3 * ALLOC_HEADER_SIZE;
    int more_heap_size = (((int) needed_size / (int) ARENA_SIZE) + 1) *
      ((int) ARENA_SIZE);

    // used for coalesce()

    header * prev_heap_fencepost = g_last_fence_post;
    void * more_heap = sbrk(more_heap_size);
    if (more_heap == (void *) ALLOCATION_FAIL) {
      // sometimes sbrk() fails

      errno = ENOMEM;
      return NULL;
    }

    set_fenceposts(more_heap, more_heap_size);

    // more_heap_header is the header for the additional heap just sbrk'd

    header * more_heap_header = (header *) (((char *)more_heap) +
        ALLOC_HEADER_SIZE);
    more_heap_header->size = (more_heap_size - 3 * ALLOC_HEADER_SIZE) |
                             ((state) UNALLOCATED);
    more_heap_header->left_size = 0;

    // Inserts the newly allocated heap memory into the freelist
    // insert_free_block() sets next and prev pointers

    insert_free_block(more_heap_header);
    coalesce(prev_heap_fencepost, g_last_fence_post);
  }

  switch (FIT_ALGORITHM) {
    case 1:
      return first_fit(size);
    case 2:
      return next_fit(size);
    case 3:
      return best_fit(size);
    case 4:
      return worst_fit(size);
  }
} /* find_header() */

/*
 * Allocates memory in the heap based on the 'size' argument, rounded up to a
 * multiple of MIN_ALLOCATION.
 */

void * my_malloc(size_t size) {
  pthread_mutex_lock(&g_mutex);

  // size round up and error checking

  if (size == 0) {
    return NULL;
  }

  // Make sure size is a multiple of MIN_ALLOCATION

  if (size % MIN_ALLOCATION != 0) {
    size = size + (MIN_ALLOCATION - (size % MIN_ALLOCATION));
  }

  //determined by (FIT_ALGORITHM) (use find_header(*h))
  //find available free block using an algorithm determined by FIT_ALGORITHM

  header * open_block = find_header(size);
  if (open_block == NULL) {
    errno = ENOMEM;
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }
  open_block = (header *) (((char *) open_block) + ALLOC_HEADER_SIZE);
  pthread_mutex_unlock(&g_mutex);
  return open_block;
} /* my_malloc() */

/*
 * Deallocate memory at the passed in address. If the pointer is NULL, don't do
 * anything. If the memory is already deallocated at that address, an error is
 * thrown.
 */

void my_free(void *p) {
  pthread_mutex_lock(&g_mutex);
  if (p == NULL) {
    // if trying to free a NULL pointer, don't do anything

    return;
  }

  header * ptr = (header *) (((char *) p) - ALLOC_HEADER_SIZE);
  header * right_block = (header *) (((char *) ptr) + TRUE_SIZE(ptr) +
      ALLOC_HEADER_SIZE);
  header * left_block = (header *) (((char *) ptr) - ptr->left_size -
      ALLOC_HEADER_SIZE);
  int right_block_all = right_block->size % MIN_ALLOCATION;
  int left_block_all = left_block->size % MIN_ALLOCATION;

  if ((ptr->size % MIN_ALLOCATION) == (state) UNALLOCATED) {
    assert(false);
  }
  else if ((right_block_all != (state) UNALLOCATED) &&
      (left_block_all != (state) UNALLOCATED)) {
    // right and left blocks are both allocated. put ptr into free list

    ptr->size = TRUE_SIZE(ptr) | (state) UNALLOCATED;
    insert_free_block(ptr);
  }
  else if ((right_block_all == (state) UNALLOCATED) &&
           (left_block_all == (state) UNALLOCATED)) {
    // both right and left are unallocated. coalesce both neighbors with ptr.

    coalesce_blocks(left_block, ptr, right_block);
  }
  else if (right_block_all == (state) UNALLOCATED) {
    // only right block is unallocated. coalesce ptr and right block.

    coalesce_blocks(ptr, right_block, NULL);
  }
  else if (left_block_all == (state) UNALLOCATED) {
    // only left block is unallocated. coalesce ptr and left block

    coalesce_blocks(left_block, ptr, NULL);
  }

  pthread_mutex_unlock(&g_mutex);
} /* my_free() */

/*
 * Calls malloc and sets each byte of
 * the allocated memory to a value.
 */

void * my_calloc(size_t nmemb, size_t size) {
  return memset(my_malloc(size * nmemb), 0, size * nmemb);
} /* my_calloc() */

/*
 * Reallocates an allocated block to a new size and
 * copies the contents to the new block.
 */

void * my_realloc(void *ptr, size_t size) {
  void * mem = my_malloc(size);
  memcpy(mem, ptr, size);
  my_free(ptr);
  return mem;
} /* my_realloc() */
