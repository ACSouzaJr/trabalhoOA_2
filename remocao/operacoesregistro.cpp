#include <iostream>
#include <fstream>
#include "commom.h"
#include "printer.h"
#include <stack>

using namespace std;


void inserirRegistro(page **root, stack<int> &PED, int entrada)
{
    registro data;

    /*  
        data.policyID = entrada;
        cin >> data.statecode;
        cin >> data.county;
        cin >> data.eq_site_limit;
        cin >> data.hu_site_limit;
        cin >> data.fl_site_limit;
        cin >> data.fr_site_limit;
        cin >> data.tiv_2011;
        cin >> data.tiv_2012;
        cin >> data.eq_site_deductible;
        cin >> data.hu_site_deductible;
        cin >> data.fl_site_deductible;
        cin >> data.fr_site_deductible;
        cin >> data.point_latitude;
        cin >> data.point_longitude;
        cin >> data.line;
        cin >> data.construction;
        cin >> data.point_granularity;

    */

    /*  Inserção arvore*/
    indice key;
    indice promo_key;
    page *promo_nrr;
    bool promoted, found;

    /*  Monta a chave*/
    key.chavePrimaria = entrada;
    key.nrr = NRR;
    NRR++;

    /*  Insercao*/
    cout << endl;
    cout << "Caminho percorrido:" << endl;
    promoted = insert((*root), key, &promo_nrr, &promo_key, &found);
    cout << endl;
    if (promoted)
    {
        (*root) = promote(promo_key, (*root), promo_nrr);
    }

    /*  Se a chave nao foi encontrada, insere na arvore e no arquivo*/
    if ( !found )
    {
        /*  Insere no arquivo*/
        fstream file(FILE_NAME);
        if ( PED.empty())
        {
            file.seekg(0, file.end);
            file << "\n" << entrada;
        }
        else
        {
            gotoLine(file, PED.top());
            file << entrada;
            cin.get();
            PED.pop();
        }
        file.close();

        /*  mostra arvore nova*/ 
        cout << "Arvore apos insercao:" << endl;
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
    found = search_2((*root), key, &nrr);
    if ( found )
    {
        //remoçao da arvore
        deletaNoeDaArvore(key, (*root));
        //removeChave(root, key);

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
        cout << "Arvore apos remocao" << endl;
        //traversal((*root));
        print((*root));
        cout << endl;

        return true;
    }
    else
    {
        cout << "Esse registro nao existe" << endl;
        return false;
    }
}


void atualizarRegistro(page **root, stack<int> &PED)
{
    int entrada;
    bool sucesso;

    cout << "Insira o indice do registro que deseja modificar" << endl;
    cin >> entrada;

    if ( (removerRegistro(root, PED, entrada)) )
    {
        inserirRegistro(root, PED, entrada);
    }
}