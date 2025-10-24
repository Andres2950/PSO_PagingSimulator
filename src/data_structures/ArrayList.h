#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#define DEFAULT_MAX_SIZE 1024
#include "List.h"

#include <iostream>
#include <stdexcept>

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class ArrayList : public List<E> {
private:
    E *elements;
    int max;
    int size;
    int pos;

    void expand(){
        E *temp = new E[max*2];
        for (int i = 0; i < size; i++){
            temp[i] = elements[i];
        }
        delete elements;
        elements = temp;
    }

public:
    ArrayList(int max = DEFAULT_MAX_SIZE) {
        elements = new E[max];
        this->max = max;
        pos = size = 0;
    }
    ~ArrayList(){
        delete [] elements;
    }
    void insert(E element){
        if (size == max)
            expand();
        for (int i = size; i > pos; i--){
            elements[i] = elements[i-1];
        }
        elements[pos] = element;
        size++;
    }
    void append(E element){
        if (size == max)
            expand();
        elements[size] = element;
        size++;
    }
    E remove(){
        if (size == 0)
            throw runtime_error("List is empty.");
        if (size == pos)
            throw runtime_error("No current element.");
        E result = elements[pos];
        for (int i = pos; i < size - 1; i++)
            elements[i] = elements[i+1];
        size--;
        return result;
    }
    void clear(){
        pos = size = 0;
    }
    E getElement(){
        if (size == 0)
            throw runtime_error("List is empty.");
        if (size == pos)
            throw runtime_error("No current element.");
        return elements[pos];
    }
    E& getElementRef(){
        if (size == 0)
            throw runtime_error("List is empty.");
        if (size == pos)
            throw runtime_error("No current element.");
        return elements[pos];
    }
    E* getElementPointer(){
        if (size == 0)
            throw runtime_error("List is empty.");
        if (size == pos)
            throw runtime_error("No current element.");
        return &elements[pos];
    }
    void goToStart(){
        pos = 0;
    }
    void goToEnd(){
        pos = size;
    }
    void goToPos(int pos){
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of bounds.");
        this->pos = pos;
    }
    void next(){
        if (pos < size)
            pos++;
    }
    void previous(){
        if (pos > 0)
            pos--;
    }
    bool atStart(){
        return pos == 0;
    }
    bool atEnd(){
        return pos == size;
    }
    int getPos(){
        return pos;
    }
    int getSize(){
        return size;
    }
    void print(){
        cout << "[";
        for (int i=0; i<size; i++){
            if (i==pos)
                cout << "*";
            cout << elements[i] << " ";
        }
        cout << "]";
    }
    int indexOf(E element){
        int temp = -1;
        for (int i = 0; i < size; i++){
            if (elements[i] == element)
                temp = i;
        }
        return temp;
    }
    bool contains(E element){
        return indexOf(element) != -1;
    }

};

#endif // ARRAYLIST_H
