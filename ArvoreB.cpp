#include <stdio.h>
#include <stdlib.h>
#define ORDEM 3

struct indice
{
    int chavePrimaria;
    int NRR;
};

typedef struct indice indice;


struct node {
    int nChaves; /* Numero de chaves adicionadas ex:'3' */
    indice chaves[ORDEM-1]; /* Chaves adicionadas ex:[A B C]*/
    struct node *filhos[ORDEM];
    bool folha;
};

typedef struct node node;

no *raiz = NULL;
i = 0, j;

node insereNoe(raiz *raiz, int valor ){
    //inserção simples de um nó
    if( raiz == NULL ) //nao tem no
    {
        indice indice;

        raiz = (node *) malloc(sizeof(node));
        raiz->nChaves = 1;

        indice.chavePrimaria = valor;
        NRR = i;

        raiz->chaves[nChaves] = indice;
        for(int i=0;i<ORDEM;i++){
            raiz->filhos[i] = NULL;
        }
    }
    else    //ja tem um no
    {
        if(valor == raiz->chaves[i]{  //verifica se o valor a sr inserido já existe
            printf("O valor da chave já existe");
            return NULL;
        }
        else if{
            if (raiz->chaves[nChaves] != NULL) //se o no esta cheio
            {
                raiz->nChaves += 1;
                raiz->chaves[nChaves] = valor;
            }
            else if()
            {
                
            }
        }
        //Acha local da chave.
        //move todas as chaves maiores para direita.
        if (folha){
            i = nChaves;
            while (i >= 0 && chave[i] > value){
                chaves[i+1] = chaves[i];
                i --;
            }
            chaves[i+1] = value;
            nChaves++;
        }
        
    }

    //inserção de um nó com promoção


}

node removeNoe(raiz *B, int valor)
{
    //remoção simples de uma chave





    // remoção de uma chave em que o nó ficará com menos que o número mínimo
    // de chaves



    // remoção de uma chave em que o nó ficará com menos que o número mínimo
    // de chaves e seu pai também ficará

}

void split(int nChaves, node *root){

    min = nChaves/2 - 1;
    //i = nChaves;

    //criar funcao novo no
    newNode = (node *)malloc(sizeof(node));

    for (size_t i = 0; i < min; i++)
    {
        newNode->chaves[i] = root->chaves[i+min];
    }


}


node* newNode(int value){

    indice indice;

    node *raiz = (node *)malloc(sizeof(node));

    raiz->nChaves = 1;

    indice.chavePrimaria = value;
    indice.NRR = i;

    raiz->chaves[0] = indice;

    for (int i = 0; i < ORDEM; i++)
        raiz->filhos[i] = NULL;
    for (int i = 1; i < ORDEM-1; i++)
        raiz->chaves[i] = (int) NULL;
    

    return raiz;
}
