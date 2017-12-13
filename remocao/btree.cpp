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
        cout << root->page << "->";
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

/**************************************************************************************/

// diminui o número de chaves na página
void removeChave(page *node, int pos)
{
    int i;
    for (i = pos + 1; i < node->keyCount; ++i)
    {
        node->keys[i - 1] = node->keys[i];
        //node->children[i - 1] = node->children[i];
    }
    //node->children[i - 1] = node->children[i];

    node->keyCount--;
}

/* funde os nós */
void merge(page *node, int pos)
{   /*
    int j = 1;
    page *filho1 = node->children[pos], *filho2 = node->children[pos - 1];

    filho2->keyCount++;
    filho2->keys[filho2->keyCount] = node->keys[pos];
    filho2->children[filho2->keyCount] = node->children[0];

    while (j <= filho1->keyCount)
    {
        filho2->keyCount++;
        filho2->keys[filho2->keyCount] = filho1->keys[j];
        filho2->children[filho2->keyCount] = filho1->children[j];
        j++;
    }

    j = pos;
    while (j < node->keyCount)
    {
        node->keys[j] = node->keys[j + 1];
        node->children[j] = node->children[j + 1];
        j++;
    }
    node->keyCount--;
    free(filho1);
    */

    page *child = node->children[pos];
    page *sibling = node->children[pos + 1];

    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
    child->keys[MINKEYS - 1] = node->keys[pos];

    // Copying the keys from C[idx+1] to C[idx] at the end
    for (int i = 0; i < sibling->keyCount; ++i)
        child->keys[i + MINKEYS] = sibling->keys[i];

    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->keyCount; ++i)
            child->children[i + MINKEYS] = sibling->children[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for (int i = pos + 1; i < node->keyCount; ++i)
        node->keys[i - 1] = node->keys[i];

    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i = pos + 2; i <= node->keyCount; ++i)
        node->children[i - 1] = node->children[i];

    // Updating the key count of child and the current node
    child->keyCount += sibling->keyCount + 1;
    node->keyCount--;

    // Freeing the memory occupied by sibling
    delete sibling;

    return;
}

/* move a chave do pai para o filho da direita */
void moveDireita(page *node, int pos)
{
    /*
    page *x = node->children[pos];
    int j = x->keyCount;

    while (j > 0)
    {
        x->keys[j + 1] = x->keys[j];
        x->children[j + 1] = x->children[j];
    }
    x->keys[1] = node->keys[pos];
    x->children[1] = x->children[0];
    x->keyCount++;

    x = node->children[pos - 1];
    node->keys[pos] = x->keys[x->keyCount];
    node->children[pos] = x->children[x->keyCount];
    x->keyCount--;
    return;
    */

    page *child = node->children[pos];
    page *sibling = node->children[pos - 1];

    // The last key from children[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in children[idx]. Thus, the  loses
    // sibling one key and child gains one key

    // Moving all key in children[idx] one step ahead
    for (int i = child->keyCount - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    // If children[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->leaf)
    {
        for (int i = child->keyCount; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    // Setting child's first key equal to keys[idx-1] from the current node
    child->keys[0] = node->keys[pos - 1];

    // Moving sibling's last child as children[idx]'s first child
    if (!node->leaf)
        child->children[0] = sibling->children[sibling->keyCount];

    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    node->keys[pos - 1] = sibling->keys[sibling->keyCount - 1];

    child->keyCount++;
    sibling->keyCount--;

    return;
}

/* move a chave do pai para o filho da esquerda */
void moveEsquerda(page *node, int pos)
{
    /*
    int j = 1;
    page *x = node->children[pos - 1];

    x->keyCount++;
    x->keys[x->keyCount] = node->keys[pos];
    x->children[x->keyCount] = node->children[pos]->children[0];

    //sobe chave da direita para um nivel acima
    x = node->children[pos];
    node->keys[pos] = x->keys[1];
    x->children[0] = x->children[1];
    x->keyCount--;

    while (j <= x->keyCount)
    {
        x->keys[j] = x->keys[j + 1];
        x->children[j] = x->children[j + 1];
        j++;
    }
    return;
    */

    page *child = node->children[pos];
    page *sibling = node->children[pos + 1];

    // keys[idx] is inserted as the last key in node->children[idx]
    child->keys[(child->keyCount)] = node->keys[pos];

    // Sibling's first child is inserted as the last child
    // into node->children[idx]
    if (!(child->leaf))
        child->children[(child->keyCount) + 1] = sibling->children[0];

    //The first key from sibling is inserted into keys[idx]
    node->keys[pos] = sibling->keys[0];

    // Moving all keys in sibling one step behind
    for (int i = 1; i < sibling->keyCount; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->keyCount; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->keyCount++;
    sibling->keyCount--;

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
                moveDireita(node, pos);
            else
                merge(node, pos);
        }
    }
}

void copiaSucessor(page *node, int pos)
{
    page *temp;
    temp = node->children[pos+1];

    //
    while (!temp-> leaf)
        temp = temp->children[0];

    node->keys[pos] = temp->keys[0];
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
            /*for (pos = node->keyCount; key.chavePrimaria < node->keys[pos].chavePrimaria && pos > 0; pos--)
            {
                printf("Remoção ocorreu corretamente \n");
            }
            if (key.chavePrimaria == node->keys[pos].chavePrimaria)
            {
                flag = 1;
            }
            else
            {
                flag = 0;
            }*/
            flag = search(node, key, &pos);
        }

        /*  Se a chave foi encontrada*/
        if (flag)
        {
            /*  Se a nó for folha*/
            //if (node->children[pos - 1])
            if (!node->leaf)
            { 
                copiaSucessor(node, pos);
                flag = deletaChave(node->keys[pos], node->children[pos+1]);
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
        //se a raiz possui 0 chaves 
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
/***************************************************************************************/

/*void removePage(page *node, indice key, page *r_child)
{
    int i;
    for (i = node->keyCount; key.chavePrimaria < node->keys[i - 1].chavePrimaria && i > 0; i--)
    {
        node->keys[i] = node->keys[i-1];
        node->children[i+1] = node->children[i];
    }
    node->keyCount--;
}

void mergeNode(page *node1, page *node2, indice key, page **promo_nrr, indice *promo_key, page *r_child)
{

    indice tempKeys[MAXKEYS + 1];
    page *tempCh[MAXKEYS+2];
    int i;

    //  Move todas as chaves e filhos para um temporario
    for (i = 0; i < MAXKEYS; i++)
    {
        tempKeys[i] = node1->keys[i];
        tempCh[i] = node1->children[i];
    }
    tempCh[i] = node1->children[i];

    //  Encontra lugar para chave que vai ser inserida
    for (i = MAXKEYS; key.chavePrimaria < tempKeys[i - 1].chavePrimaria && i > 0; i--)
    {
        tempKeys[i] = tempKeys[i - 1];
        tempCh[i + 1] = tempCh[i];
    }
    tempKeys[i] = key;
    tempCh[i + 1] = r_child;

    //  Movimenta chaves, distribuição uniforme 
    for (i = 0; i < MINKEYS; i++)
    {
        node1->keys[i] = tempKeys[i];
        node1->children[i] = tempCh[i];
    }
    node1->children[MINKEYS] = tempCh[MINKEYS];

    node1->keyCount = node1->keyCount + node2->keyCount;

    *promo_key = tempKeys[MINKEYS]; //-> promoção chave do meio
    *promo_nrr = *root; // a nrr do novo elemento é o pai dos nós que foram concatenados
}

bool delete(page *root, indice key, page **promo_nrr, indice *promo_key, bool *found)
{

    int pos;
    bool promoted;
    page *p_b_rrn;
    indice p_b_key;


    (*found) = search(root, key, &pos); //   Retorna a posicao relativa da chave
    if ((*found))
    {
        std::cout << "Essa chave ja existe" << '\n';
        return false;
    }

    promoted = insert(root->children[pos], key, &p_b_rrn, &p_b_key, found);
    if (!promoted)
    {
        return false;
    }
    if (root->keyCount < MAXKEYS) //  Pagina nao esta cheia
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
*/