#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"


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
        ListSequence(T* items, size_t count);
        ListSequence(const LinkedList<T> &other);

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
ListSequence<T>::ListSequence() {
    this->list = new LinkedList<T>();
}

template <typename T>
ListSequence<T>::~ListSequence() {
    delete this->list;
}

template <typename T>
ListSequence<T>::ListSequence(T* items, size_t count) {
    this->list = new LinkedList<T>(items, count);
}

template <typename T>
ListSequence<T>::ListSequence(const LinkedList<T> &other) {
    this->list = new LinkedList<T>(other);
}

// Декомпозиция
template <typename T>
size_t ListSequence<T>::GetLength() const {
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
T ListSequence<T>::Get(size_t index) const {
    return this->list->Get(index);
}

// Перегрузка операторов
template <typename T>
T& ListSequence<T>::operator[](size_t index) {
    if (index >= this->list->GetLength()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return (*this->list)[index];
}

template <typename T>
const T& ListSequence<T>::operator[](size_t index) const {
    if (index >= this->list->GetLength()) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return (*this->list)[index];
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
Sequence<T>* ListSequence<T>::Remove(size_t index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->Remove(index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::InsertAt(T item, size_t index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->InsertAt(item, index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::PutAt(T item, size_t index) {
    ListSequence<T> *newSequence = Mode();
    newSequence->list->PutAt(item, index);
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T> *other) {
    ListSequence<T> *newSequence = Mode();
    for (size_t i = 0; i < other->GetLength(); i++) {
        newSequence->Append(other->Get(i));
    }
    return newSequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) {
    ListSequence<T> *newList = new ListSequence<T>(*this->list->GetSubList(startIndex, endIndex));
    return newList;
}

// Дополнительные операции
template <typename T>
template <typename U>
Sequence<U>* ListSequence<T>::Map(std::function<U(T)> func) {
    ListSequence<U> *sequence = new ListSequence<U>();
    for (size_t i = 0; i < this->GetLength(); i++) {
        sequence->Append(func(this->Get(i)));
    }
    return sequence;
}

template <typename T>
Sequence<T>* ListSequence<T>::Where(std::function<bool(T)> func) {
    ListSequence<T> *sequence = new ListSequence<T>();
    for (size_t i = 0; i < this->GetLength(); i++) {
        if (func(this->Get(i))) sequence->Append(this->Get(i));
    }
    return sequence;
}

template <typename T>
T ListSequence<T>::Reduce(std::function<T(T, T)> func, T start) {
    for (size_t i = 0; i < this->GetLength(); i++) {
        start = func(start, this->Get(i));
    }
    return start;
}

template <typename T>
template <typename U>
Sequence<std::pair<T, U>>* ListSequence<T>::Zip(Sequence<U> *other) {
    ListSequence<std::pair<T, U>> *sequence = new ListSequence<std::pair<T, U>>();
    for (size_t i = 0; i < std::min(this->GetLength(), other->GetLength()); i++) {
        sequence->Append(make_pair(this->Get(i), other->Get(i)));
    }
    return sequence;
}

template <typename T>
template <typename U>
std::pair<Sequence<T>*, Sequence<U>*> ListSequence<T>::Unzip(Sequence<std::pair<T, U>> *sequence) {
    ListSequence<T> *first = new ListSequence<T>();
    ListSequence<U> *second = new ListSequence<U>();
    for (size_t i = 0; i < sequence->GetLength(); i++) {
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
