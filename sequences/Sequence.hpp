#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <utility>
#include <functional>
using namespace std;


template <typename T>
class Sequence {
    public:
        virtual ~Sequence() = default;
        // Декомпозиция
        virtual int GetLength() const = 0;
        virtual T GetFirst() const = 0;
        virtual T GetLast() const = 0;
        virtual T Get(int index) const = 0;
        virtual T& operator[](int index) = 0;
        virtual const T& operator[](int index) const = 0;
        virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
        // Операции
        virtual Sequence<T>* Append(T item) = 0;
        virtual Sequence<T>* Prepend(T item) = 0;
        virtual Sequence<T>* Remove(int index) = 0;
        virtual Sequence<T>* InsertAt(T item, int index) = 0;
        virtual Sequence<T>* PutAt(T item, int index) = 0;
        virtual Sequence<T>* Concat(Sequence<T> *other) = 0;
};

#endif // SEQUENCE_HPP
