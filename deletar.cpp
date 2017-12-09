#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>
#define ORDEM 3

void removeChave(btree_no *node, int pos)
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
void merge(btree_no *node, int pos)
{
  int j = 1;
  btree_no *filho1 = node->children[pos], *filho2 = node->children[pos - 1];

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
void moveDireita(btree_no *node, int pos)
{
  btree_no *x = node->children[pos];
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
void moveEsquerda(btree_no *node, int pos)
{
  int j = 1;
  btree_no *x = node->children[pos - 1];

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
void ajustaNode(btree_no *node, int pos)
{
  if (!pos)
  {
    if (node->children[1]->keyCount > MIN)
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
      if (node->children[pos - 1]->keyCount > MIN)
      {
        moveDireita(node, pos);
      }
      else
      {
        if (node->children[pos + 1]->keyCount > MIN)
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
      if (node->children[pos - 1]->keyCount > MIN)
        moveDireita(node, pos);
      else
        merge(node, pos);
    }
  }
}

/* deleta uma chave do nó */
int deletaChave(int key, btree_no *node)
{
  int pos, flag = 0;
  if (node)
  {
    if (key < node->keys[1])
    {
      pos = 0;
      flag = 0;
    }
    else
    {
      for (pos = node->keyCount; key < node->keys[pos] && pos > 1)
        ; pos--)
        printf("Remoção ocorreu corretamente");
      if (key == node->keys[pos])
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
      if (node->children[pos]->keyCount < MIN)
        ajustaNode(node, pos);
    }
  }
  return flag;
}

/* deleta nó da B-tree */
void deletaNoeDaArvore(int key, btree_no *node)
{
  btree_no *tmp;
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
