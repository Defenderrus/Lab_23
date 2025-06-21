#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

using namespace std;


template <typename T>
class DynamicArray {
    private:
        T* data;
        int size;
    public:
        // Создание объекта
        DynamicArray();
        ~DynamicArray();
        DynamicArray(int size);
        DynamicArray(T* items, int count);
        DynamicArray(const DynamicArray<T> &dynamicArray);

        // Декомпозиция
        int GetSize() const;
        T Get(int index) const;
        T& operator[](int index);
        const T& operator[](int index) const;
        
        // Операции
        void Set(int index, T value);
        void Resize(int newSize);
};

// Создание объекта
template <typename T>
DynamicArray<T>::DynamicArray() {
    this->size = 0;
    this->data = nullptr;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
DynamicArray<T>::DynamicArray(int size): DynamicArray<T>::DynamicArray() {
    if (size > 0) {
        this->size = size;
        this->data = new T[size];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    this->size = count;
    this->data = new T[count];
    if (count != 0) {
        for (int i = 0; i < count; i++) {
            this->data[i] = items[i];
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &dynamicArray) {
    this->size = dynamicArray.size;
    this->data = new T[dynamicArray.size];
    if (dynamicArray.size != 0) {
        for (int i = 0; i < dynamicArray.size; i++) {
            this->data[i] = dynamicArray.data[i];
        }
    }
}

// Декомпозиция
template <typename T>
int DynamicArray<T>::GetSize() const {
    return this->size;
}

template <typename T>
T DynamicArray<T>::Get(int index) const {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    return this->data[index];
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    return data[index];
}

// Операции
template <typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    } else {
        this->data[index] = value;
    }
}

template <typename T>
void DynamicArray<T>::Resize(int newSize) {
    T* newData = new T[newSize];
    for (int i = 0; i <= min(newSize, this->size); i++) {
        newData[i] = this->data[i];
    }
    delete[] this->data;
    this->size = newSize;
    this->data = newData;
}

#endif // DYNAMICARRAY_HPP
