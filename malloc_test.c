#include "malloc.h"
#include <string.h>
extern t_block first_block;
int main() {
    printf("test begin\n");
    void* start = sbrk(0);
    char* s1 = calloc(10);
    char* s2 = calloc(31);
    char* s3 = calloc(12345);
    print_block();
    free(s1 - 1);
    free(s2 + 1);
    print_block();
    free(s2);
    print_block();
    free(s3);
    print_block();
    free(s1);
    print_block();
    free((void*)first_block+40);
    print_block();
    void* end = sbrk(0);
    printf("start=%p\nend=%p\n", start, end);
    return 0;
}