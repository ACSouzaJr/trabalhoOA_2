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

void inserirRegistro(page **root, stack<int> &PED, int entrada);
void removerRegistro(page **root, stack<int> &PED, int entrada);
void atualizarRegistro(page **root, stack<int> &PED);

int main(int argc, char *argv[])
{
    /*  Cria arvore*/
    page *root = NULL;
    root = criaArvore(root);

    /*  Mostra arvore*/
    //traversal(root);
    print(root);
    cout << endl;

    /*  Escreve no arquivo*/
    ofstream myfile("indicelista.bt", ios::trunc);
    displayNode(root, myfile);
    myfile.close();

    stack<int> PED;
    int entrada;
    
    do
    {
        /*  Menu*/
        cout << "Insira a opção desejada" << endl;
        cout << "1. Inserir Registro " << endl;
        cout << "2. Remover Registro " << endl;
        cout << "3. Atualizar Registro " << endl;
        cout << "Qualquer outro valor para encerrar \n" << endl;
        char opcao;
        cin >> opcao;

        switch (opcao)
        {
        case '1':
            cout << "Insira o indice do novo Registro:" << endl;
            cin >> entrada;

            inserirRegistro(&root, PED, entrada);
            break;
        case '2':
            cout << "Insira o indice do Registro para ser removido:" << endl;
            cin >> entrada;

            removerRegistro(&root, PED, entrada);
            cout << PED.top();
            break;
        case '3':
            atualizarRegistro(&root, PED);
            break;                
        default:
            return 0;
            break;
        }
        
    } while(true);


}