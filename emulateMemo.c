#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>

#define SIZE 65536
typedef int address_t;


typedef struct memoBlock {
    address_t adr;
    int size;
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
        block->next = NULL;
        mp->size += size;
        block->adr = mp->size +1;
        res = block->adr - size;
        printf("Size : %d\n", block->adr);
    }
    else{
        res = -1;
        printf("Memory is full\n");
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
    printf("Prev %p\n", block->prev);
    printf("next %p\n\n", block->next);
}

//Verify that a memory block exists (1 if exists, 0 if not)
int exist(mem_t *mp, address_t p, int sz){
    int result = 0;
    
    //Verify that memory, address or size exists.
    if(mp == NULL || p < 0 || sz < 1 || p > 65535 || sz > 65535){
        exit(EXIT_FAILURE);
        printf("Liar !!\n");
    } else {
        result = 1;
        printf("Informations are credible. \n");
    }
    return result;
}

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz){
    bool prev = false, next = false, state = false, get = false;
    
    while(!get){
       printf("p : %d ; mp->root->adr : %d\n", p, mp->root->adr);
        if(p == mp->root->adr) {
            if(sz >= mp->root->size){
                printf("Taille saisie plus grande que l'objet.\nSuppression de tout l'élément...\n");
                if(mp->root->prev){ //Vérification qu'un block précédent existe
                    if(mp->root->next){
                        mp->root->next->prev = NULL;
                    }
                    prev = true;
                }
                
                if(!mp->root->next){ //Vérification qu'un block suivant existe
                    if(mp->root->prev){
                        mp->root->prev->next = NULL;
                    }
                    next = true;
                }
                
                if(prev && next){
                    memoBlock_t *temp;
                    temp= mp->root->next->prev;
                    mp->root->next->prev = mp->root->prev->next;
                    mp->root->prev->next = temp;
                }
                
                
                memoBlock_t* block_temp = (memoBlock_t*) malloc(sizeof(memoBlock_t));
                
                block_temp = mp->root->next;
                
                if(mp->root->next){
                    state = true;
                }
                
                if(state) {
                    get = true;
                    free(block_temp);
                    printf("La suppression est un succès!\n");
                } else {
                    printf("La suppression est un échec.\n");
                }
            }
            if(sz < mp->root->size){
                printf("Rétrécissement de la mémoire allouée...\n");
                get = true;
                mp->root->size = mp->root->size - sz;
            }
        } else {
            if(mp->root->next != NULL){
                mp->root = mp->root->next;
            }
            else{
                get =  true; // Juste pour sortir du while au cas ou il ne trouve pas de block
            }
        }
    }
}

int main() {
    mem_t *mem = initMem();
    
    //Allocation
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
    
    //Free
    myFree(mem, adr2, 3);
    myFree(mem, adr1, 2);
}
