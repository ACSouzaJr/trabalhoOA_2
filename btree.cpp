#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>

using namespace std;

bool search_2(page *root, int value, int *pos)
{

    page *node;
    //int pos;

    node = root;

    while ( node != NULL )
    {
        *pos = binarySearch(node, value);
        if ( *pos < node->keyCount && node->keys[*pos] == value)
            return true;
        else
            node = node->children[*pos];
    }

    return false;
}

int binarySearch(page *node, int value)
{

    int meio, inicio, fim;
    inicio = 0;
    fim = node->keyCount-1;

    while ( inicio <= fim )
    {
        meio = (inicio + fim) / 2;
        if ( node->keys[meio] == value )
        {
            /*  Encontrou retorna a chave*/
            return meio;
        }
        else if( node->keys[meio] > value )
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
    }   
    /*  Nao encontrou retorna posicao para o filho*/
    return inicio;
}

bool search(page *root, int key, int *pos)
{
    int i;
    for(i = 0;i < root->keyCount && key > root->keys[i];i++)
        ;
    *pos = i;
    if ( *pos < root->keyCount && key == root->keys[*pos])
        return true;
    else
        return false;        
}

    //promo key: promoted key
    //promo r child: numero no criado

    //p b rrn: nrr numro promovido
    //
    bool insert(page *root, int key, page **promo_nrr, int *promo_key)
{

    int pos;
    bool found, promoted;

    /*  Folha*/
    if ( root == NULL )
    {
        *promo_key = key;
        promo_nrr = NULL;
        return true;

    }

    found = search(root, key, &pos);//tem que retornar pagina e posição
    if ( found )
    {
        std::cout << "Essa chave ja existe" << '\n';
        return 0;
    }

    promoted = insert(root->children[pos], key, promo_nrr, promo_key);
    if ( !promoted )
    {
        return false;
    }
    if ( root->keyCount < MAXKEYS ) //folha nao cheia
    {
        insertPage(root, key);
        return false;
    }
    else
    {
        split(root, key, promo_nrr, promo_key);
        return true;
    }
}

void insertPage(page *node, int key)
{
    int i;
    for(i = node->keyCount;key < node->keys[i-1] && i > 0;i--)
    {
        node->keys[i] = node->keys[i-1];
        node->children[i+1] = node->children[i];//nao precisa sempre vai ser folha
    }
    node->keyCount++;
    node->keys[i] = key;
    //node->children[i+1] = r_child;

}

//r_child = filho a direita;

void split(page *oldNode, int key, page **promo_nrr, int *promo_key)
{

    int tempKeys[MAXKEYS+1];
    page *tempCh[MAXKEYS+2];
    int i;

        /*  Move todas as chaves e filhos para um temporario*/
        for (i = 0; i < MAXKEYS; i++)
    {
        tempKeys[i] = oldNode->keys[i];
        tempCh[i] = oldNode->children[i];
    }
    tempCh[i] = oldNode->children[i];

    /*  Encontra lugar para chave que vai ser inserida*/
    for(i = MAXKEYS;key < tempKeys[i-1] && i > 0 ;i--)
    {
        tempKeys[i] = tempKeys[i-1];
        tempCh[i+1] = tempCh[i];
    }
    tempKeys[i] = key;
    //tempCh[i+1] = r_child;


    /*  Cria nova pagina para split*/
    page *newNode = createPage();

    /*  Movimenta chaves, distribuição uniforme */
    for (int i = 0; i < MINKEYS; i++)
    {
        oldNode->keys[i] = tempKeys[i];
        oldNode->children[i] = tempCh[i];
        newNode->keys[i] = tempKeys[i + 1 + MINKEYS];
        newNode->children[i] = tempCh[i + 1 + MINKEYS];

        /*  Marca segunda metado do vetor como vazio*/
        oldNode->keys[i + MINKEYS] = (int) NULL;
        oldNode->children[i + MINKEYS] = NULL;
    }
    oldNode->children[i] = tempCh[i];
    newNode->children[i] = tempCh[i + 1 + MINKEYS];

    newNode->keyCount = MAXKEYS - MINKEYS;
    oldNode->keyCount = MINKEYS;

    *promo_key = tempKeys[MINKEYS]; //-> promoção chave do meio
    *promo_nrr = newNode;
}