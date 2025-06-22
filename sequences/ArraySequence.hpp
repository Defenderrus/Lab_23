#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"


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
        ArraySequence(T* items, size_t count);
        ArraySequence(const DynamicArray<T> &other);

        // Декомпозиция
        size_t GetLength() const override;
        T Get(size_t index) const override;
        T GetFirst() const override;
        T GetLast() const override;

        // Перегрузка операторов
        T& operator[](size_t index) override;
        const T& operator[](size_t index) const override;

        // Операции
        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* Remove(size_t index) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* PutAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T> *other) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;

        // Дополнительные операции
        template <typename U>
        Sequence<U>* Map(std::function<U(T)> func);
        Sequence<T>* Where(std::function<bool(T)> func);
        T Reduce(std::function<T(T, T)> func, T start);
        template <typename U>
        Sequence<std::pair<T, U>>* Zip(Sequence<U> *other);
        template <typename U>
        static std::pair<Sequence<T>*, Sequence<U>*> Unzip(Sequence<std::pair<T, U>> *sequence);
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
ArraySequence<T>::ArraySequence(T* items, size_t count) {
    this->array = new DynamicArray<T>(items, count);
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &other) {
    this->array = new DynamicArray<T>(other);
}

// Декомпозиция
template <typename T>
size_t ArraySequence<T>::GetLength() const {
    return this->array->GetSize();
}

template <typename T>
T ArraySequence<T>::Get(size_t index) const {
    return this->array->Get(index);
}

template <typename T>
T ArraySequence<T>::GetFirst() const {
    return this->array->Get(0);
}

template <typename T>
T ArraySequence<T>::GetLast() const {
    return this->array->Get(this->array->GetSize()-1);
}

// Перегрузка операторов
template <typename T>
T& ArraySequence<T>::operator[](size_t index) {
    if (index >= this->array->GetSize()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return (*this->array)[index];
}

template <typename T>
const T& ArraySequence<T>::operator[](size_t index) const {
    if (index >= this->array->GetSize()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return (*this->array)[index];
}

// Операции
template <typename T>
Sequence<T>* ArraySequence<T>::Append(T item) {
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()+1);
    newArray->Set(newSequence->array->GetSize(), item);
    for (size_t i = 0; i < newSequence->array->GetSize(); i++) {
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
    for (size_t i = 0; i < newSequence->array->GetSize(); i++) {
        newArray->Set(i+1, newSequence->array->Get(i));
    }
    newSequence->array = newArray;
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Remove(size_t index) {
    if (index >= this->array->GetSize()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()-1);
    for (size_t i = 0; i < newSequence->array->GetSize(); i++) {
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
Sequence<T>* ArraySequence<T>::InsertAt(T item, size_t index) {
    ArraySequence<T> *newSequence = Mode();
    newSequence->array->Set(index, item);
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::PutAt(T item, size_t index) {
    if (index >= this->array->GetSize()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    ArraySequence<T> *newSequence = Mode();
    DynamicArray<T> *newArray = new DynamicArray<T>(newSequence->array->GetSize()+1);
    newArray->Set(index, item);
    for (size_t i = 0; i < newSequence->array->GetSize(); i++) {
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
    for (size_t i = 0; i < other->GetLength(); i++) {
        newSequence->Append(other->Get(i));
    }
    return newSequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) {
    if (endIndex >= this->array->GetSize() || endIndex < startIndex) {
        throw std::out_of_range("Некорректный индекс!");
    }
    DynamicArray<T> *newArray = new DynamicArray<T>(endIndex-startIndex+1);
    for (size_t i = 0; i < endIndex-startIndex+1; i++) {
        newArray->Set(i, this->array->Get(i+startIndex));
    }
    return new ArraySequence<T>(*newArray);
}

// Дополнительные операции
template <typename T>
template <typename U>
Sequence<U>* ArraySequence<T>::Map(std::function<U(T)> func) {
    ArraySequence<U> *sequence = new ArraySequence<U>();
    for (size_t i = 0; i < this->GetLength(); i++) {
        sequence->Append(func(this->Get(i)));
    }
    return sequence;
}

template <typename T>
Sequence<T>* ArraySequence<T>::Where(std::function<bool(T)> func) {
    ArraySequence<T> *sequence = new ArraySequence<T>();
    for (size_t i = 0; i < this->GetLength(); i++) {
        if (func(this->Get(i))) sequence->Append(this->Get(i));
    }
    return sequence;
}

template <typename T>
T ArraySequence<T>::Reduce(std::function<T(T, T)> func, T start) {
    for (size_t i = 0; i < this->GetLength(); i++) {
        start = func(start, this->Get(i));
    }
    return start;
}

template <typename T>
template <typename U>
Sequence<std::pair<T, U>>* ArraySequence<T>::Zip(Sequence<U> *other) {
    ArraySequence<std::pair<T, U>> *sequence = new ArraySequence<std::pair<T, U>>();
    for (size_t i = 0; i < std::min(this->GetLength(), other->GetLength()); i++) {
        sequence->Append(make_pair(this->Get(i), other->Get(i)));
    }
    return sequence;
}

template <typename T>
template <typename U>
std::pair<Sequence<T>*, Sequence<U>*> ArraySequence<T>::Unzip(Sequence<std::pair<T, U>> *sequence) {
    ArraySequence<T> *first = new ArraySequence<T>();
    ArraySequence<U> *second = new ArraySequence<U>();
    for (size_t i = 0; i < sequence->GetLength(); i++) {
        auto pair = sequence->Get(i);
        first->Append(pair.first);
        second->Append(pair.second);
    }
    return make_pair(first, second);
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
