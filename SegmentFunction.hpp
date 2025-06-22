#ifndef SEGMENTFUNCTION_HPP
#define SEGMENTFUNCTION_HPP

#include <cmath>
#include <functional>
#include "ICollectionSegment.hpp"
#include "EnumeratorSegment.hpp"
#include "sequences/ArraySequence.hpp"
#include "sequences/ListSequence.hpp"
using namespace std;


template <typename T>
class Segment {
    public:
        double start;
        double end;
        function<T(double)> func;
        Segment(): start(0), end(0), func(nullptr) {}
        Segment(double s, double e, function<T(double)> f): start(s), end(e), func(f) {}
};

template <typename T>
class SegmentFunction: public ICollectionSegment<Segment<T>>, public IEnumerableSegment<Segment<T>> {
    protected:
        friend class Segment<T>;
        Sequence<Segment<T>> *segments;
    public:
        // Конструкторы
        SegmentFunction();
        ~SegmentFunction() override;
        SegmentFunction(const SegmentFunction<T> &other);
        SegmentFunction(SegmentFunction<T> &&other);

        // Вспомогательные функции (+ ICollection)
        size_t GetSize() const override;
        Segment<T> Get(size_t index) const override;
        string Rounding(double number);
        void Clear();

        // Базовые функции
        void Define(double start, double end, function<T(double)> func);
        bool IsMonotonic() const;
        bool IsContinuous() const;
        T CalculateAt(double x);

        // Перегрузка операторов
        T operator()(double x);
        SegmentFunction<T>& operator=(const SegmentFunction<T> &other);
        SegmentFunction<T>& operator=(SegmentFunction<T> &&other);

        // Дополнительные функции
        template <typename U>
        SegmentFunction<U> Map(function<Segment<U>(Segment<T>)> func);
        SegmentFunction<T> Where(function<bool(Segment<T>)> func);
        T Reduce(function<T(T, Segment<T>)> func, T start);
        template <typename U>
        SegmentFunction<pair<T, U>> Zip(SegmentFunction<U> &other);
        template <typename U, typename V>
        static pair<SegmentFunction<U>, SegmentFunction<V>> Unzip(SegmentFunction<pair<U, V>> &other);

        // IEnumerator + IEnumerable
        class IteratorSegment: public IEnumeratorSegment<Segment<T>> {
            private:
                const SegmentFunction<T> *segmentFunction;
                size_t index;
                bool mode;
            public:
                IteratorSegment(const SegmentFunction<T> *other) {
                    segmentFunction = other;
                    index = 0;
                    mode = true;
                }
                Segment<T> Current() override {
                    if (mode || index >= segmentFunction->GetSize()) {
                        throw out_of_range("Неправильный индекс!");
                    }
                    return segmentFunction->Get(index);
                }
                void Reset() override {
                    index = 0;
                    mode = true;
                }
                bool MoveNext() override {
                    if (mode) {
                        mode = false;
                        return segmentFunction->GetSize() > 0;
                    }
                    index++;
                    return index < segmentFunction->GetSize();
                }
        };
        IEnumeratorSegment<Segment<T>>* GetEnumerator() override {
            return new IteratorSegment(this);
        }
};

// Конструкторы
template <typename T>
SegmentFunction<T>::SegmentFunction() {
    segments = new ArraySequence<Segment<T>>();
}

template <typename T>
SegmentFunction<T>::~SegmentFunction() {
    delete segments;
}

template <typename T>
SegmentFunction<T>::SegmentFunction(const SegmentFunction<T> &other) {
    segments = new ArraySequence<Segment<T>>();
    for (size_t i = 0; i < other.GetSize(); i++) {
        Segment<T> segment = other.Get(i);
        segments->Append(segment);
    }
}

template <typename T>
SegmentFunction<T>::SegmentFunction(SegmentFunction<T> &&other) {
    segments = other.segments;
    other.segments = nullptr;
}

// Вспомогательные функции
template <typename T>
size_t SegmentFunction<T>::GetSize() const {
    return segments->GetLength();
}

template <typename T>
Segment<T> SegmentFunction<T>::Get(size_t index) const {
    if (index >= GetSize()) {
        throw out_of_range("Неправильный индекс!");
    }
    return (*segments)[index];
}

template <typename T>
string SegmentFunction<T>::Rounding(double number) {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.2f", number);
    return string(buffer);
}

template <typename T>
void SegmentFunction<T>::Clear() {
    while (GetSize() > 0) segments->Remove(0);
}

// Базовые функции
template <typename T>
void SegmentFunction<T>::Define(double start, double end, function<T(double)> func) {
    if (start >= end) throw invalid_argument("Неправильные аргументы!");
    bool flag = true;
    size_t length = segments->GetLength(), counter = 0;
    for (size_t i = 0; i < length; i++) {
        Segment<T> &segment = (*segments)[i-counter];
        if (start <= segment.start && segment.end <= end) {
            segments->Remove(i-counter);
            counter++;
        } else if (start <= segment.start && segment.start < end) {
            segment.start = end;
            Segment<T> new_segment(start, end, func);
            segments->PutAt(new_segment, i-counter);
            flag = false;
            break;
        } else if (start < segment.end && segment.end <= end) {
            segment.end = start;
            if (i != length-1) {
                if (end <= (*segments)[i-counter+1].start) {
                    Segment<T> new_segment(start, end, func);
                    segments->PutAt(new_segment, i-counter+1);
                    flag = false;
                    break;
                }
            } else {
                Segment<T> new_segment(start, end, func);
                segments->Append(new_segment);
                flag = false;
                break;
            }
        } else if (segment.start < start && end < segment.end) {
            Segment<T> new_segment_1(start, end, func);
            Segment<T> new_segment_2(segment.start, start, segment.func);
            segment.start = end;
            segments->PutAt(new_segment_1, i-counter);
            segments->PutAt(new_segment_2, i-counter);
            flag = false;
            break;
        }
    }
    if (flag) {
        Segment<T> segment(start, end, func);
        segments->Append(segment);
    }
}

template <typename T>
bool SegmentFunction<T>::IsMonotonic() const {
    if (segments->GetLength() == 0) return false;
    double dx, prev_x = (*segments)[0].start;
    T start, end, dy, prev_y = (*segments)[0].func(prev_x);
    bool increase = false, decrease = false;
    for (size_t i = 0; i < segments->GetLength(); i++) {
        Segment<T> &segment = (*segments)[i];
        start = segment.func(segment.start);
        end = segment.func(segment.end);
        if (abs(prev_x-segment.start) < 1e-6) {
            if (abs(prev_y-start) < 1e-6 && abs(end-start) < 1e-6) {}
            else if (prev_y <= start && start <= end && !decrease) increase = true;
            else if (prev_y >= start && start >= end && !increase) decrease = true;
            else return false;
        } else return false;
        dx = (segment.end-segment.start)/100.0;
        for (double x = segment.start; x < segment.end; x += dx) {
            dy = segment.func(x+dx)-segment.func(x);
            if ((dy < 0 && increase) || (dy > 0 && decrease)) return false;
        }
        prev_x = segment.end;
        prev_y = end;
    }
    return true;
}

template <typename T>
bool SegmentFunction<T>::IsContinuous() const {
    if (segments->GetLength() == 0) return false;
    double prev_x = (*segments)[0].start;
    T start, end, prev_y = (*segments)[0].func(prev_x);
    for (size_t i = 0; i < segments->GetLength(); i++) {
        Segment<T> &segment = (*segments)[i];
        start = segment.func(segment.start);
        end = segment.func(segment.end);
        if (prev_x != segment.start || abs(prev_y-start) > 1e-12) return false;
        prev_x = segment.end;
        prev_y = end;
    }
    return true;
}

template <typename T>
T SegmentFunction<T>::CalculateAt(double x) {
    for (size_t i = 0; i < segments->GetLength(); i++) {
        const Segment<T> &left_segment = (*segments)[i];
        if (left_segment.start <= x && x <= left_segment.end) {
            if (x == left_segment.end && i < segments->GetLength()-1) {
                const Segment<T> &right_segment = (*segments)[i+1];
                if (x == right_segment.start && left_segment.func(x) != right_segment.func(x)) {
                    throw domain_error("Критическая точка x = "+Rounding(x)+" (разрыв)");
                }
            }
            return left_segment.func(x);
        }
    }
    throw out_of_range("Функция не определена в точке x = "+Rounding(x)+"!");
}

// Перегрузка операторов
template <typename T>
T SegmentFunction<T>::operator()(double x) {
    return CalculateAt(x);
}

template <typename T>
SegmentFunction<T>& SegmentFunction<T>::operator=(const SegmentFunction<T> &other) {
    if (this != &other) {
        delete segments;
        segments = new ArraySequence<Segment<T>>();
        for (size_t i = 0; i < other.GetSize(); i++) {
            Segment<T> segment = other.Get(i);
            segments->Append(segment);
        }
    }
    return *this;
}

template <typename T>
SegmentFunction<T>& SegmentFunction<T>::operator=(SegmentFunction<T> &&other) {
    if (this != &other) {
        delete segments;
        segments = other.segments;
        other.segments = nullptr;
    }
    return *this;
}

// Дополнительные функции
template <typename T>
template <typename U>
SegmentFunction<U> SegmentFunction<T>::Map(function<Segment<U>(Segment<T>)> func) {
    SegmentFunction<U> result;
    for (size_t i = 0; i < segments->GetLength(); i++) {
        Segment<T> &segment = (*segments)[i];
        Segment<U> newSegment = func(segment);
        result.Define(newSegment.start, newSegment.end, newSegment.func);
    }
    return result;
}

template <typename T>
SegmentFunction<T> SegmentFunction<T>::Where(function<bool(Segment<T>)> func) {
    SegmentFunction<T> result;
    for (size_t i = 0; i < segments->GetLength(); i++) {
        Segment<T> &segment = (*segments)[i];
        if (func(segment)) result.Define(segment.start, segment.end, segment.func);
    }
    return result;
}

template <typename T>
T SegmentFunction<T>::Reduce(function<T(T, Segment<T>)> func, T start) {
    for (size_t i = 0; i < segments->GetLength(); i++) {
        Segment<T> &segment = (*segments)[i];
        start = func(start, segment);
    }
    return start;
}

template <typename T>
template <typename U>
SegmentFunction<pair<T, U>> SegmentFunction<T>::Zip(SegmentFunction<U> &other) {
    SegmentFunction<pair<T, U>> result;
    size_t i = 0, j = 0;
    while (i < segments->GetLength() && j < other.segments->GetLength()) {
        Segment<T> &segment1 = (*segments)[i];
        Segment<U> &segment2 = (*other.segments)[j];
        double start = max(segment1.start, segment2.start);
        double end = min(segment1.end, segment2.end);
        if (start < end) {
            function<pair<T, U>(double)> newFunction = [segment1, segment2](double x) {
                return make_pair(segment1.func(x), segment2.func(x));
            };
            result.Define(start, end, newFunction);
        }
        if (segment1.end < segment2.end) i++;
        else j++;
    }
    return result;
}

template <typename T>
template <typename U, typename V>
pair<SegmentFunction<U>, SegmentFunction<V>> SegmentFunction<T>::Unzip(SegmentFunction<pair<U, V>> &other) {
    SegmentFunction<U> first;
    SegmentFunction<V> second;
    for (size_t i = 0; i < other.GetSize(); i++) {
        auto segment = other.Get(i);
        auto firstFunction = [segment](double x) {return segment.func(x).first;};
        auto secondFunction = [segment](double x) {return segment.func(x).second;};
        first.Define(segment.start, segment.end, firstFunction);
        second.Define(segment.start, segment.end, secondFunction);
    }
    return {move(first), move(second)};
}

template <typename T>
class ImmutableSegmentFunction: public SegmentFunction<T> {
    public:
        ImmutableSegmentFunction(const SegmentFunction<T> &other): SegmentFunction<T>() {
            this->segments = new ArraySequence<Segment<T>>();
            for (size_t i = 0; i < other.GetSize(); ++i) {
                Segment<T> segment = other.Get(i);
                this->segments->Append(segment);
            }
        }
        ImmutableSegmentFunction(SegmentFunction<T> &&other): SegmentFunction<T>() {
            this->segments = other.segments;
            other.segments = nullptr;
        }
        ImmutableSegmentFunction(const ImmutableSegmentFunction&) = delete;
        ImmutableSegmentFunction& operator=(const ImmutableSegmentFunction&) = delete;
        T operator()(double x) {return SegmentFunction<T>::operator()(x);}
        void Define(double, double, std::function<T(double)>) = delete;
        void Clear() = delete;
};

#endif // SEGMENTFUNCTION_HPP
