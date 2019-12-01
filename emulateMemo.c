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

    memo->size = 0;

    return memo;
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int size){

    bool available = true;
    memoBlock_t* tmp = mp->root;
    memoBlock_t* block = (memoBlock_t*) malloc(sizeof(memoBlock_t));


    // Find a free block with an approprieted size
    while(tmp->isfree == false && tmp->size <= size)
    {
        //check if not the last block of the memo
        if (tmp->next != NULL){
            tmp = tmp->next;
        }
        else{
            available = false;
            printf("No block available\n");
            break;
        }
    }
    // Take an existing free block
    if (available){
        tmp->size = size;
        tmp->isfree = false;
        mp->size += size;
        return tmp->adr;
    }
    // No more free blocks
    else{
        if ((mp->size + size) <= SIZE){
            //Create a new block
            if (mp->last == NULL){
                block->adr = mp->root->adr + 1;
                mp->root->next = block;
                block->prev = mp->root;

            }
            else{
                block->adr = mp->last->adr + 1;
                mp->last->next = block;
                block->prev = mp->last;
            }
            block->size = size;
            block->isfree = false;
            block->next = NULL;
            mp->size += size;
            mp->last = block;
            return block->adr;
        }
        else{
            printf("Memory is full");
        }

    }
    return 0;

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
    printf("netx %p\n\n", block->next);
}

int main() {
    mem_t *mem = initMem();
    displayMemo(mem);
    displayBlock(mem->root);
    address_t adr1 = myAlloc(mem, 5);
    displayMemo(mem);
    displayBlock(mem->root);

    address_t adr2 = myAlloc(mem, 6);
    displayMemo(mem);
    displayBlock(mem->root->next);

    printf("%d", adr1);
    printf("%d", adr2);

}
