#ifndef PART1_H
#define PART1_H

#define BUF_SIZE 32

typedef struct {
  char buf[BUF_SIZE];
  int head;
  int tail;
} bounded_buffer;

void *producer(void *ptr);

void *consumer(void *ptr);

#endif // PART1_H
