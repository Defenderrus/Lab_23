#ifndef ICOLLECTION_HPP
#define ICOLLECTION_HPP


template <typename T>
class ICollection {
    public:
        virtual ~ICollection() = default;
        virtual size_t GetLength() const = 0;
        virtual T Get(size_t index) const = 0;
};

#endif // ICOLLECTION_HPP
