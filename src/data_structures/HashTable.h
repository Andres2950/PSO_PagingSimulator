#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <math.h>
#include "KVPair.h"
#include "Dictionary.h"
#include "ArrayList.h"

using std::runtime_error;
using std::cout;
using std::endl;
using std::string;

/* Implementación de una tabla Hash, posee la función
hash y unos busckets de los elementos que contiene, realiza
la generación de código hash mediante función polinomial,
realiza la compresión mediante multiplicación, suma y división*/

/* mantiene una carga minima de 0.2 y una máxima de 0.6 */

/* Posee los siguientes métodos básicos:
insert, remove, getValue, setValue, clear, contains,
getKeys, getValues, getSizee y print.

Sus métodos especiales son:
reHashUp, reHashDown, reHash, loadFactor, checkNotExisting,
checkExisting, función hash(h), compress y hashCodePolynomial */

template <typename K, typename V>
class HashTable : public Dictionary<K, V> {
private:
    ArrayList<KVPair<K, V>> *buckets;
    int max;
    int size;
    double maxLoad;
    double minLoad;
    ArrayList<int> *primes; // lista de primos para el tamaño del arreglo

    // carga números primos para ser usados como capacidad
    // máxima de la tabla. Inicia en 32537.
    void initPrimes() {
        primes = new ArrayList<int>();
        primes->append(2039);
        primes->append(4073);
        primes->append(8147);
        primes->append(16273);
        primes->append(32537);
        primes->append(65071);
        primes->append(130147);
        primes->goToPos(4);
    }

    double loadFactor() {
        return (double)size / (double)max;
    }

    // redimensiona la tabla para hacerla más grande
    void reHashUp() {
        primes->next();
        int newMax = primes->getElement();
        reHash(newMax);
    }

    // redimensiona la tabla para hacerla más pequeña
    void reHashDown() {
        primes->previous();
        int newMax = primes->getElement();
        if (newMax != max)
            reHash(newMax);
    }

    // redimensiona la tabla al tamaño indicado
    void reHash(int newMax) {
        int oldMax = max;
        max = newMax;
        //cout << "Rehashing" << newMax << endl;

        ArrayList<KVPair<K, V>> *newBuckets = new ArrayList<KVPair<K, V>>[max];
        for (int i = 0; i < oldMax; i++) {
            buckets[i].goToStart();
            while (!buckets[i].getSize() == 0) {
                KVPair<K, V> p = buckets[i].remove();
                newBuckets[h(p.key)].append(p);
            }
        }
        delete [] buckets;
        buckets = newBuckets;
    }

    // revisa que una llave no exista en la estructura
    // si la encuentra lanza error
    // si no la encuentra la posición actual queda al final
    void checkNotExisting(K key) {
        KVPair<K, V> p(key);
        if (buckets[h(key)].contains(p))
            throw runtime_error("Duplicated key.");
    }

    // revisa que una llave exista en la estructura
    // si la encuentra deja la posición actual apuntando
    // a la llave buscada
    // si no la encuentra lanza error
    void checkExisting(K key) {
        KVPair<K, V> p(key);
        if (!buckets[h(key)].contains(p))
            throw runtime_error("Key not found.");
    }
    //Funcion hash completa
    int h(K key) {
        return compress(hashCodePolynomial(key));
    }
    //Funcion de compresion
    int compress(int code) {
        int a = 1097;
        int b = 1279;
        return abs(a * code + b) % max;
    }

    //Para crer el indice
    template <typename T>
    int hashCodePolynomial(T key) {
        int a = 33;
        int result = 0;
        char* bytes = reinterpret_cast<char*>(&key);
        for (unsigned int i = 0; i < sizeof(K); i++) {
            result += bytes[i] * pow(a, i);
        }
        return result;
    }
    int hashCodePolynomial(string key) {
        int a = 33;
        int result = 0;
        for (unsigned int i = 0; i < key.size(); i++) {
            result += (int)key[i] * pow(a, i);
        }
        return result;
    }

public:
    HashTable() {
        initPrimes();
        max = primes->getElement();
        buckets = new ArrayList<KVPair<K, V>>[max];
        size = 0;
        maxLoad = 0.6;
        minLoad = 0.2;
    }
    ~HashTable() {
        delete [] buckets;
        delete primes;
    }
    //Para insertar valida y luego agrega el valor en un indice dado por la llave
    void insert(K key, V value) {
        if (loadFactor() > maxLoad)
            reHashUp();
        checkNotExisting(key);
        KVPair<K, V> p(key, value);
        buckets[h(key)].append(p);
        size++;
    }
    //Para remover busca la llave y los elimina
    V remove(K key) {
        if (loadFactor() <= minLoad)
            reHashDown();
        checkExisting(key);
        KVPair<K, V> p = buckets[h(key)].remove();
        size--;
        return p.value;
    }
    V getValue(K key) {
        checkExisting(key);
        ArrayList<KVPair<K, V>>& bucket = buckets[h(key)];
        bucket.goToStart();
        KVPair<K, V> p = bucket.getElement();
        return p.value;
    }
    //Busca la llave y asigna un nuevo valor
    void setValue(K key, V value) {
        checkExisting(key);
        KVPair<K, V> *p = buckets[h(key)].getElementPointer();
        p->value = value;
    }
    //usa el contains de la lista
    bool contains(K key) {
        KVPair<K, V> p(key);
        return buckets[h(key)].contains(key);
    }
    //Limpia
    void clear() {
        for (int i = 0; i < max; i++) {
            buckets[i].clear();
        }
        size = 0;
    }
    //Mete cada llave en una lista y la retorna
    List<K>* getKeys() {
        List<K> *keys = new ArrayList<K>();
        for (int i = 0; i < max; i++) {
            buckets[i].goToStart();
            while (!buckets[i].atEnd()) {
                KVPair<K, V> p = buckets[i].getElement();
                keys->append(p.key);
                buckets[i].next();
            }
        }
        return keys;
    }
    //Mete cada valor en una lista y los retorna
    List<V>* getValues() {
        List<V> *values = new ArrayList<V>();
        for (int i = 0; i < max; i++) {
            buckets[i].goToStart();
            while (!buckets[i].atEnd()) {
                KVPair<K, V> p = buckets[i].getElement();
                values->append(p.value);
                buckets[i].next();
            }
        }
        return values;
    }
    int getSize() {
        return size;
    }
    void print() {
        cout << "[";
        for (int i = 0; i < max; i++) {
            buckets[i].goToStart();
            while (!buckets[i].atEnd()) {
                cout << buckets[i].getElement() << " ";
                buckets[i].next();
            }
            buckets[i].goToStart();
        }
        cout << "]" << endl;
    }
};

#endif // HASHTABLE_H
