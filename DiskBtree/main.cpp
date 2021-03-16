/*******************************************************
* Trabalho de Técnicas de Programação:                 *
* Implementação de uma Árvore B em disco               *
*                                                      *
* Integrantes do Grupo:                                *
*     1 - Lucas dos Anjos de Castro - RA: 0026970      *
*     2 - Hudson Teles Camilo - RA: 0011657            *
*     3 - Railson Martins da Mata - RA: 0022030        *
*                                                      *
*                                                      *
********************************************************/

#include <iostream>
#include "includes/dBTree.h"
#include "includes/intserial.h"


using namespace std;

int main(){

    dBTree<intSerial> t("arvore.dat", "TPG", 1);
    int opcao, k;
    intSerial insercao[] = {6, 5, 9, 7, 11, 1, 2, 3, 4, 8, 10};
    bool sair = false;

    do{
        cout << "\n\Testando Arvore B em disco\n" << endl;
        cout << "1. Inserir um numero inteiro" << endl;
        cout << "2. Remover um numero inteiro" << endl;
        cout << "3. Buscar um numero" << endl;
        cout << "4. Mostrar a arvore" << endl;
        cout << "5. Rodar caso de teste" << endl;
        cout << "6. Sair" << endl;
        cout << "\n> ";
        cin >> opcao;

        switch (opcao) {
             case 1:
                cout << "Digite um valor inteiro para inserir: ";
                cin >> k;
                t.insert(intSerial(k));
                break;
             case 2:
                cout << "Operacao nao implementada :(";
                break;
             case 3:
                cout << "Operacao nao implementada :(";
                break;
             case 4:
                t.print();
                break;
             case 5:
                cout << "Casos teste de Insercao" << endl;
                for (intSerial i : insercao) {
                   cout << "\nInserindo a chave " << i.getValue() << ":" << endl;
                   t.insert(i);
                }
                t.print();

                break;
             case 6:
                sair = true;
                break;
             default:
                cout << "Opcao invalida, por vavor tente novamente.\n" << endl;
                break;
          }


    }while(!sair);




    return 0;
}
