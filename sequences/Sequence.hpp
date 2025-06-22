#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <functional>
#include "ICollection.hpp"
#include "Enumerator.hpp"


template <typename T>
class Sequence: public ICollection<T>, public IEnumerable<T> {
    public:
        virtual ~Sequence() = default;

        // Декомпозиция (+ ICollection)
        size_t GetLength() const override = 0;
        T Get(size_t index) const override = 0;
        virtual T GetFirst() const = 0;
        virtual T GetLast() const = 0;

        // Перегрузка операторов
        virtual T& operator[](size_t index) = 0;
        virtual const T& operator[](size_t index) const = 0;

        // Операции
        virtual Sequence<T>* Append(T item) = 0;
        virtual Sequence<T>* Prepend(T item) = 0;
        virtual Sequence<T>* Remove(size_t index) = 0;
        virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
        virtual Sequence<T>* PutAt(T item, size_t index) = 0;
        virtual Sequence<T>* Concat(Sequence<T> *other) = 0;
        virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) = 0;

        // IEnumerator + IEnumerable
        class Iterator: public IEnumerator<T> {
            private:
                const Sequence<T> *sequence;
                size_t index;
                bool mode;
            public:
                Iterator(const Sequence<T> *other) {
                    sequence = other;
                    index = 0;
                    mode = true;
                }
                T Current() override {
                    if (mode || index >= sequence->GetLength()) {
                        throw std::out_of_range("Неправильный индекс!");
                    }
                    return sequence->Get(index);
                }
                void Reset() override {
                    index = 0;
                    mode = true;
                }
                bool MoveNext() override {
                    if (mode) {
                        mode = false;
                        return sequence->GetLength() > 0;
                    }
                    index++;
                    return index < sequence->GetLength();
                }
        };
        IEnumerator<T>* GetEnumerator() override {
            return new Iterator(this);
        }
};

#endif // SEQUENCE_HPP
