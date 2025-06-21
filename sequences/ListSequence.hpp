#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"
using namespace std;


template <typename T>
class ListSequence: public Sequence<T> {
    protected:
        LinkedList<T> *list;
        virtual ListSequence<T>* Mode() {
            return this;
        };
    public:
        // Создание объекта
        ListSequence();
        ~ListSequence() override;
        ListSequence(T* items, int count);
        ListSequence(const LinkedList<T> &other);

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

        // Дополнительные операции
        template <typename U>
        Sequence<U>* Map(function<U(T)> func);
        Sequence<T>* Where(function<bool(T)> func);
        T Reduce(function<T(T, T)> func, T start);
        template <typename U>
        Sequence<pair<T, U>>* Zip(Sequence<U> *other);
        template <typename U>
        static pair<Sequence<T>*, Sequence<U>*> Unzip(Sequence<pair<T, U>> *sequence);
};

// Создание объекта
template <typename T>
ListSequence<T>::ListSequence() {
    this->list = new LinkedList<T>();
}

template <typename T>
ListSequence<T>::~ListSequence() {
    delete this->list;
}

template <typename T>
ListSequence<T>::ListSequence(T* items, int count) {
    this->list = new LinkedList<T>(items, count);
}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T> &other) {
    this->list = new LinkedList<T>(other);
}

// Декомпозиция
template <typename T>
int ListSequence<T>::GetLength() const {
    return this->list->GetLength();
}

template <typename T>
T ListSequence<T>::GetFirst() const {
    return this->list->GetFirst();
}

template <typename T>
T ListSequence<T>::GetLast() const {
    return this->list->GetLast();
}

template <typename T>
T ListSequence<T>::Get(int index) const {
    return this->list->Get(index);
}

template <typename T>
T& ListSequence<T>::operator[](int index) {
    if (index >= this->list->GetLength() || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    return (*this->list)[index];
}

template <typename T>
const T& ListSequence<T>::operator[](int index) const {
    if (index >= this->list->GetLength() || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    return (*this->list)[index];
}

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex) const {
    ListSequence<T> *newList = new ListSequence<T>(*this->list->GetSubList(startIndex, endIndex));
    return newList;
}

// Операции
template <typename T>
Sequence<T>* ListSequence<T>::Append(T item) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->Append(item);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Prepend(T item) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->Prepend(item);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Remove(int index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->Remove(index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::InsertAt(T item, int index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->InsertAt(item, index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::PutAt(T item, int index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->PutAt(item, index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T> *other) {
    ListSequence<T> *newSequence = Mode();
    for (int i = 0; i < other->GetLength(); i++) {
        newSequence->Append(other->Get(i));
    }
    return newSequence;
}

template <typename T>
template <typename U>
Sequence<U>* ListSequence<T>::Map(function<U(T)> func) {
    ListSequence<U> *sequence = new ListSequence<U>();
    for (int i = 0; i < this->GetLength(); i++) {
        sequence->Append(func(this->Get(i)));
    }
    return sequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Where(function<bool(T)> func) {
    ListSequence<T> *sequence = new ListSequence<T>();
    for (int i = 0; i < this->GetLength(); i++) {
        if (func(this->Get(i))) sequence->Append(this->Get(i));
    }
    return sequence;
}

template <typename T>
T ListSequence<T>::Reduce(function<T(T, T)> func, T start) {
    for (int i = 0; i < this->GetLength(); i++) {
        start = func(start, this->Get(i));
    }
    return start;
}

template <typename T>
template <typename U>
Sequence<pair<T, U>>* ListSequence<T>::Zip(Sequence<U> *other) {
    ListSequence<pair<T, U>> *sequence = new ListSequence<pair<T, U>>();
    for (int i = 0; i < min(this->GetLength(), other->GetLength()); i++) {
        sequence->Append(make_pair(this->Get(i), other->Get(i)));
    }
    return sequence;
}

template <typename T>
template <typename U>
pair<Sequence<T>*, Sequence<U>*> ListSequence<T>::Unzip(Sequence<pair<T, U>> *sequence) {
    ListSequence<T> *first = new ListSequence<T>();
    ListSequence<U> *second = new ListSequence<U>();
    for (int i = 0; i < sequence->GetLength(); i++) {
        auto pair = sequence->Get(i);
        first->Append(pair.first);
        second->Append(pair.second);
    }
    return make_pair(first, second);
}


template <typename T>
class ImmutableListSequence: public ListSequence<T> {
    protected:
        ListSequence<T>* Mode() override {
            return new ListSequence<T>(*this->list);
        }
    public:
        using ListSequence<T>::ListSequence;
};

#endif // LISTSEQUENCE_HPP
