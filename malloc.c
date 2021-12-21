#include "malloc.h"

t_block first_block = NULL;
size_t align8(size_t size) {
    return ((size & 0x7) == 0)?size:((size >> 3) + 1) << 3;
}

t_block find_block(t_block* last, size_t size) {
    t_block cur = first_block;
    while (cur && !(cur->free && cur->size >= size)) {
        *last = cur;
        cur = cur->next;
    }
    return cur;
}

void block_split(t_block b, size_t size) {
    if (b->size >= METASZ + 8 + size) {
        t_block new = b + METASZ + size;
        new->size = b->size - METASZ - size;
        new->free = 1;
        new->prev = b;
        new->next = b->next;
        new->ptr = new->data;
        b->next = new;
        b->size = size;
    }
}

t_block expend_heap(t_block last, size_t size) {
    t_block b = sbrk(size + METASZ);
    if (b) {
        b->size = size;
        b->free = 1;
        b->ptr = b->data;
        b->prev = NULL;
        b->next = NULL;
    }
    if (last) {
        last->next = b;
        b->prev = last;
    }
    return b;
}

void* malloc(size_t size) {
    //write(1, "malloc %d\n", size);
    t_block b;
    t_block last = NULL;
    size_t size_8 = align8(size);
    if (first_block) {
        b = find_block(&last, size_8);
        if (b) {
            b->free = 0;
            block_split(b, size_8);
            return b->data;
        }
        else {
            if (b = expend_heap(last, size_8)) {
                b->free = 0;
                return b->data;
            }
            else return NULL;
        }
    }
    else {
        first_block = expend_heap(last, size_8);
        first_block->free = 0;
        return first_block->data;
    }
}
void* calloc(size_t size) {
    size_t* p = (size_t*)malloc(size);
    size_t size_8 = align8(size) >> 3;
    for (size_t i = 0; i < size_8; ++i) 
        p[i] = 0;
    return p;
}
void free(void* p) {
    if (isValid(p)) {
        t_block b = p - METASZ;
        t_block left = NULL;
        t_block right = NULL;
        t_block tmp = b;
        b->free = 1;
        while (tmp->prev && (tmp->prev->free)) {
            left = tmp->prev;
            tmp->ptr = 0;
            tmp = tmp->prev;
        }
        tmp = b;
        while (tmp->next && (tmp->next->free)) {
            right = tmp->next;
            tmp->ptr = 0;
            tmp = tmp->next;
        }
        if (!left) left = b;
        if (!right) right = b;
        printf("left=%p right=%p\n", left, right);
        left->free = 1;
        left->size = (void*)right - (void*)left + right->size;
        left->next = right->next;
        left->ptr = left->data;
        if (left->next == NULL) {
            if (left->prev == NULL) 
                first_block = NULL;
            else {
                left->prev->next = NULL;
            }
            brk(left);
        }
    }
}

int isValid(void* p) {
    if (first_block)
        if (p >= ((void*)first_block + METASZ) && p < sbrk(0)) {
            t_block tmp = p - METASZ;
            return (tmp->ptr == tmp->data);
        }
    return 0;
}

void print_block() {
    t_block cur = first_block;
    while (cur) {
        printf("|size:%lu, free:%d| ---> ", cur->size, cur->free);
        cur = cur->next;
    }
    printf("end:%p\n", sbrk(0));
}