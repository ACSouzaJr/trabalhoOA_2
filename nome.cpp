#include <iostream>
#include <stdlib.h>
#include "commom.h"
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstring>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;
int NRR = 0;
const char  *FILE_NAME  = "teste3.txt";
int PAGE = 0;

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
    node->leaf = true;
    node->page = PAGE;
    PAGE++;

    return node;
}

page* criaArvore(page *root)
{
    string line;
    indice key;
    int i = 0;

    ifstream myfile(FILE_NAME);
    //getline(myfile, line); /*   retira o cabeçalho*/
    
    bool promoted, found;
    indice promo_key;
    page *child;

    /*  Preenche arvore*/
    while (getline(myfile, line))
    {
        if ( line[0] != '*')
        {
            line = line.substr(0, 6);
            key.chavePrimaria = atoi(line.c_str());
            key.nrr = i;
            //se estiver vazia cria raiz
            if ( root == NULL )
            {
                root = createPage();
                root->keyCount++;
                root->keys[0] = key;
            }
            else
            {    
                promoted = insert(root, key, &child, &promo_key, &found);
                if ( promoted )
                {
                    root = promote(promo_key, root, child);
                }
            }
            i++;
        }
    }
    NRR = i;
    //retirar depois
    cout << endl;

    return root;
}

page* promote(indice promo_key, page *left, page *right) /* key, root, promo_nrr*/
{ 
    page *node = createPage();

    node->keys[0] = promo_key;
    node->children[0] = left;
    node->children[1] = right;
    node->keyCount = 1;
    node->leaf = false;

    return node;
}


void traversal(page *node)
{
    /*  Inorder(left, root, right)*/
    /*
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
    */
    /*  BFS*/
    std::queue<page *> queue;
    queue.push(node);
    while (!queue.empty())
    {
        page *current = queue.front();
        queue.pop();
        int i;
        for (i = 0; i < current->keyCount; i++)
        {
            if (current->leaf == false)
                queue.push(current->children[i]);
            cout << " " << current->keys[i].chavePrimaria;
        }
        if (current->leaf == false)
            queue.push(current->children[i]);
    }
}

void displayNode(page *x, ofstream &myfile)
{
    //ofstream myfile("indicelista.bt", ios::app);

    myfile << x->page << "   |";

    for (int i = 0;x->keys[i].chavePrimaria != NULL && i < x->keyCount;i++)
    {
        myfile << left << setw(6) << x->keys[i].chavePrimaria << "|";
    }
    myfile << internal << setw(7) << "";

    for (int i = 0;x->children[i] != NULL && i <= x->keyCount;i++)
    {
        myfile << x->children[i]->page << "|";
    }
    myfile << endl;
    for (int i = 0; x->children[i] != NULL && i <= x->keyCount; i++)
    {
        displayNode(x->children[i], myfile);
    }
}

/*
    entrada: fstrem, numero da linha;
    saida: fstream para o inicio da linha escolhida;
*/
fstream& gotoLine(fstream &file, int num){

    string line;
    file.seekg(ios::beg);
    for(int i = 0;i < num;i++)
    {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        //getline(file, line); ^ mais bonito.
    }

    return file;
}


void ArvoreB(){

}