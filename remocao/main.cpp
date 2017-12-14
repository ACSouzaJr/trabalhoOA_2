#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include "commom.h"
#include "printer.h"

using namespace std;

void inserirRegistro(page **root, stack<int> &PED);
void removerRegistro(page **root, stack<int> &PED, int entrada);
void atualizarRegistro(page **root, stack<int> &PED);

int main(int argc, char *argv[])
{
    /*  Cria arvore*/
    page *root = NULL;
    root = criaArvore(root);

    /*  Mostra arvore*/
    //traversal(root);
    //print(root);
    printIndice(root);
    cout << endl;

    /*  Escreve no arquivo*/
    ofstream myfile("indicelista.bt", ios::trunc);
    atualizaIndice(root, myfile);
    myfile.close();

    stack<int> PED;
    int entrada;
    
    do
    {
        /*  Menu*/
        cout << "| Insira a opção desejada: " << endl;
        cout << "| 1. Inserir Registro " << endl;
        cout << "| 2. Remover Registro " << endl;
        cout << "| 3. Atualizar Registro " << endl;
        cout << "| 4. Mostrar Arvore " << endl;
        cout << "| Qualquer outro valor para encerrar \n" << endl;
        char opcao;
        cin >> opcao;

        switch (opcao)
        {
        case '1':
            inserirRegistro(&root, PED);
            break;
        case '2':
            cout << "Insira o policyID do Registro para ser removido:" << endl;
            cin >> entrada;

            removerRegistro(&root, PED, entrada);
            break;
        case '3':
            atualizarRegistro(&root, PED);
            break;             
        case '4':
            print(root);
            break;               
        default:
            return 0;
            break;
        }
        
    } while(true);


}