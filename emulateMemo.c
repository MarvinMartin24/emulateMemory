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
    int mem[SIZE];
    int size;
    memoBlock_t* root;
    memoBlock_t* last;
} mem_t;


// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem(){

    mem_t* memo = (mem_t*) malloc(sizeof(mem_t));
    memoBlock_t* initBlock = (memoBlock_t*) malloc(sizeof(memoBlock_t));

    // First memory space
    initBlock->adr = 1;
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

    bool isAllocated = false, get = false;
    address_t res = -1;
    memoBlock_t* tmp = mp->root;
    memoBlock_t* block = (memoBlock_t*) malloc(sizeof(memoBlock_t));
    while(!get){
        if(!mp->root->next){
            if ((mp->size + size) <= SIZE){
                //Create a new block
               
                block->adr = mp->size + 1;
                mp->root->next = block;
                block->prev = mp->root;

                block->size = size;
                block->next = NULL;
                mp->size += size;

                res = block->adr;
                printf("[myAlloc] Size : %d\n", block->adr);
            }
            else{
                res = -1;
                printf("[myAlloc] Memory is full\n");
            }
            get = true;
        } else{
            if(mp->root->next != NULL ){
                mp->root = mp->root->next;
            } else {
                get = true; // Juste pour sortir du while au cas ou il ne trouve pas de block
            }
        }
    }

    return res;
}

void displayMemo(mem_t* mp){
    printf("[displayMemo] Size %d\n", mp->size);
    printf("[displayMemo] root %p\n", mp->root);
    printf("[displayMemo] next %p\n\n", mp->root->next);

}

void displayBlock(memoBlock_t* block){
    printf("[displayBlock] Address %d\n", block->adr);
    printf("[displayBlock] Size %d\n", block->size);
    printf("[displayBlock] Prev %p\n", block->prev);
    printf("[displayBlock] next %p\n\n", block->next);
}

//Verify that a memory block exists (1 if exists, 0 if not)
int exist(mem_t *mp, address_t p, int sz){
    int result = 0;
    
    //Verify that memory, address or size exists.
    if(mp == NULL || p < 0 || sz < 1 || p > 65535 || sz > 65535){
        exit(EXIT_FAILURE);
        printf("[exist] Liar !!\n");
    } else {
        result = 1;
        printf("[exist] Informations are credible. \n");
    }
    return result;
}

void backtofirst(mem_t *mp){
    while( mp->root->prev){
        mp->root = mp->root->prev;
    }
}

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz){
    bool prev = false, next = false, state = false, get = false;
    
    while(!get){
        if(p == mp->root->adr) {
            if(sz >= mp->root->size){
                printf("[myFree] Taille saisie plus grande que l'objet.\n[myFree] Suppression de tout l'élément...\n");
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
                    printf("[myFree] La suppression est un succès!\n\n");
                } else {
                    printf("[myFree] La suppression est un échec.\n\n");
                }
            }
            if(sz < mp->root->size){
                printf("[myFree] Rétrécissement de la mémoire allouée...\n");
                get = true;
                mp->root->size = mp->root->size - sz;
                printf("[myFree] La suppression est un succès!\n\n");
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

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, int val){
    bool get = false;
    if(exist(mp, p, 1) == 1){
        while(!get){
             if(mp->root->adr == p){
                 printf("[myWrite] ");
                 for(int i = 0; i < mp->root->size; i++){
                     mp->mem[p + i] = val;
                 }
                 get = true;
             } else {
                 if(mp->root->next != NULL ){
                     mp->root = mp->root->next;
                 } else {
                     get = true; // Juste pour sortir du while au cas ou il ne trouve pas de block
                 }
             }
         }
     }
}

// read memory from a byte
void myRead(mem_t *mp, address_t p){
    bool get = false;
    if(exist(mp, p, 1) == 1){
        while(!get){
            if(mp->root->adr == p){
                printf("[myRead] ");
                for(int i = 0 ; i < mp->root->size; i++){
                    printf("%d", mp->mem[p+ i]);
                }
                get = true;
            } else {
                if(mp->root->next){
                    mp->root = mp->root->next;
                } else {
                    get = true; // Juste pour sortir du while au cas ou il ne trouve pas de block
                }
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
    backtofirst(mem);
    myFree(mem, adr1, 2);
    backtofirst(mem);
    
    //Writing
    myWrite(mem, adr3, 3);
    backtofirst(mem);
    
    //Reading
    myRead(mem, adr3);
    backtofirst(mem);
}
