#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <new>
#define MAX 4
#define MIN 2

using namespace std;

struct btree_no
{
  int chave[MAX + 1], count;
  btree_no *link[MAX + 1];
};

btree_no *root;

/* cria um novo nó */
btree_no *createNode(int chave, btree_no *filho)
{
  btree_no *novo_no = new btree_no;
  novo_no->chave[1] = chave;
  novo_no->count = 1;
  novo_no->link[0] = root;
  novo_no->link[1] = filho;
  return novo_no;
}

/* Coloca a chave no local correto */
void insereChave(int chave, int pos, btree_no *node, btree_no *filho)
{
  int j = node->count;
  while (j > pos)
  {
    node->chave[j + 1] = node->chave[j];
    node->link[j + 1] = node->link[j];
    j--;
  }
  node->chave[j + 1] = chave;
  node->link[j + 1] = filho;
  node->count++;
}

/* faz o split com o nó */
void splitNo(int chave, int *pchave, int pos, btree_no *node, btree_no *filho, btree_no **novo_no)
{
  int meio, j;

  if (pos > MIN)
    meio = MIN + 1;
  else
    meio = MIN;

  *novo_no = new btree_no;
  j = meio + 1;
  while (j <= MAX)
  {
    (*novo_no)->chave[j - meio] = node->chave[j];
    (*novo_no)->link[j - meio] = node->link[j];
    j++;
  }
  node->count = meio;
  (*novo_no)->count = MAX - meio;

  if (pos <= MIN)
  {
    insereChave(chave, pos, node, filho);
  }
  else
  {
    insereChave(chave, pos - meio, *novo_no, filho);
  }
  *pchave = node->chave[node->count];
  (*novo_no)->link[0] = node->link[node->count];
  node->count--;
}

/* Insere a chave no nó */
int insereNo(int chave, int *pchave, btree_no *node, btree_no **filho)
{

  int pos;
  if (!node)
  {
    *pchave = chave;
    *filho = NULL;
    return 1;
  }

  if (chave < node->chave[1])
  {
    pos = 0;
  }
  else
  {
    for (pos = node->count;
         (chave < node->chave[pos] && pos > 1); pos--)
      ;
    if (chave == node->chave[pos])
    {
      cout << "Nao sao permitadas chaves iguais\n";
      return 0;
    }
  }
  if (insereNo(chave, pchave, node->link[pos], filho))
  {
    if (node->count < MAX)
    {
      insereChave(*pchave, pos, node, *filho);
    }
    else
    {
      splitNo(*pchave, pchave, pos, node, *filho, filho);
      return 1;
    }
  }
  return 0;
}

/* insere chave na B-Tree */
void insereChaveNaArvore(int chave)
{
  int flag, i;
  btree_no *filho;

  flag = insereNo(chave, &i, root, &filho);
  if (flag)
    root = createNode(i, filho);
}

/* copia o sucessor da chave para ser deletado */
void copiaSucessor(btree_no *myNode, int pos)
{
  btree_no *dummy;
  dummy = myNode->link[pos];

  for (; dummy->link[0] != NULL;)
    dummy = dummy->link[0];
  myNode->chave[pos] = dummy->chave[1];
}

/* remove a chave do nó e rearranja os valores */
void removeChave(btree_no *myNode, int pos)
{
  int i = pos + 1;
  while (i <= myNode->count)
  {
    myNode->chave[i - 1] = myNode->chave[i];
    myNode->link[i - 1] = myNode->link[i];
    i++;
  }
  myNode->count--;
}

/* move a chave do pai para o filho da direita */
void moveDireita(btree_no *myNode, int pos)
{
  btree_no *x = myNode->link[pos];
  int j = x->count;

  while (j > 0)
  {
    x->chave[j + 1] = x->chave[j];
    x->link[j + 1] = x->link[j];
  }
  x->chave[1] = myNode->chave[pos];
  x->link[1] = x->link[0];
  x->count++;

  x = myNode->link[pos - 1];
  myNode->chave[pos] = x->chave[x->count];
  myNode->link[pos] = x->link[x->count];
  x->count--;
  return;
}

/* move a chave do pai para o filho da esquerda */
void moveEsquerda(btree_no *myNode, int pos)
{
  int j = 1;
  btree_no *x = myNode->link[pos - 1];

  x->count++;
  x->chave[x->count] = myNode->chave[pos];
  x->link[x->count] = myNode->link[pos]->link[0];

  x = myNode->link[pos];
  myNode->chave[pos] = x->chave[1];
  x->link[0] = x->link[1];
  x->count--;

  while (j <= x->count)
  {
    x->chave[j] = x->chave[j + 1];
    x->link[j] = x->link[j + 1];
    j++;
  }
  return;
}

/* funde os nós */
void fundeNode(btree_no *myNode, int pos)
{
  int j = 1;
  btree_no *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

  x2->count++;
  x2->chave[x2->count] = myNode->chave[pos];
  x2->link[x2->count] = myNode->link[0];

  while (j <= x1->count)
  {
    x2->count++;
    x2->chave[x2->count] = x1->chave[j];
    x2->link[x2->count] = x1->link[j];
    j++;
  }

  j = pos;
  while (j < myNode->count)
  {
    myNode->chave[j] = myNode->chave[j + 1];
    myNode->link[j] = myNode->link[j + 1];
    j++;
  }
  myNode->count--;
  free(x1);
}

/* Ajusta as chaves dentro do nó */
void ajustaNode(btree_no *myNode, int pos)
{
  if (!pos)
  {
    if (myNode->link[1]->count > MIN)
    {
      moveEsquerda(myNode, 1);
    }
    else
    {
      fundeNode(myNode, 1);
    }
  }
  else
  {
    if (myNode->count != pos)
    {
      if (myNode->link[pos - 1]->count > MIN)
      {
        moveDireita(myNode, pos);
      }
      else
      {
        if (myNode->link[pos + 1]->count > MIN)
        {
          moveEsquerda(myNode, pos + 1);
        }
        else
        {
          fundeNode(myNode, pos);
        }
      }
    }
    else
    {
      if (myNode->link[pos - 1]->count > MIN)
        moveDireita(myNode, pos);
      else
        fundeNode(myNode, pos);
    }
  }
}

/* deleta uma chave do nó */
int deletaChave(int chave, btree_no *myNode)
{
  int pos, flag = 0;
  if (myNode)
  {
    if (chave < myNode->chave[1])
    {
      pos = 0;
      flag = 0;
    }
    else
    {
      for (pos = myNode->count;
           (chave < myNode->chave[pos] && pos > 1); pos--)
        ;
      if (chave == myNode->chave[pos])
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
      if (myNode->link[pos - 1])
      {
        copiaSucessor(myNode, pos);
        flag = deletaChave(myNode->chave[pos], myNode->link[pos]);
        if (flag == 0)
        {
          cout << "A chave a ser deletada nao pertence a arvore\n";
        }
      }
      else
      {
        removeChave(myNode, pos);
      }
    }
    else
    {
      flag = deletaChave(chave, myNode->link[pos]);
    }
    if (myNode->link[pos])
    {
      if (myNode->link[pos]->count < MIN)
        ajustaNode(myNode, pos);
    }
  }
  return flag;
}

/* deleta chave da B-tree */
void deletaChaveDaArvore(int chave, btree_no *myNode)
{
  btree_no *tmp;
  if (!deletaChave(chave, myNode))
  {
    cout << "A chave a ser deletada nao pertence a arvore\n";
    return;
  }
  else
  {
    if (myNode->count == 0)
    {
      tmp = myNode;
      myNode = myNode->link[0];
      free(tmp);
    }
  }
  root = myNode;
  return;
}

/* procura chave na B-Tree */
void procuraChaveDaArvore(int chave, int *pos, btree_no *myNode)
{
  if (!myNode)
  {
    return;
  }

  if (chave < myNode->chave[1])
  {
    *pos = 0;
  }
  else
  {
    for (*pos = myNode->count;
         (chave < myNode->chave[*pos] && *pos > 1); (*pos)--)
      ;
    if (chave == myNode->chave[*pos])
    {
      cout << "A chave foi encontrada\n";
      return;
    }
  }
  procuraChaveDaArvore(chave, pos, myNode->link[*pos]);
  return;
}


int main()
{
  int chave, opcoes;
  while (true)
  {
    cout << "1. Insercao\t2. Remocao\n";
    cout << "3. Procurar\t4. Exit\n";
    cout << "Escolha uma das opcoes: ";
    cin >> opcoes;
    cout << endl;
    switch (opcoes)
    {
    case 1:
      cout << "Escreva o valor da chave a ser inserido:";
      cin >> chave;
      insereChaveNaArvore(chave);
      break;
    case 2:
      cout << "Escolha o valor da chave a ser deletada:";
      cin >> chave;
      deletaChaveDaArvore(chave, root);
      break;
    case 3:
      cout << "Escolha o valor da chave a ser buscada:";
      cin >> chave;
      procuraChaveDaArvore(chave, &opcoes, root);
      break;
    case 4:
      exit(0);
    }
    cout << endl;
  }
}
