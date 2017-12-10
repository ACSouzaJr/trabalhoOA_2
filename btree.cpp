#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>

using namespace std;

bool search_2(page *root, index key)
{

    page *node;
    int pos;

    node = root;

    while ( node != NULL )
    {
        binarySearch(node, key, &pos);
        if ( pos < node->keyCount && node->keys[pos].chavePrimaria == key.chavePrimaria)
            return true;
        else
            node = node->children[pos];
    }

    return false;
}

bool binarySearch(page *node, index key, int *pos)
{

    int meio, inicio, fim;
    inicio = 0;
    fim = node->keyCount-1;

    while ( inicio <= fim )
    {
        meio = (inicio + fim) / 2;
        if ( node->keys[meio].chavePrimaria == key.chavePrimaria )
        {
            /*  Encontrou retorna a chave*/
            *pos = meio;
            return true;
        }
        else if( node->keys[meio].chavePrimaria > key.chavePrimaria )
        {
            fim = meio - 1;
        }
        else
        {
            inicio = meio + 1;
        }
    }   
    /*  Nao encontrou retorna posicao para o filho*/
    *pos = inicio;
    return false;
}

/*  busca sequencial*/
bool search(page *root, index key, int *pos)
{
    int i;
    for(i = 0;i < root->keyCount && key.chavePrimaria > root->keys[i].chavePrimaria;i++)
        ;
    *pos = i;
    if ( *pos < root->keyCount && key.chavePrimaria == root->keys[*pos].chavePrimaria)
        return true;
    else
        return false;        
}

/*  Recebe: pagina, chave a ser adicionada
*   Retorna: chave a ser promovida, seus filhos e flag se a chave foi encontrada.
*/
bool insert(page *root, index key, page **promo_nrr, index *promo_key, bool *found)
{

    int pos;
    bool promoted;
    page *p_b_rrn;
    index p_b_key;

    /*  Folha*/
    if ( root == NULL )
    {
        *promo_key = key;
        *promo_nrr = NULL;
        return true;

    }

    (*found) = binarySearch(root, key, &pos); /*   Retorna a posicao relativa da chave*/
    if ( (*found) )
    {
        std::cout << "Essa chave ja existe" << '\n';
        return false;
    }

    promoted = insert(root->children[pos], key, &p_b_rrn, &p_b_key, found);
    if ( !promoted )
    {
        return false;
    }
    if ( root->keyCount < MAXKEYS ) /*  Pagina nao esta cheia*/
    {
        insertPage(root, p_b_key, p_b_rrn);
        return false;
    }
    else
    {
        split(root, p_b_key, promo_nrr, promo_key, p_b_rrn);
        return true;
    }
}

/*  Insere na pagina nao vazia*/
void insertPage(page *node, index key, page *r_child)
{
    int i;
    for(i = node->keyCount;key.chavePrimaria < node->keys[i-1].chavePrimaria && i > 0;i--)
    {
        node->keys[i] = node->keys[i-1];
        node->children[i+1] = node->children[i];
    }
    node->keyCount++;
    node->keys[i] = key;
    node->children[i+1] = r_child;

}

void split(page *oldNode, index key, page **promo_nrr, index *promo_key, page *r_child)
{

    index tempKeys[MAXKEYS + 1];
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
    for(i = MAXKEYS;key.chavePrimaria < tempKeys[i-1].chavePrimaria && i > 0 ;i--)
    {
        tempKeys[i] = tempKeys[i-1];
        tempCh[i+1] = tempCh[i];
    }
    tempKeys[i] = key;
    tempCh[i+1] = r_child;


    /*  Cria nova pagina para split*/
    page *newNode = createPage();

    /*  Movimenta chaves, distribuição uniforme */
    for (i = 0; i < MINKEYS; i++)
    {
        oldNode->keys[i] = tempKeys[i];
        oldNode->children[i] = tempCh[i];
        newNode->keys[i] = tempKeys[i + 1 + MINKEYS];
        newNode->children[i] = tempCh[i + 1 + MINKEYS];

        /*  Marca segunda metado do vetor como vazio*/
        oldNode->keys[i + MINKEYS].chavePrimaria = (int) NULL;
        oldNode->keys[i + MINKEYS].nrr = (int) NULL;
        oldNode->children[i + MINKEYS] = NULL;
    }
    oldNode->children[MINKEYS] = tempCh[MINKEYS];
    newNode->children[MINKEYS] = tempCh[i + 1 + MINKEYS];

    newNode->keyCount = MAXKEYS - MINKEYS;
    oldNode->keyCount = MINKEYS;

    *promo_key = tempKeys[MINKEYS]; //-> promoção chave do meio
    *promo_nrr = newNode;
}