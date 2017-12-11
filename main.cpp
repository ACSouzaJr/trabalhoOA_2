#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stack>
#include "commom.h"

using namespace std;

void inserirRegistro(page **root, stack<int> &PED);
void removerRegistro(page **root, stack<int> &PED);

int main(int argc, char *argv[])
{
    /*  Cria arvore*/
    page *root = NULL;

    root = criaArvore(root);

    traversal(root);
    cout << endl;
    /*  Escreve no arquivo*/
    ofstream myfile("indicelista.bt", ios::trunc);
    displayNode(root, myfile);
    myfile.close();

    /*  Mostra arvore*/
    //ArvoreB();
    //cout << endl;

    stack<int> PED;
    do
    {
        /*  Menu*/
        printf("Insira a opção desejada\n");
        printf("1. Inserir Registro \n");
        printf("2. Remover Registro \n");
        printf("3. Atualizar Registro \n");
        printf("Qualquer outro valor para encerrar\n\n");
        char opcao;
        cin >> opcao;

        switch (opcao)
        {
        case '1':
            inserirRegistro(&root, PED);
            break;
        case '2':
            removerRegistro(&root, PED);
            cout << PED.top();
            break;
        case '3':
            //atualizarRegistro();
            break;                
        default:
            return 0;
            break;
        }
        
    } while(true);


}