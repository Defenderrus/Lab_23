#ifndef SEGMENTFUNCTION_HPP
#define SEGMENTFUNCTION_HPP

#include <cmath>
#include <functional>
#include "sequences/ArraySequence.hpp"
#include "sequences/ListSequence.hpp"
using namespace std;


template <typename T>
class SegmentFunction {
    private:
        struct Segment {
            double start;
            double end;
            function<T(double)> func;
        };
        Sequence<Segment> *segments;
    public:
        SegmentFunction();
        ~SegmentFunction();

        int GetSize();
        double GetStart(int index);
        double GetEnd(int index);
        void Clear();

        void Define(double start, double end, function<T(double)> func);
        bool IsMonotonic() const;
        bool IsContinuous() const;
        T CalculateAt(double x) const;
};

template <typename T>
SegmentFunction<T>::SegmentFunction() {
    this->segments = new ArraySequence<Segment>();
}

template <typename T>
SegmentFunction<T>::~SegmentFunction() {
    delete this->segments;
}

template <typename T>
int SegmentFunction<T>::GetSize() {
    return this->segments->GetLength();
}

template <typename T>
double SegmentFunction<T>::GetStart(int index) {
    return (*segments)[index].start;
}

template <typename T>
double SegmentFunction<T>::GetEnd(int index) {
    return (*segments)[index].end;
}

template <typename T>
void SegmentFunction<T>::Clear() {
    while (segments->GetLength() > 0) {
        segments->Remove(0);
    }
}

template <typename T>
void SegmentFunction<T>::Define(double start, double end, function<T(double)> func) {
    if (start >= end) {
        throw invalid_argument("Неправильные аргументы!");
    }
    bool flag = true;
    int length = segments->GetLength(), counter = 0;
    for (int i = 0; i < length; i++) {
        Segment& segment = (*segments)[i-counter];
        if (start <= segment.start && segment.end <= end) {
            segments->Remove(i-counter);
            counter++;
        } else if (start <= segment.start && segment.start < end) {
            segment.start = end;
            Segment new_segment{start, end, func};
            segments->PutAt(new_segment, i-counter);
            flag = false;
            break;
        } else if (start < segment.end && segment.end <= end) {
            segment.end = start;
            if (i != length-1) {
                if (end <= (*segments)[i-counter+1].start) {
                    Segment new_segment{start, end, func};
                    segments->PutAt(new_segment, i-counter+1);
                    flag = false;
                    break;
                }
            } else {
                Segment new_segment{start, end, func};
                segments->Append(new_segment);
                flag = false;
                break;
            }
        } else if (segment.start < start && end < segment.end) {
            Segment new_segment_1{start, end, func};
            Segment new_segment_2{segment.start, start, segment.func};
            segment.start = end;
            segments->PutAt(new_segment_1, i-counter);
            segments->PutAt(new_segment_2, i-counter);
            flag = false;
            break;
        }
    }
    if (flag) {
        Segment segment{start, end, func};
        segments->Append(segment);
    }
}

template <typename T>
bool SegmentFunction<T>::IsMonotonic() const {
    if (segments->GetLength() == 0) {
        return false;
    }
    double dx, prev_x = (*segments)[0].start;
    T start, end, dy, prev_y = (*segments)[0].func(prev_x);
    bool increase = false, decrease = false;
    for (int i = 0; i < segments->GetLength(); i++) {
        Segment& segment = (*segments)[i];
        start = segment.func(segment.start);
        end = segment.func(segment.end);
        if (abs(prev_x-segment.start) < 1e-6) {
            if (abs(prev_y-start) < 1e-6 && abs(end-start) < 1e-6) {
                //
            } else if (prev_y <= start && start <= end && !decrease) {
                increase = true;
            } else if (prev_y >= start && start >= end && !increase) {
                decrease = true;
            } else {
                return false;
            }
        } else {
            return false;
        }
        dx = (segment.end-segment.start)/100.0;
        for (double x = segment.start; x < segment.end; x += dx) {
            dy = segment.func(x+dx)-segment.func(x);
            if ((dy < 0 && increase) || (dy > 0 && decrease)) {
                return false;
            }
        }
        prev_x = segment.end;
        prev_y = end;
    }
    return true;
}

template <typename T>
bool SegmentFunction<T>::IsContinuous() const {
    if (segments->GetLength() == 0) {
        return false;
    }
    double prev_x = (*segments)[0].start;
    T start, end, prev_y = (*segments)[0].func(prev_x);
    for (int i = 0; i < segments->GetLength(); i++) {
        Segment& segment = (*segments)[i];
        start = segment.func(segment.start);
        end = segment.func(segment.end);
        if (prev_x != segment.start || abs(prev_y-start) > 1e-12) {
            return false;
        }
        prev_x = segment.end;
        prev_y = end;
    }
    return true;
}

template <typename T>
T SegmentFunction<T>::CalculateAt(double x) const {
    for (int i = 0; i < segments->GetLength(); i++) {
        const Segment& left_segment = (*segments)[i];
        if (left_segment.start <= x && x <= left_segment.end) {
            if (x == left_segment.end && i < segments->GetLength()-1) {
                const Segment& right_segment = (*segments)[i+1];
                if (x == right_segment.start && left_segment.func(x) != right_segment.func(x)) {
                    throw domain_error("Критическая точка (разрыв первого рода)");
                }
            }
            return left_segment.func(x);
        }
    }
    throw out_of_range("Функция не определена в точке x");
}

#endif // SEGMENTFUNCTION_HPP
