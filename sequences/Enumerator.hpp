#ifndef ENUMERATOR_HPP
#define ENUMERATOR_HPP


template <typename T>
class IEnumerator {
    public:
        virtual ~IEnumerator() = default;
        virtual T Current() = 0;
        virtual void Reset() = 0;
        virtual bool MoveNext() = 0;
};

template <typename T>
class IEnumerable {
    public:
        virtual ~IEnumerable() = default;
        virtual class IEnumerator<T>* GetEnumerator() = 0;
};

#endif // ENUMERATOR_HPP
