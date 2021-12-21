#ifndef __MALLOC__
#define __MALLOC__
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
struct block {
    //meta
    size_t size;
    struct block* prev;
    struct block* next;
    int free;
    int padding;
    void* ptr;
    //data
    char data[1];
};
typedef struct block* t_block;

void* malloc(size_t);
void* calloc(size_t);
void free(void*);
t_block find_block(t_block*, size_t);
void block_split(t_block, size_t);
t_block expend_heap(t_block, size_t);
size_t align8(size_t);
int isValid(void*);
void print_block();
#define METASZ (sizeof(struct block) - 8)

#endif
