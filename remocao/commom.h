#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

/* Numero maximo de keys*/
#define MAXKEYS 6
#define MINKEYS MAXKEYS/2

/*	variaveis globais
*	NRR e o nome do arquivo e a pagina
*/
extern int NRR;
extern const char *FILE_NAME;
extern int PAGE;

struct ind
{
    int chavePrimaria;
    int nrr;

};

typedef struct ind indice;

/*  Declaracao da pagina(nó)*/
struct BTPAGE
{
    int keyCount;   /* Numero de chaves*/
    indice keys[MAXKEYS];   /*  Chaves*/
    struct BTPAGE *children[MAXKEYS+1];    /*  NRR dos filhos*/
	int page;
	bool leaf;
};

typedef struct BTPAGE page;

struct regist
{
    int policyID;
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

typedef struct regist registro;

/*  Funcoes*/
bool search(page *root, indice key, int *pos);
bool search_2(page *root, indice key, int *nrr);
bool binarySearch(page *node, indice key, int *pos);
bool insert(page *root, indice key, page **promo_nrr, indice *promo_key, bool *found);
void insertPage(page *node, indice key, page *r_child);
void splitNode(page *oldNode, indice key, page **promo_nrr, indice *promo_key, page *r_child);
page *createPage();
page *criaArvore(page *root);
page *promote(indice promo_key, page *left, page *right);
void traversal(page *node);
void atualizaIndice(page *x, ofstream &myfile);
void printIndice(page *x);
fstream& gotoLine(fstream &file, int num);
void getRegistro(registro *data);
void writeRegistro(fstream &file, registro data);

/*	Remocao arvore*/
void removeChave(page *node, int pos);
void merge(page *node, int pos);
void moveDireita(page *node, int pos);
void moveEsquerda(page *node, int pos);
void ajustaNode(page *node, int pos);
void copiaSucessor(page *node, int pos);
bool deletaChave(indice key, page *node);
bool deletaNoeDaArvore(indice key, page **node);