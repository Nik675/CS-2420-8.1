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
    Hashtable(int capacity, double loadFactorThreshold);
    Hashtable(const Hashtable<Type>& other);
    Hashtable& operator=(const Hashtable<Type>& other);

    int getSize() const;
    int getCapacity() const;
    double getLoadFactorThreshold() const;
    bool empty() const;
    void insert(const Type);
    void remove(int);
    bool contains(int) const;
    int indexOf(int value) const;
    void clear();
    static bool isPrime(int n);
    static int nextPrime(int n);
    friend ostream& operator<< <>(ostream &, Hashtable<Type>&);

private:
    int hash(int v) const;
    void resize();
    int capacity;
    int size;
    double loadFactorThreshold;
    unique_ptr<Bucket<Type>[]> htable; // Add this line
};


template<class Type>
Hashtable<Type>::Hashtable()
{
    size = 0;
    capacity = 17;
    loadFactorThreshold = 0.65; // Initialize loadFactorThreshold
    htable = make_unique<Bucket<Type>[]>(capacity);
}


template<class Type>
Hashtable<Type>::Hashtable(int capacity) : capacity(capacity) {
    size = 0;
    loadFactorThreshold = 0.65; // Initialize loadFactorThreshold
    htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
Hashtable<Type>::Hashtable(int capacity, double loadFactorThreshold) : capacity(capacity), loadFactorThreshold(loadFactorThreshold) {
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
double Hashtable<Type>::getLoadFactorThreshold() const {
    return loadFactorThreshold;
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
    if (size >= capacity * loadFactorThreshold) {
        resize();
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
            
            // Rehash elements after the removed element
            int nextIndex = (index + 1) % capacity;
            while (!htable[nextIndex].empty) {
                Type currentValue = htable[nextIndex].data;
                htable[nextIndex].empty = true;
                --size;
                
                // Find the correct position for this value
                int newIndex = hash(currentValue);
                while (!htable[newIndex].empty) {
                    newIndex = (newIndex + 1) % capacity;
                }
                htable[newIndex].data = currentValue;
                htable[newIndex].empty = false;
                ++size;
                
                nextIndex = (nextIndex + 1) % capacity;
            }
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
int Hashtable<Type>::indexOf(int value) const {
    int index = hash(value);
    int j = 1;
    while (!htable[index].empty) {
        if (htable[index].data == value) {
            return index;
        }
        index = (index + j * j) % capacity;
        ++j;
    }
    return -1;
}

template<class Type>
bool Hashtable<Type>::isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

template<class Type>
int Hashtable<Type>::nextPrime(int n) {
    while (!isPrime(n)) {
        ++n;
    }
    return n;
}

template<class Type>
void Hashtable<Type>::clear()
{
    size = 0;
    htable = make_unique<Bucket<Type>[]>(capacity);
}

template<class Type>
void Hashtable<Type>::resize() {
    int oldCapacity = capacity;
    capacity = nextPrime(2 * oldCapacity);
    unique_ptr<Bucket<Type>[]> newTable = make_unique<Bucket<Type>[]>(capacity);

    for (int i = 0; i < oldCapacity; ++i) {
        if (!htable[i].empty) {
            int index = hash(htable[i].data);
            while (!newTable[index].empty) {
                index = (index + 1) % capacity;
            }
            newTable[index].data = htable[i].data;
            newTable[index].empty = false;
        }
    }

    htable = move(newTable);
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