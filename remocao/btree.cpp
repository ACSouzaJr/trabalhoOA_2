#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>

using namespace std;

bool search_2(page *root, indice key, int *nrr)
{

    page *node;
    int pos;

    node = root;

    while ( node != NULL )
    {
        binarySearch(node, key, &pos);
        if ( pos < node->keyCount && node->keys[pos].chavePrimaria == key.chavePrimaria)
        {
            *nrr = node->keys[pos].nrr;
            return true;
        }            
        else
            node = node->children[pos];
    }

    return false;
}

/*  NAO FUNCIONA COM O DATABASE*/
bool binarySearch(page *node, indice key, int *pos)
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
            cout << node->page;
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
    cout << node->page << "->";
    return false;
}

/*  busca sequencial*/
bool search(page *root, indice key, int *pos)
{
    int i;
    for(i = 0;i < root->keyCount && key.chavePrimaria > root->keys[i].chavePrimaria;i++)
        ;
    *pos = i;
    if ( *pos < root->keyCount && key.chavePrimaria == root->keys[*pos].chavePrimaria)
        return true;
    else
    {
        //cout << root->page << "->";
        return false;        
    }        
}

/*  Recebe: pagina, chave a ser adicionada
*   Retorna: chave a ser promovida, seus filhos e flag se a chave foi encontrada.
*/
bool insert(page *root, indice key, page **promo_nrr, indice *promo_key, bool *found)
{

    int pos;
    bool promoted;
    page *p_b_rrn;
    indice p_b_key;

    /*  Folha*/
    if ( root == NULL )
    {
        *promo_key = key;
        *promo_nrr = NULL;
        return true;

    }

    (*found) = search(root, key, &pos); /*   Retorna a posicao relativa da chave*/
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
        splitNode(root, p_b_key, promo_nrr, promo_key, p_b_rrn);
        return true;
    }
}

/*  Insere na pagina nao vazia*/
void insertPage(page *node, indice key, page *r_child)
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

void splitNode(page *oldNode, indice key, page **promo_nrr, indice *promo_key, page *r_child)
{

    indice tempKeys[MAXKEYS + 1];
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
    newNode->leaf = oldNode->leaf;
    
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

/************************************REMOÇÃO*****************************************/

// diminui o número de chaves na página
void removeChave(page *node, int pos)
{
    int i;
    for (i = pos + 1; i < node->keyCount; ++i)
    {
        node->keys[i - 1] = node->keys[i];
    }

    node->keyCount--;
}

/* funde os nós irmãos */
void merge(page *node, int pos)
{   

    page *child = node->children[pos];
    page *irmao = node->children[pos + 1];

    // pega uma chave do nó atual e insere na posição do valor mínimo de chaves -1
    child->keys[child->keyCount] = node->keys[pos];

    // copia as chaves do filho pro irmão
    for (int i = 0; i < irmao->keyCount; ++i)
        child->keys[i + child->keyCount + 1] = irmao->keys[i];

    // copia os ponteiros do filho pro irmão
    if (!child->leaf)
    {
        for (int i = 0; i <= irmao->keyCount; ++i)
            child->children[i + child->keyCount + 1] = irmao->children[i];
    }

    // preenche os gaps movendo as chaves para tras
    for (int i = pos + 1; i < node->keyCount; ++i)
        node->keys[i - 1] = node->keys[i];

    // move os ponteiros dos filhos para tras
    for (int i = pos + 2; i <= node->keyCount; ++i)
        node->children[i - 1] = node->children[i];

    // atualiza o número de chaves do filho e de seus irmão
    child->keyCount += irmao->keyCount + 1;
    node->keyCount--;

    // libera a memória ocupada pelo irmão
    delete irmao;

    return;
}

/* move a chave do pai para o filho da direita */
void moveDireita(page *node, int pos)
{

    page *filho = node->children[pos];
    page *irmao = node->children[pos - 1];


    // Move todas as chaves do filho para frente
    for (int i = filho->keyCount - 1; i >= 0; --i)
        filho->keys[i + 1] = filho->keys[i];

    // Se for uma folha, os ponteiros dos filhos são movidos para frente
    if (!filho->leaf)
    {
        for (int i = filho->keyCount; i >= 0; --i)
            filho->children[i + 1] = filho->children[i];
    }

    // Setting child's first key equal to keys[idx-1] from the current node
    filho->keys[0] = node->keys[pos - 1];

    // Move o último filho do irmão e o primeiro de todos os filhos
    if (!node->leaf)
        filho->children[0] = irmao->children[irmao->keyCount];

    // Move a chave do irmao para seu pai, reduzindo o número de chaves no irmão
    node->keys[pos - 1] = irmao->keys[irmao->keyCount - 1];

    filho->keyCount++;
    irmao->keyCount--;

    return;
}

/* move a chave do pai para o filho da esquerda */
void moveEsquerda(page *node, int pos)
{

    page *filho = node->children[pos];
    page *irmao = node->children[pos + 1];

    // a chave é inserida como ultima em keys
    filho->keys[(filho->keyCount)] = node->keys[pos];

    // o primeiro filho do irmao é inserido como último filho em children
    if (!(filho->leaf))
        filho->children[(filho->keyCount) + 1] = irmao->children[0];

    //a primeira chave do irmao é inserida em keys
    node->keys[pos] = irmao->keys[0];

    // Move todas as chaves no irmao para tras
    for (int i = 1; i < irmao->keyCount; ++i)
        irmao->keys[i - 1] = irmao->keys[i];

    // Move todos os ponteiros do irmao para tras
    if (!irmao->leaf)
    {
        for (int i = 1; i <= irmao->keyCount; ++i)
            irmao->children[i - 1] = irmao->children[i];
    }

    // O número de chaves do filho aumenta e consequentemente do irmao diminui
    // em razão da movimentação
    filho->keyCount++;
    irmao->keyCount--;

    return;
}

/* Ajusta as chaves dentro do nó */
void ajustaNode(page *node, int pos)
{
    if (!pos)
    {
        if (node->children[1]->keyCount > MINKEYS)
        {
            moveEsquerda(node, 0);
        }
        else
        {
            merge(node, 0);
        }
    }
    else
    {
        if (node->keyCount != pos)
        {
            if (node->children[pos - 1]->keyCount > MINKEYS)
            {
                moveDireita(node, pos);
            }
            else
            {
                if (node->children[pos + 1]->keyCount > MINKEYS)
                {
                    moveEsquerda(node, pos);
                }
                else
                {
                    merge(node, pos);
                }
            }
        }
        else
        {
            if (node->children[pos - 1]->keyCount > MINKEYS)
                moveDireita(node, pos - 1);
            else
                merge(node, pos - 1);
        }
    }
}

void copiaSucessor(page *node, int pos)
{
    page *temp;
    temp = node->children[pos];

    //
    while (!temp-> leaf)
        temp = temp->children[temp->keyCount];

    node->keys[pos] = temp->keys[temp->keyCount-1];
}

/* deleta uma chave do nó */
bool deletaChave(indice key, page *node)
{
    int pos, flag = 0;
    if (node)
    {
        /* Se a chave estive na primeira posicao - otimo*/
        if (key.chavePrimaria < node->keys[0].chavePrimaria)
        {
            pos = 0;
            flag = 0;
        }
        else //caso contrario acha local
        {
            flag = search(node, key, &pos);
        }

        /*  Se a chave foi encontrada*/
        if (flag)
        {
            /*  Se a nó for folha*/
            if (!node->leaf)
            { 
                 
                copiaSucessor(node, pos);
                flag = deletaChave(node->keys[pos], node->children[pos]);
                if (flag == 0)
                {
                    cout << "\nA chave a ser deletada nao pertence a arvore\n";
                }
            }
            else //se for a unica apenas remove
            {
                removeChave(node, pos);
            }
        }
        else //se a chave nao foi encontrada
        {
            //continua procurando
            flag = deletaChave(key, node->children[pos]);
        }

        /*  se o numero de chaves for menor que o min ajusta*/
        if (node->children[pos])
        {
            if (node->children[pos]->keyCount < MINKEYS)
                ajustaNode(node, pos);
        }
    }
    return flag;
}

/* deleta nó da B-tree */
bool deletaNoeDaArvore(indice key, page **node)
{
    

    if (!deletaChave(key, (*node)))
    {
        printf("\nA chave a ser deletada nao pertence a arvore\n");
        return false;
    }
    else
    {
        //se a raiz possui 0 chaves troca raiz de posicao
        if ((*node)->keyCount == 0)
        {
            page *tmp;
            tmp = (*node);
            if ( (*node)->leaf)
            {
                (*node) = NULL;
            }
            else
            {
                (*node) = (*node)->children[0];
            }
            delete tmp;
        }
    };
    return true;
}
