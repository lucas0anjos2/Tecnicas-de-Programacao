#ifndef BTREENODE_H
#define BTREENODE_H

#include <type_traits>
#include <iostream>
#include <string>
#include "serializable.h"
#include "dBTree.h"

using namespace std;

const unsigned int t = 3;
const unsigned int MAX = 2 * t - 1;
const unsigned int MIN = t - 1;

template <class T>
class BTreeNode : public serializable {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
    public:
        BTreeNode();
        BTreeNode(const BTreeNode<T> &other);
        BTreeNode<T> operator=(const BTreeNode<T> &other);
        unsigned long long int sizeNode() const;
        void resizeNode(unsigned long long int n);
        T getKey(unsigned long long int i) const;
        void setKey(T key, unsigned long long int i);
        unsigned long long int getChild(unsigned long long int i) const;
        void setChild(unsigned long long int child, unsigned long long int i);
        bool isLeaf() const;
        void setLeaf(bool leaf);
        bool isDeleted() const;
        void del();
        void undel();
        void insertKey(T key);
        unsigned long long int getNext() const;
        void setNext(unsigned long long int n);
        void printNode();
        void printChildren();
        virtual string toString();
        virtual void fromString(string repr);
        virtual string toCSV() { return ""; }
        virtual void fromCSV(string repr) {}
        virtual string toJSON() { return ""; }
        virtual void fromJSON(string repr) {}
        virtual string toXML() { return ""; }
        virtual void fromXML(string repr) {}
        virtual unsigned long long int size() const;

    protected:
        unsigned long long int n;
        bool leaf;
        bool deleted;
        T *keys;
        unsigned long long int *children;
        unsigned long long int next;

};

template <class T>
BTreeNode<T>::BTreeNode() : serializable() {
    this->next = 0;
    this->n = 0;
    this->leaf = true;
    this->deleted = false;
    this->keys = new T[MAX];

    for(int i = 0; i < MAX; i++){
        keys[i] = T(0);
    }

    this->children = new unsigned long long int[MAX + 1];

    for(unsigned long long int i = 0; i < MAX + 1; i++){
        children[i] = 0;
    }
}


template <class T>
BTreeNode<T>::BTreeNode(const BTreeNode<T> &other){
    this->n = other.sizeNode();
    this->leaf = other.isLeaf();
    this->deleted = other.isDeleted();
    this->keys = new T[MAX];
    for(int i = 0; i < other.sizeNode(); i++){
        this->keys[i] = other.getKey(i);
    }
    this->children = new unsigned long long int[MAX + 1];
    for(int i = 0; i < MAX + 1; i++){
        this->children[i] = other.getChild(i);
    }
}

template <class T>
BTreeNode<T> BTreeNode<T>::operator=(const BTreeNode<T> &other) {
    BTreeNode<T> aux(other);
    if(this == &other)
        return *this;

    this->t = other.getDegree();
    this->n = other.getNkeys();
    this->leaf = other.isLeaf();
    this->deleted = other.isDeleted();
    this->keys = new T[2 * other.getDegree() - 1];
    for(int i = 0; i < other.getNkeys(); i++){
        this->keys[i] = other.getKey(i);
    }
    this->children = new unsigned long long int[2 * other.getDegree()];
    for(int i = 0; i < 2 * other.getDegree(); i++){
        this->children[i] = other.getChild(i);
    }

    return aux;
}


template <class T>
unsigned long long int BTreeNode<T>::sizeNode() const {
    return this->n;
}

template <class T>
void BTreeNode<T>::resizeNode(unsigned long long int n){
    this->n = n;
}

template <class T>
bool BTreeNode<T>::isLeaf() const {
    return this->leaf;
}

template <class T>
T BTreeNode<T>::getKey(unsigned long long int i) const {
    return this->keys[i];
}

template <class T>
void BTreeNode<T>::setKey(T key, unsigned long long int i){
    this->keys[i] = key;
}

template <class T>
unsigned long long int BTreeNode<T>::getChild(unsigned long long int i) const {
    return this->children[i];
}

// colocar o nó da posição child como i-ésimo filho
template <class T>
void BTreeNode<T>::setChild(unsigned long long int child, unsigned long long int i){
    this->children[i] = child;
}

template <class T>
void BTreeNode<T>::setLeaf(bool leaf){
    this->leaf = leaf;
}

template <class T>
bool BTreeNode<T>::isDeleted() const {
    return this->deleted;
}

template <class T>
void BTreeNode<T>::del(){
    this->deleted = true;
}

template <class T>
void BTreeNode<T>::undel(){
    this->deleted = false;
}

// insere a chave key na posição apropriada dos filhos
template <class T>
void BTreeNode<T>::insertKey(T key){
    if(this->n < 2 * t - 1){
        int i = this->n - 1;
        while(i >= 0 && key < keys[i]){
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = key;
        this->n++;

    }else{
        cout << "no cheio" << endl;
    }

}

// usado apra quando o nó entrar na lista de expluídos
template <class T>
unsigned long long int BTreeNode<T>::getNext() const {
    return this->next;
}

template <class T>
void BTreeNode<T>::setNext(unsigned long long int n){
    this->next = n;
}

// mostrar todas as chaves do nó
template <class T>
void BTreeNode<T>::printNode(){
    string aux = "[";
    if(this->n == 0) cout << "--vazio--" << endl;
    else{
        for(int i = 0; i < this->n; i++){
            if(i == this->n - 1) aux += to_string(this->keys[i].getValue()) + "]";
            else aux+= to_string(this->keys[i].getValue()) + "|";
        }
    }
    cout << aux;
}

template <class T>
void BTreeNode<T>::printChildren(){
    cout << "[";
    for(unsigned long long int i = 0; i < MAX + 1; i++ ){
        cout << getChild(i) << ", ";
    }
    cout << "]" << endl;
}

// Métodos para serialização e deserialização do nó

template <class T>
string BTreeNode<T>::toString(){
    string aux = "";
    //aux += string(reinterpret_cast<char*>(&t), sizeof(t));
    aux += string(reinterpret_cast<char*>(&n), sizeof(n));
    aux += string(reinterpret_cast<char*>(&next), sizeof(next));
    aux += string(reinterpret_cast<char*>(&leaf), sizeof(leaf));
    aux += string(reinterpret_cast<char*>(&deleted), sizeof(deleted));
    T key;
    for(int i = 0; i < 2 * t - 1; i++){
        key = keys[i];
        aux += keys[i].toString();
    }
    for(int i = 0; i < 2 * t; i++){

        aux += string(reinterpret_cast<char*>(&children[i]), this->size());
    }
    return aux;
}

template <class T>
void BTreeNode<T>::fromString(string repr){
    int pos = 0;
//    repr.copy(reinterpret_cast<char*>(&t), sizeof(t), pos);
//    pos += sizeof(t);
    repr.copy(reinterpret_cast<char*>(&n), sizeof(n), pos);
    pos += sizeof(n);
    repr.copy(reinterpret_cast<char*>(&next), sizeof(next), pos);
    pos += sizeof(next);
    repr.copy(reinterpret_cast<char*>(&leaf), sizeof(leaf), pos);
    pos += sizeof(leaf);
    repr.copy(reinterpret_cast<char*>(&deleted), sizeof(deleted), pos);
    pos += sizeof(deleted);

    if(this->keys == NULL) this->keys = new T[2 * t - 1];

    for(int i = 0; i < 2 * t - 1; i++){
        keys[i].fromString(string(repr, pos, keys[i].size()));
        pos += keys[i].size();
    }

    if(this->children == NULL) this->children = new unsigned long long int[2 * t];

    for(int i = 0; i < 2 * t ; i++){
        repr.copy(reinterpret_cast<char*>(&children[i]), sizeof(unsigned long long int), pos);
        pos += sizeof(unsigned long long int);
    }
}

template <class T>
unsigned long long int BTreeNode<T>::size() const{
    T key;
    BTreeNode<T> child;
    return sizeof(n) + sizeof(leaf) + sizeof(deleted) + ((2 * t-1) * key.size()) + (2 * t * sizeof(unsigned long long int));
}





#endif //BTREENODE_H
