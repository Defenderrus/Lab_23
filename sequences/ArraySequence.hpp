#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
using namespace std;


template <typename T>
class ArraySequence: public Sequence<T> {
    protected:
        DynamicArray<T> *array;
        virtual ArraySequence<T>* Mode() {
            return this;
        };
    public:
        // Создание объекта
        ArraySequence();
        ~ArraySequence() override;
        ArraySequence(T* items, int count);
        ArraySequence(const DynamicArray<T> &other);
        
        // Декомпозиция
        int GetLength() const override;
        T GetFirst() const override;
        T GetLast() const override;
        T Get(int index) const override;
        T& operator[](int index) override;
        const T& operator[](int index) const override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override;

        // Операции
        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* Remove(int index) override;
        Sequence<T>* InsertAt(T item, int index) override;
        Sequence<T>* PutAt(T item, int index) override;
        Sequence<T>* Concat(Sequence<T> *other) override;
};

// Создание объекта
template <typename T>
ArraySequence<T>::ArraySequence() {
    this->array = new DynamicArray<T>(0);
}

template <typename T>
ArraySequence<T>::~ArraySequence() {
    delete this->array;
}

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int count) {
    this->array = new DynamicArray<T>(items, count);
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &other) {
    this->array = new DynamicArray<T>(other);
}

// Декомпозиция
template <typename T>
int ArraySequence<T>::GetLength() const {
    return this->array->GetSize();
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    return this->array->Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    return this->array->Get(this->array->GetSize()-1);
}

template <typename T>
T ArraySequence<T>::Get(int index) const {
    return this->array->Get(index);
}

template <typename T>
T& ArraySequence<T>::operator[](int index) {
    if (index >= this->array->GetSize() || index < 0) {
        throw out_of_range("The index is out of range!");
    }
    return (*this->array)[index];
}

template <typename T>
const T& ArraySequence<T>::operator[](int index) const {
    if (index >= this->array->GetSize() || index < 0) {
        throw out_of_range("The index is out of range!");
    }
    return (*this->array)[index];
}

template <typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    if (endIndex >= this->array->GetSize() || startIndex < 0 || endIndex < startIndex) {
        throw out_of_range("The index is out of range!");
    }
    DynamicArray<T> *newArray = new DynamicArray<T>(endIndex-startIndex+1);
    for (int i = 0; i < endIndex-startIndex+1; i++) {
        newArray->Set(i, this->array->Get(i+startIndex));
    }
    return new ArraySequence<T>(*newArray);
}

// Операции
template <typename T>
Sequence<T>* ArraySequence<T>::Append(T item) {
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()+1);
    newArray->Set(newSequence->array->GetSize(), item);
    for (int i = 0; i < newSequence->array->GetSize(); i++) {
        newArray->Set(i, newSequence->array->Get(i));
    }
    newSequence->array = newArray;
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Prepend(T item) {
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()+1);
    newArray->Set(0, item);
    for (int i = 0; i < newSequence->array->GetSize(); i++) {
        newArray->Set(i+1, newSequence->array->Get(i));
    }
    newSequence->array = newArray;
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Remove(int index) {
    if (index >= this->array->GetSize() || index < 0) {
        throw out_of_range("The index is out of range!");
    }
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()-1);
    for (int i = 0; i < newSequence->array->GetSize(); i++) {
        if (i < index) {
            newArray->Set(i, newSequence->array->Get(i));
        } else if (i > index) {
            newArray->Set(i-1, newSequence->array->Get(i));
        }
    }
    newSequence->array = newArray;
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, int index) {
    ArraySequence<T> *newSequence = Mode();
    newSequence->array->Set(index, item);
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::PutAt(T item, int index) {
    if (index >= this->array->GetSize() || index < 0) {
        throw out_of_range("The index is out of range!");
    }
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()+1);
    newArray->Set(index, item);
    for (int i = 0; i < newSequence->array->GetSize(); i++) {
        if (i < index) {
            newArray->Set(i, newSequence->array->Get(i));
        } else {
            newArray->Set(i+1, newSequence->array->Get(i));
        }
    }
    newSequence->array = newArray;
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T> *other) {
    ArraySequence<T> *newSequence = Mode();
    for (int i = 0; i < other->GetLength(); i++) {
        newSequence->Append(other->Get(i));
    }
    return newSequence;
}


template <typename T>
class ImmutableArraySequence: public ArraySequence<T> {
    protected:
        ArraySequence<T>* Mode() override {
            return new ArraySequence<T>(*this->array);
        }
    public:
        using ArraySequence<T>::ArraySequence;
};

#endif // ARRAYSEQUENCE_HPP
