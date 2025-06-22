#ifndef ICOLLECTIONSEGMENT_HPP
#define ICOLLECTIONSEGMENT_HPP


template <typename T>
class ICollectionSegment {
    public:
        virtual ~ICollectionSegment() = default;
        virtual size_t GetSize() const = 0;
        virtual T Get(size_t index) const = 0;
};

#endif // ICOLLECTIONSEGMENT_HPP
