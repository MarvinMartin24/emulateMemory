#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>

#define SIZE 65536
typedef int address_t;


typedef struct memoBlock {
    address_t adr;
    int size;
    bool isfree;
    struct memoBlock *next;
    struct memoBlock *prev;
} memoBlock_t;


typedef struct {
    int size;
    memoBlock_t* root;
    memoBlock_t* last;
} mem_t;


// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem(){

    mem_t* memo = (mem_t*) malloc(sizeof(mem_t));
    memoBlock_t* initBlock = (memoBlock_t*) malloc(sizeof(memoBlock_t));

    // First memory space
    initBlock->adr = 0;
    initBlock->size = 0;
    initBlock->isfree = true;
    initBlock->next = NULL;
    initBlock->prev = NULL;
    memo->root = initBlock;
    memo->last = NULL;

    memo->size = initBlock->size;

    return memo;
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int size){

    bool isAllocated = false;
    address_t res = -1;
    memoBlock_t* tmp = mp->root;
    memoBlock_t* block = (memoBlock_t*) malloc(sizeof(memoBlock_t));

    if ((mp->size + size) <= SIZE){
        res = -1;
    }

    while (tmp != NULL){

        if(tmp->isfree == true){
            tmp->size = size;
            tmp->isfree = false;
            mp->size += size;
            tmp->adr = mp->size + 1;
            isAllocated = true;
            res = tmp->adr - size;
        }
        else{
            tmp = tmp->next;
        }
    }
    // No more free blocks
    if (!isAllocated){
        if ((mp->size + size) <= SIZE){
            //Create a new block
            if (mp->last == NULL){
                block->adr = mp->root->adr + 1;
                mp->root->next = block;
                block->prev = mp->root;
                mp->last = block;
            }
            else{
                block->adr = mp->last->adr + 1;
                block->prev = mp->last;
                mp->last = block;
            }
            block->size = size;
            block->isfree = false;
            block->next = NULL;
            mp->size += size;
            block->adr = mp->size +1;
            res = block->adr - size;
        }
        else{
            printf("Memory is full\n");
        }
    }
    return res;
}

void displayMemo(mem_t* mp){
    printf("Size %d\n", mp->size);
    printf("root %p\n", mp->root);
    printf("next %p\n\n", mp->root->next);

}

void displayBlock(memoBlock_t* block){
    printf("Address %d\n", block->adr);
    printf("Size %d\n", block->size);
    printf("isFree %d\n", block->isfree);
    printf("Prev %p\n", block->prev);
    printf("next %p\n\n", block->next);
}

int main() {
    mem_t *mem = initMem();
    address_t adr1 = myAlloc(mem, 5);
    address_t adr2 = myAlloc(mem, 6);
    address_t adr3 = myAlloc(mem, 8);
    address_t adr4 = myAlloc(mem, 1000);
    address_t adr5 = myAlloc(mem, 1);




    printf("%d ->%d\n", adr1, 5);
    printf("%d ->%d\n", adr2, 6);
    printf("%d ->%d\n", adr3, 8);
    printf("%d ->%d\n", adr4, 1000);
    printf("%d\n", adr5);

}
