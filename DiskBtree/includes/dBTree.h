#ifndef DBTREE_H_INCLUDED
#define DBTREE_H_INCLUDED

#include<fstream>
#include <type_traits>
#include <vector>
#include "serializable.h"
#include "header.h"
#include "BtreeNode.h"


using namespace std;

const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T>
class dBTree : private fstream {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
    public:
        dBTree();
        dBTree(const string name, const string type,
                 const unsigned int version, ios::openmode openmode = mode);

        bool open(const string name, const string type,
                 const unsigned int version, ios::openmode openmode = mode);
        BTreeNode<T>  *getRoot();
        void setRoot(BTreeNode<T> node);
        unsigned long long int getFirstDeleted();
        bool isOpen();
        bool close();
        void insert(T k);
        void print();

    protected:
        bool readNode(BTreeNode<T> &r, unsigned long long int i);
        bool writeNode(BTreeNode<T> &r, unsigned long long int i);
        BTreeNode<T> *root;
        header head;
        bool readHeader(header &h);
        bool writeHeader(header &h);
        unsigned long long int index2pos(unsigned long long int i);
        unsigned long long int pos2index(unsigned long long int p);
        unsigned long long int allocateNode();
        bool splitChild(BTreeNode<T> *x, unsigned long long int ix, unsigned long long int i);
        bool insertNonFull(BTreeNode<T> *x, unsigned long long int ix, T k);
};

template <class T>
dBTree<T>::dBTree(){

}

// Observação: a raiz é sempre armazenada na posição 1 do arquivo em disco
template <class T>
dBTree<T>::dBTree(const string name, const string type,
                const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode) {
    // caso não encontre o nome de árvore especificado -> criar nova árvore
    if(!isOpen()){
        fstream::open(name.c_str(), mode | ios::trunc);
        if(isOpen()){
            cout << "arquivo criado com sucesso" << endl;
            header aux(type, version);
            head = aux;
            head.setFirstValid(1);
            this->root = new BTreeNode<T>;
            if(writeHeader(aux)){
                cout << "cabecalho gravado" << endl;
                if(writeNode(*this->root, 1)){
                    cout << "raiz gravada" << endl;
                }
            }
        }else cerr << "erro ao criar arquivo" << endl;
    }else{ // caso encontre o arquivo com o nome especificado
        cout << "arquivo aberto com sucesso" << endl;
        if(readHeader(head) && head.getType() == type && head.getVersion() == version){
            BTreeNode<T> *aux = new BTreeNode<T>;
            cout << "cabecalhos iguais. acesso permitido" << endl;
            if(readNode(*aux, 1)){
                cout << "raiz lida com sucesso" << endl;
                this->root = aux;
            }
        }else{ // caso os cabeçalhos sejam diferentes
            this->close();
            cout << "cabecalhos diferentes. acesso negado" << endl;
        }
    }
}

template <class T>
BTreeNode<T> * dBTree<T>::getRoot(){
    return this->root;
}

template <class T>
void dBTree<T>::setRoot(BTreeNode<T> node){
    this->root = node;
}

template <class T>
bool dBTree<T>::open(const string name, const string type,
                const unsigned int version, ios::openmode openmode){
    fstream::open(name.c_str(), openmode);
    if(isOpen()){
        header aux(type, version);
        if(readHeader(aux) && aux.getType() == type && aux.getVersion() == version){
            BTreeNode<T> *aux = new BTreeNode<T>(NULL);
            if(readNode(*aux, 1)){
                cout << "cabecalhos iguais. acesso permitido" << endl;
                this->root = aux;
            }
        }else{
            cout << "cabecalhos diferentes, acesso negado" << endl;
            this->close();
        }
    }else{
        fstream::open(name.c_str(), mode | ios::trunc);
        if(isOpen()){
            cout << "arquivo criado com sucesso" << endl;
            header aux(type, version);
            head = aux;
            head.setFirstValid(1);
            this->root = new BTreeNode<T>(true);
            // gravar nó raiz
            if(writeHeader(aux)){
                cout << "cabecalho gravado" << endl;
                if(writeNode(*this->root, 1)){
                    cout << "raiz gravada" << endl;
                }
            }
        }
    }
    return fstream::good();
}

template <class T>
bool dBTree<T>::isOpen() {
    return fstream::is_open();
}

template <class T>
bool dBTree<T>::close() {
    fstream::close();
    return !isOpen();
}

template <class T>
bool dBTree<T>::readNode(BTreeNode<T> &r, unsigned long long int i){
    if(isOpen()){
        fstream::seekg(index2pos(i));
        char *aux = new char[r.size()];
        fstream::read(aux, r.size());
        r.fromString(string(aux, r.size()));
        delete aux;
        return fstream::good();
    }else{
        cerr << "erro ao ler o registro" << endl;
        return false;
    }

}

template <class T>
bool dBTree<T>::writeNode(BTreeNode<T> &r, unsigned long long int i){
    if(isOpen()){
        fstream::seekp(index2pos(i), ios::beg);
        fstream::write(r.toString().c_str(), r.size());
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
unsigned long long int dBTree<T>::index2pos(unsigned long long int i) {
    BTreeNode<T> r;
    return head.size() + ((i - 1) * r.size());
}

template <class T>
unsigned long long int dBTree<T>::pos2index(unsigned long long int p) {
    BTreeNode<T> r;
    return ((p - head.size()) / r.size()) + 1;
}

template <class T>
bool dBTree<T>::readHeader(header &h){
    if(isOpen()){
        char *aux = new char[h.size()];
        fstream::seekg(0, ios::beg);
        fstream::read(aux, h.size());
        h.fromString(string(aux, h.size()));
        delete aux;
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
bool dBTree<T>::writeHeader(header &h){
    if(isOpen()){
        fstream::seekp(0, ios::beg);
        fstream::write(h.toString().c_str(), h.size());
        return fstream::good();
    }else{
        return false;
    }
}

template <class T>
unsigned long long int dBTree<T>::allocateNode(){
    if(isOpen()){
        unsigned long long int pos;
        if(head.getFirstDeleted() == 0){
            fstream::seekp(0, ios::end);
            pos = fstream::tellp();
            return pos;
        }else{
            BTreeNode<T> aux;
            pos = head.getFirstDeleted();
            if(readNode(aux, pos)){
                head.setFirstDeleted(aux.getNext());
                if(writeHeader(head)) return pos;
                else return 0;
            } else return 0;
        }
    }else return 0;
}


// realizando split do i-ésimo filho do nó encontrado na posição ix
template <class T>
bool dBTree<T>::splitChild(BTreeNode<T> *x, unsigned long long int ix, unsigned long long int i){
     BTreeNode<T> *y = new BTreeNode<T>;

    // carregando nó y do disco para a memória
    if(readNode(*y, x->getChild(i))){
        BTreeNode<T> *z = new BTreeNode<T>;


        // copiar para z as t-1 maiores chaves de y
        for(int j = 0; j < MIN; j++){
            z->insertKey(y->getKey(j + t));
        }


        // se y não for folha, é necessário copiar os filhos
        if(!y->isLeaf()){
            z->setLeaf(false);
            for(int j = 0; j < t; j++){
                z->setChild(y->getChild(t + j), j);
            }
        }

        // rearanjando os filhos de X para inserção de z como novo filho
        for(int j = x->sizeNode(); j > i; j--){
            x->setChild(x->getChild(j), j + 1);
        }

        // pegando posição para gravar z em disco
        unsigned long long posz = pos2index(allocateNode());

        // z vira novo filho de x
        x->setChild(posz, i + 1);

        // o registro mediano de y é inserido apropriadamente em x
        x->insertKey(y->getKey(MIN));

        y->resizeNode(MIN);
        z->resizeNode(MIN);

        return writeNode(*x, ix) && writeNode(*y, x->getChild(i)) && writeNode(*z, posz); // GRAVAR X, Y e Z em disco


    }else return false;

}




// inserir no nó não cheio x localizado em disco na posição ix ou em seu descendente apropriado a chave k
template <class T>
bool dBTree<T>::insertNonFull(BTreeNode<T> *x, unsigned long long int ix, T k){
    int i = x->sizeNode() - 1;
    BTreeNode<T> *y = new BTreeNode<T>;

    // se o nó x for folha -> inserir k na posição apropriada
    if(x->isLeaf()){
        x->insertKey(k);
        return writeNode(*x, ix);
    }else{ // se x não é folha -> buscar o filho apropriado
        while(i >= 0 && k < x->getKey(i)){ // encontrando índice do filho apropriado
            i--;
        }
        i++; // posição do filho apropriado em children

        if(readNode(*y, x->getChild(i))){ // lendo do disco para a memória o filho apropriado
            if(y->sizeNode() == MAX){ // caso o devido filho esteja cheio
                this->splitChild(x, ix, i); // realizar split

                if(k > x->getKey(i)){
                    i++;
                }
            }
        }
        insertNonFull(y, x->getChild(i), k);
    }
}

template <class T>
void dBTree<T>::insert(T k){
    BTreeNode<T> *r = root, *s = NULL;

    if(r->sizeNode() == MAX){ // se a raiz estiver cheia
        s = new BTreeNode<T>; // alocar novo nó para ser nova raiz
        s->setLeaf(false);
        unsigned long long int i = pos2index(allocateNode()); // alocar espaço em disco para gravar um novo nó
        root = s;

        s->setChild(i, 0); // novo filho da raiz será o novo nó alocado

        if(writeNode(*root, 1)) cout << "raiz atualizada" << endl;
        if(writeNode(*r, i)) cout << "no gravado" << endl;

        if(splitChild(s, 1, 0)) cout << "split realizado com sucesso" << endl; // realizar split no filho [0] de s (antiga raiz cheia)
        if(insertNonFull(s, 1, k)) cout << "registro " << k.getValue() << " inserido com sucesso!" << endl;
    }else{
        if(insertNonFull(r, 1, k)) cout << "registro " << k.getValue() << " inserido com sucesso!" << endl;
        else cout << "erro ao inserir" << endl;
    }
}

template <class T>
void dBTree<T>::print(){
    BTreeNode<T> y, z, x;
    cout << "        ";
    root->printNode();
    cout << endl;

    if(readNode(y, root->getChild(0))) y.printNode();
    cout << "  ";
    if(readNode(z, root->getChild(1))) z.printNode();
    cout << "  ";
    if(readNode(x, root->getChild(2))) x.printNode();
    cout << endl;

}







#endif // DBTREE_H_INCLUDED
