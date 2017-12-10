/*  Vamos fazer o codigo em ingles*/
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

/* Numero maximo de keys*/
#define MAXKEYS 64
#define MINKEYS MAXKEYS/2

extern int NRR;

struct indice
{
    int chavePrimaria;
    int nrr;

};

typedef struct indice index;

/*  Declaracao da pagina(nó)*/
struct BTPAGE
{
    int keyCount;   /* Numero de chaves*/
    index keys[MAXKEYS];   /*  Chaves*/
    struct BTPAGE *children[MAXKEYS+1];    /*  NRR dos filhos*/
};

typedef struct BTPAGE page;

struct registro
{
    string policyID;
    string statecode;
	string county;
	string eq_site_limit;
	string hu_site_limit;
	string fl_site_limit;
	string fr_site_limit;
	string tiv_2011;
	string tiv_2012;
	string eq_site_deductible;
	string hu_site_deductible;
	string fl_site_deductible;
	string fr_site_deductible;
	string point_latitude;
	string point_longitude;
	string line;
	string construction;
	string point_granularity;
};

typedef struct registro registro;

    /*  Funcoes*/
bool search(page *root, index key, int *pos);
bool binarySearch(page *node, index key, int *pos);
bool insert(page *root, index key, page **promo_nrr, index *promo_key, bool *found);
void insertPage(page *node, index key, page *r_child);
void split(page *oldNode, index key, page **promo_nrr, index *promo_key, page *r_child);
page *createPage();
page *criaArvore(page *root);
page *promote(index promo_key, page *left, page *right);
void traversal(page *node);