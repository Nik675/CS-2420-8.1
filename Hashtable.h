#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

template<class Type>
struct Bucket {
    Type data;
    bool empty = true;
};


template<class Type>
class Hashtable;

template<class Type>
ostream& operator<< (ostream &, Hashtable<Type>&);


template<class Type>
class Hashtable
{
public:
    Hashtable();
    Hashtable(int);
    Hashtable(const Hashtable<Type>& other);
    Hashtable& operator=(const Hashtable<Type>& other);

    int getSize() const;
    int getCapacity() const;
    bool empty() const;
    void insert(const Type);
    void remove(int);
    bool contains(int) const;
    void clear();
    friend ostream& operator<< <>(ostream &, Hashtable<Type>&);

private:
    int hash(int v) const;
    int capacity;
    int size;
    unique_ptr<Bucket<Type>[]> htable; // Add this line
};


template<class Type>
Hashtable<Type>::Hashtable()
{
    size = 0;
    capacity = 17;
    htable = make_unique<Bucket<Type>[]>(capacity);
}


template<class Type>
Hashtable<Type>::Hashtable(int capacity) : capacity(capacity)
{
    size = 0;
    htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
Hashtable<Type>::Hashtable(const Hashtable<Type>& other) {
    size = other.size;
    capacity = other.capacity;
    htable = make_unique<Bucket<Type>[]>(capacity);
    for (int i = 0; i < capacity; ++i) {
        htable[i] = other.htable[i];
    }
}

template<class Type>
Hashtable<Type>& Hashtable<Type>::operator=(const Hashtable<Type>& other) {

    if (this != &other) {

        size = other.size;
        capacity = other.capacity;
        htable = make_unique<Bucket<Type>[]>(capacity);
		
        for (int i = 0; i < capacity; ++i) 
		{

            htable[i] = other.htable[i];

        }
    }
    return *this;
}

template<class Type>
int Hashtable<Type>::hash(int v) const {
    return v % capacity;
}

template<class Type>
int Hashtable<Type>::getSize() const
{
    return size;
}

template<class Type>
int Hashtable<Type>::getCapacity() const
{
    return capacity;
}


template<class Type>
bool Hashtable<Type>::empty() const
{
    if (size == 0) {
        return true;
    }
    return false;
}

template<class Type>
void Hashtable<Type>::insert(Type value) {
    if (size == capacity) {
        throw runtime_error("Hashtable is full");
    }
    int index = hash(value);
    while (!htable[index].empty) {
        index = (index + 1) % capacity;
    }
    htable[index].data = value;
    htable[index].empty = false;
    ++size;
}

template<class Type>
void Hashtable<Type>::remove(int value) {
    int index = hash(value);
    int start_index = index;
    while (!htable[index].empty) {
        if (htable[index].data == value) {
            htable[index].empty = true;
            --size;
            return;
        }
        index = (index + 1) % capacity;
        if (index == start_index) {
            break;
        }
    }
}

template<class Type>
bool Hashtable<Type>::contains(int value) const {
    int index = hash(value);
    int start_index = index;
    while (!htable[index].empty) {
        if (htable[index].data == value) {
            return true;
        }
        index = (index + 1) % capacity;
        if (index == start_index) {
            break;
        }
    }
    return false;
}

template<class Type>
void Hashtable<Type>::clear()
{
    size = 0;
    htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>

ostream& operator<< (ostream & out, Hashtable<Type>& h) 
{
    out << "\nCapacity: " << h.capacity << "\n";
    for (int i = 0; i < h.capacity; i++) {

        if (h.htable[i].empty == false) {
            out << i << ": " << h.htable[i].data << endl;
        }

    }

    return out;
}