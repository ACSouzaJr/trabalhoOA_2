#include <iostream>
#include <stdlib.h>
#include "commom.h"
#include <string>
#include <fstream>

using namespace std;

page* createPage(){

    page *node = (page *) malloc(sizeof(page));

    for(int i = 0;i <= MAXKEYS;i++)
    {
        //node->keys[i] = (int) NULL;
        node->children[i] = NULL;
    }
    //node->children[MAXKEYS] = NULL;

    return node;
}
/*
void lerquivo(){

    string line;
    ifstream myfile("nomeaquivo");

    while (getline(myfile, line)){
    {
}
*/

page* criaArvore(page *root)
{
    string line;
    int key;

    ifstream myfile("teste2.txt");
    //se estiver vazia cria raiz
    if ( root == NULL )
    {
        root = createPage();
        root->keyCount++;
        //ler arquivo
        getline(myfile, line);
        key = atoi(line.c_str());
        root->keys[0] = key;
    }
    
    bool promoted;
    int promo_key;
    page *promo_nrr;

    /*  Preenche arvore*/
    while (getline(myfile, line))
    {
        key = atoi(line.c_str());
        promoted = insert(root, key, &promo_nrr, &promo_key);
        if ( promoted )
        {
            root = promote(promo_key, root, promo_nrr);
        }
    }
    
    return root;
}

page* promote(int promo_key, page *left, page *right){ /* key, root, promo_nrr*/

    page *node = createPage();

    node->keys[0] = promo_key;
    node->children[0] = left;
    node->children[1] = right;
    node->keyCount = 1;

    return node;
}