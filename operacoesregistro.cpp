#include <iostream>
#include <fstream>
#include "commom.h"

using namespace std;


void inserirRegistro(page **root){

    int indice;

    cout << "Insira o indice do novo Registro:" << endl;
    cin >> indice;
    

    /*  Inserção arvore*/
    index key;
    index promo_key;
    page *promo_nrr;
    bool promoted, found;

    /*  Monta o indice*/
    key.chavePrimaria = indice;
    key.nrr = NRR;
    NRR++;

    promoted = insert((*root), key, &promo_nrr, &promo_key, &found);
    if (promoted)
    {
        (*root) = promote(promo_key, (*root), promo_nrr);
    }

    /*  Se a chave nao foi encontrada, insere na arvore e no arquivo*/
    if ( !found )
    {
        /*  Insere no arquivo*/
        ofstream myfile(FILE_NAME, ios::app);
        myfile << "\n" << indice;
        myfile.close();

        /*  Reecreve arvore*/ //mecanismo de escrita enquanto ocorre a inserção?
        cout << "Arvore apos insercao" << endl;
        traversal((*root));
    }



}


void removerRegistro(page **root){

    int indice;

    cout << "Insira o indice do Registro para ser removido:" << endl;
    cin >> indice;


    //realizar remoção.
    bool found;
    int nrr;
    index key;
    key.chavePrimaria = indice;
    found = search_2((*root), key, &nrr);
    if ( found )
    {
        //remoçao da arvore
        //removeChave(root, key);

        //remoção dos registros

        //reescrita dos registros

        //mostrar arvore
        cout << "Arvore apos remocao" << endl;
        traversal((*root));
    }
    else
    {
        cout << "Esse registro nao existe" << endl;
    }
}