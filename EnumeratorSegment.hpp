#ifndef ENUMERATORSEGMENT_HPP
#define ENUMERATORSEGMENT_HPP


template <typename T>
class IEnumeratorSegment {
    public:
        virtual ~IEnumeratorSegment() = default;
        virtual T Current() = 0;
        virtual void Reset() = 0;
        virtual bool MoveNext() = 0;
};

template <typename T>
class IEnumerableSegment {
    public:
        virtual ~IEnumerableSegment() = default;
        virtual class IEnumeratorSegment<T>* GetEnumerator() = 0;
};

#endif // ENUMERATORSEGMENT_HPP
