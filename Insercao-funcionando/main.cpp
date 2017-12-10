#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "commom.h"

using namespace std;

void inserirRegistro(page **root);

int main(int argc, char *argv[])
{
    /*  Cria arvore*/
    page *root = NULL;

    root = criaArvore(root);

    traversal(root);
    cout << endl;

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
            inserirRegistro(&root);
            break;
        case '2':
            //removerRegistro();
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