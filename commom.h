/*  Vamos fazer o codigo em ingles*/
#include <stdio.h>
#include <stdlib.h>
/* Numero maximo de keys*/
#define MAXKEYS 4
#define MINKEYS MAXKEYS/2

/*  Declaracao da pagina(nó)*/
struct BTPAGE
{
    int keyCount;   /* Numero de chaves*/
    int keys[MAXKEYS];   /*  Chaves*/
    struct BTPAGE *children[MAXKEYS+1];    /*  NRR dos filhos*/
};

typedef struct BTPAGE page;



/*  Funcoes*/
bool search(page *root, int value, int *pos);
bool binarySearch(page *node, int value, int *pos);
bool insert(page *root, int key, page **promo_nrr, int *promo_key);
void insertPage(page *node, int key, page* r_child);
void split(page *oldNode, int key, page **promo_nrr, int *promo_key, page *r_child);
page *createPage();
page *criaArvore(page *root);
page *promote(int promo_key, page *left, page *right);