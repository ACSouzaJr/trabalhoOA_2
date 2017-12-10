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
        ofstream myfile("FL_insurance_sample.csv", ios::app);
        myfile << "\n" << indice;
        myfile.close();
    }


    /*  Reecreve arvore*/ //mecanismo de escrita enquanto ocorre a inserção

}


void removerRegistro(page **root){

    int indice;

    cout << "Insira o indice do Registro para ser removido:" << endl;
    cin >> indice;


    //realizar remoção.



}