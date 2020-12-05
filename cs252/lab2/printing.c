#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <my_malloc.h>
#include <printing.h>

#define MALLOC_COLOR "MALLOC_DEBUG_COLOR"

static bool check_env;
static bool use_color;

/**
 * @brief Print just the block's size
 *
 * @param block The block to print
 */
void basic_print(header *block) {
  printf("[%zd]->", TRUE_SIZE(block));
}

/**
 * @brief Print just the block's size
 *
 * @param block The block to print
 */
void print_list(header *block) {
  printf("[%zd]\n", TRUE_SIZE(block));
}

/**
 * @brief return a string representing the allocation status
 *
 * @param size The size of the block
 *
 * @return A string representing the allocation status
 */
static inline const char *allocated_to_string(size_t size) {
  switch(size & 0b111) {
    case (state) UNALLOCATED: 
      return "false";
    case (state) ALLOCATED:
      return "true";
    case (state) FENCEPOST:
      return "fencepost";
  }
  assert(false);
}

static bool check_color() {
  if (!check_env) {
    // genenv allows accessing environment varibles
    const char * var = getenv(MALLOC_COLOR);
    use_color = var != NULL && !strcmp(var, "1337_CoLoRs");
    check_env = true;
  }
  return use_color;
}

/**
 * @brief Change the tty color based on the block's allocation status
 *
 * @param block The block to print the allocation status of
 */
static void print_color(header *block) {
  if (!check_color()) {
    return;
  }

  switch(block->size & 0b111) {
    case (state) UNALLOCATED:
      printf("\033[0;32m");
      break;
    case (state) ALLOCATED:
      printf("\033[0;34m");
      break;
    case (state) FENCEPOST:
      printf("\033[0;33m");
      break;
  }
}

static void clear_color() {
  if (check_color()) {
    printf("\033[0;0m");
  }
}

/**
 * @brief Print the free list pointers if RELATIVE_POINTERS is set to true
 * then print the pointers as an offset from the base of the heap. This allows
 * for determinism in testing. 
 * (due to ASLR https://en.wikipedia.org/wiki/Address_space_layout_randomization#Linux)
 *
 * @param p The pointer to print
 */
void print_pointer(void *p) {
  if (p == NULL) {
    printf("NULLPTR");
  } else {
    if (RELATIVE_POINTERS) {
      printf("%04zd", p - g_base);
    } else {
      printf("%p", p);
    }
  }
}

/**
 * @brief Verbose printing of all of the metadata fields of each block
 *
 * @param block The block to print
 */
void print_object(header *block) {
  print_color(block);

  printf("[\n");
  printf("\taddr: ");
  print_pointer(block);
  puts("");
  printf("\tsize: %zd\n", TRUE_SIZE(block));
  printf("\tleft_size: %zd\n", block->left_size);
  printf("\tallocated: %s\n", allocated_to_string(block->size));
  if (!(block->size & (state) ALLOCATED) && 
      !(block->size & (state) FENCEPOST)) {
    printf("\tprev: ");
    print_pointer(block->prev);
    puts("");

    printf("\tnext: ");
    print_pointer(block->next);
    puts("");
  }
  printf("]\n");

  clear_color();
}

/**
 * @brief Simple printer that just prints the allocation status of each block
 *
 * @param block The block to print
 */
void print_status(header *block) {
  print_color(block);
  switch(block->size & 0b111) {
    case (state) UNALLOCATED:
      printf("[U]");
      break;
    case (state) ALLOCATED:
      printf("[A]");
      break;
    case (state) FENCEPOST:
      printf("[F]");
      break;
  }
  clear_color();
}

/**
 * @brief print the full freelist
 *
 * @param pf Function to perform the header printing
 */
void freelist_print(printFormatter pf) {
  if (!pf) {
    return;
  }

  header *freelist = g_freelist_head;
  printf("%p\n%p\n", freelist, freelist->next);
  do {
    pf(freelist);
    puts("");
  } while ((freelist = freelist->next) != NULL);
  fflush(stdout);
}
