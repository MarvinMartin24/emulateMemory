#include<stdio.h>
#include<stdbool.h>

#ifndef __MMU__H__
#define __MMU__H__
#define SIZE 65536
typedef short byte_t;
typedef int address_t;


typedef struct memoBlock {
    address_t adr;
    int size;
    bool isfree;
    struct memoBlock *next;
    struct memoBlock *prev;
} memoBlock_t;


typedef struct {
    memoBlock_t* root;
    memoBlock_t* last;
} mem_t;


// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem(){

    mem_t* memo = (mem_t*) malloc(sizeof(mem_t));
    memoBlock_t* initBlock = (mem_t*) malloc(sizeof(memoBlock_t));
    memoBlock_t* tmp = initBlock->next;
    memoBlock_t* prevTmp = initBlock;


    // First memory space
    initBlock->adr = 0;
    initBlock->size = 0;
    initBlock->isfree = true;
    initBlock->next = NULL;
    initBlock->prev = NULL;
    memo->root = initBlock;

    while (){
        
        tmp->adr = prevTmp->adr + 1;
        tmp->size = 0;
        tmp->isfree = true;
        tmp->next = NULL;
        tmp->prev = prevTmp;

        tmp = tmp->next
        prevTmp = prevTmp->next
    }

    return memo
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int size){

    memoBlock_t* tmp = mp->root;

    // Find a free block with an approprieted size
    while(tmp->isfree == false && tmp->size < size)
    {
        //check if not the last block of the memo
        if (tmp->next != NULL){
            tmp = tmp->next;
        }
        else{
            printf("Memory is full");
            break;
        }
    }

    // Use the block
    tmp->isfree = false

}

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int size){

}

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, byte_t val){

}

// read memory from a byte
byte_t myRead(mem_t *mp, address_t p){

}
