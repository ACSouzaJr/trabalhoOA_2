#include <stdio.h>
#include <stdlib.h>
#include "commom.h"

int main(int argc, char *argv[])
{
    /*  Cria arvore*/
    page *root = NULL;

    criaArvore(root);


    do
    {
        /*  Menu*/
        printf("Insira a opção desejada\n");
        printf("1. Inserir indice \n");
        printf("2. Remover indice \n");
        printf("3. Atualizar indice \n");
        printf("Qualquer outro valor para encerrar\n\n");
        char opcao = getchar();

        switch (opcao)
        {
        case '1':
            //inserirRegistro();
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