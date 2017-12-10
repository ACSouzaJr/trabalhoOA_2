#include <iostream>
#include <stdlib.h>
#include "commom.h"
#include <string>
#include <fstream>

using namespace std;
int NRR = 0;

page* createPage()
{
    page *node = new page;

    for(int i = 0;i < MAXKEYS;i++)
    {
        node->keys[i].chavePrimaria = (int) NULL;
        node->keys[i].nrr = (int) NULL;
        node->children[i] = NULL;
    }
    node->children[MAXKEYS] = NULL;

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
    index key;
    int i = 1;

    ifstream myfile("FL_insurance_sample.csv");
    //getline(myfile, line); /*   retira o cabeçalho*/

    //se estiver vazia cria raiz
    if ( root == NULL )
    {
        root = createPage();
        root->keyCount++;
        //ler arquivo
        getline(myfile, line);
        line = line.substr(0, 6);
        key.chavePrimaria = atoi(line.c_str());
        key.nrr = 0;
        root->keys[0] = key;
    }
    
    bool promoted, found;
    index promo_key;
    page *child;

    /*  Preenche arvore*/
    while (getline(myfile, line))
    {
        line = line.substr(0, 6);
        key.chavePrimaria = atoi(line.c_str());
        key.nrr = i;
        promoted = insert(root, key, &child, &promo_key, &found);
        if ( promoted )
        {
            root = promote(promo_key, root, child);
        }
        i++;
    }
    NRR = i;

    return root;
}

page* promote(index promo_key, page *left, page *right) /* key, root, promo_nrr*/
{ 
    page *node = createPage();

    node->keys[0] = promo_key;
    node->children[0] = left;
    node->children[1] = right;
    node->keyCount = 1;

    return node;
}


void traversal(page *node)
{
    int i;
    if (node)
    {
        for (i = 0; i < node->keyCount; i++)
        {
            traversal(node->children[i]);
            cout << node->keys[i].chavePrimaria << ' ';
        }
        traversal(node->children[i]);
    }
}