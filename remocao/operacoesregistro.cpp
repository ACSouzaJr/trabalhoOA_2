#include <iostream>
#include <fstream>
#include "commom.h"
#include "printer.h"
#include <stack>

using namespace std;

void inserirRegistro(page **root, stack<int> &PED)
{
    registro data;
    int entrada;

    cout << "Insira o indice do novo Registro: " << endl;
    cin >> entrada;

    data.policyID = entrada;
    //getRegistro(&data);

    /*  Inserção arvore*/
    indice key;
    indice promo_key;
    page *promo_nrr;
    bool promoted, found;

    /*  Monta a chave*/
    key.chavePrimaria = entrada;
    key.nrr = NRR;
    NRR++;

    /*  Mostra caminho*/
    int nrr;
    cout << endl;
    cout << "Caminho percorrido: " << endl;
    search_2((*root), key, &nrr);

    /*  Insercao*/
    promoted = insert((*root), key, &promo_nrr, &promo_key, &found);
    cout << endl;
    if (promoted)
    {
        (*root) = promote(promo_key, (*root), promo_nrr);
    }

    /*  Se a chave nao foi encontrada, insere na arvore e no arquivo*/
    if (!found)
    {

        /*  Insere no arquivo*/
        fstream file(FILE_NAME);
        if (PED.empty())
        {
            file.seekg(0, file.end);
            file << "\n"
                 << entrada;
            //writeRegistro(file, data);
        }
        else
        {
            gotoLine(file, PED.top());
            file << entrada;
            //writeRegistro(file, data);
            cin.get();
            PED.pop();
        }
        file.close();

        /*  mostra arvore nova*/
        cout << "Arvore apos insercao: " << endl;
        //traversal((*root));
        print((*root));
        cout << endl;

        /*  Reecreve arvore*/
        ofstream in("indicelista.bt", ios::trunc);
        displayNode((*root), in);
        in.close();
    }
}

bool removerRegistro(page **root, stack<int> &PED, int entrada)
{
    //realizar remoção.
    bool found;
    int nrr;
    indice key;
    key.chavePrimaria = entrada;

    cout << endl;
    cout << "Caminho percorrido: " << endl;
    found = search_2((*root), key, &nrr);
    
    if (found)
    {
        //remoçao da arvore
        deletaNoeDaArvore(key, root);

        //remoção dos registros
        fstream myfile(FILE_NAME);

        //vai ate a linha e insere um simbolo de delecao
        gotoLine(myfile, nrr);
        myfile << '*';
        cin.get();
        myfile.close();
        //insere posicao vazia na pilha
        PED.push(nrr);

        //reescrita dos indices
        ofstream in("indicelista.bt", ios::trunc);
        displayNode((*root), in);
        in.close();

        //mostrar arvore
        cout << endl;
        cout << "Arvore apos remocao: " << endl;
        //traversal((*root));
        print((*root));
        cout << endl;

        return true;
    }
    else
    {
        cout << "Esse registro nao existe. \n" << endl;
        return false;
    }
}

void atualizarRegistro(page **root, stack<int> &PED)
{
    int entrada, nrr;
    char opcao;
    indice key;
    registro reg;

    cout << "Insira o indice do registro que deseja modificar." << endl;
    cin >> entrada;

    cout << "Deseja modificar a chave primario(s/n): ";
    cin >> opcao;

    if (opcao == 's')
    {
        if ((removerRegistro(root, PED, entrada)))
        {
            inserirRegistro(root, PED);
        }
    }
    else
    {
        reg.policyID = entrada;
        getRegistro(&reg);

        fstream file(FILE_NAME);
        key.chavePrimaria = entrada;
        search_2((*root), key, &nrr);
        gotoLine(file, nrr);
        writeRegistro(file, reg);
    }
}