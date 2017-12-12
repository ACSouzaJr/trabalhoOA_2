#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>

using namespace std;

bool search_2(page *root, index key, int *nrr)
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
        std::cout << "Essa chave ja existe \n" << '\n';
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

void removeChave(page *node, int pos)
{
    int i = pos + 1;
    while (i <= node->keyCount)
    {
        node->keys[i - 1] = node->keys[i];
        node->children[i - 1] = node->children[i];
        i++;
    }
    node->keyCount--;
    // diminui o número de chaves na página
}

/* funde os nós */
void merge(page *node, int pos)
{
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
}

/* move a chave do pai para o filho da direita */
void moveDireita(page *node, int pos)
{
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
}

/* move a chave do pai para o filho da esquerda */
void moveEsquerda(page *node, int pos)
{
    int j = 1;
    page *x = node->children[pos - 1];

    x->keyCount++;
    x->keys[x->keyCount] = node->keys[pos];
    x->children[x->keyCount] = node->children[pos]->children[0];

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
}

/* Ajusta as chaves dentro do nó */
void ajustaNode(page *node, int pos)
{
    if (!pos)
    {
        if (node->children[1]->keyCount > MINKEYS)
        {
            moveEsquerda(node, 1);
        }
        else
        {
            merge(node, 1);
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
                    moveEsquerda(node, pos + 1);
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
    page *dummy;
    dummy = node->children[pos];

    for (; dummy->children[0] != NULL;)
        dummy = dummy->children[0];
    node->keys[pos] = dummy->keys[1];
}

/* deleta uma chave do nó */
int deletaChave(index key, page *node)
{
    int pos, flag = 0;
    if (node)
    {
        if (key.chavePrimaria < node->keys[1].chavePrimaria)
        {
            pos = 0;
            flag = 0;
        }
        else
        {
            for (pos = node->keyCount; key.chavePrimaria < node->keys[pos].chavePrimaria && pos > 1; pos--)
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
            }
        }
        if (flag)
        {
            if (node->children[pos - 1])
            {
                copiaSucessor(node, pos);
                flag = deletaChave(node->keys[pos], node->children[pos]);
                if (flag == 0)
                {
                    cout << "A chave a ser deletada nao pertence a arvore\n";
                }
            }
            else
            {
                removeChave(node, pos);
            }
        }
        else
        {
            flag = deletaChave(key, node->children[pos]);
        }
        if (node->children[pos])
        {
            if (node->children[pos]->keyCount < MINKEYS)
                ajustaNode(node, pos);
        }
    }
    return flag;
}

/* deleta nó da B-tree */
void deletaNoeDaArvore(index key, page *node, page *root)
{
    page *tmp;
    if (!deletaChave(key, node))
    {
        printf("A chave a ser deletada nao pertence a arvore\n");
        return;
    }
    else
    {
        if (node->keyCount == 0)
        {
            tmp = node;
            node = node->children[0];
            free(tmp);
        }
    }
    root = node;
    return;
}

/****************************************

1- Se a chave a ser removida não for folha, a substitua com seu sucessor direto em uma folha
2- Delete a chave
3- Se a folha ainda possuir a quantidade mínima de folhas, a remoção está correta
4- Se não possuir a quantidade mínima, olhe para seus irmãos 
a) Se algum dos irmãos tiver mais que o mínimo de chaves, faça uma redistribuição 
b) Senão, faça um merge
5- Se as olhas forem concatenadas, aplique os passos 3-6 também para seu pai
6- Se a última folha da raiz for removida, o tamanho da árvore diminui

*****************************************/

/***********************************************************************************

bool remove(page *root, int key, page **promo_nrr, int *promo_key){

  int pos;
  bool found, promoted;

  
  if (root == NULL)
  {
    *promo_key = key;
    promo_nrr = NULL;
    return true;
  }

  found = search(root, key, &pos); // Se encontrou a chave, é possível remove-la

  else // se não for uma folha
  { 
    search(root, key, &pos);
    removeChave(root, key);

    if (root->keyCount > MINKEYS){
      printf("A remoção ocorreu corretamente");
      //não é necessário fazer nada
    }

    else if (root->keyCount == MINKEYS) 
    {
      // o numero de chaves do nó a ser eliminado passa a ser o mínimo depois da remoção
      if (root->children[pos]->keyCount > MINKEYS || root->children[pos + 1]->keyCount > MINKEYS)
      {
        reditribuir();
      }
      else if (root->children[pos]->keyCount == MINKEYS && root->children[pos + 1]->keyCount == MINKEYS)
      {
        // merge entre as duas folhas e o pai delas
        merge();
        // ainda é necessário verificar se o nó pai sofreu underflow
        if (root->keyCount > MINKEYS - 1)
        {
          printf("A remoção ocorreu corretamente");
        }
        else if (root->keyCount == MINKEYS)
        {
          if (root->children[pos]->keyCount > MINKEYS || root->children[pos + 1]->keyCount > MINKEYS)
          {
            reditribuir();
          }
          else if (root->children[pos]->keyCount == MINKEYS && root->children[pos + 1]->keyCount == MINKEYS)
          {
            // merge entre os dois nós e o pai deles, que não são folhas
            merge();
          }
        }
      }
    }
  }
}

*********************************************************************************/