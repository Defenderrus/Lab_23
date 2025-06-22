#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP


template <typename T>
class DynamicArray {
    private:
        T* data;
        size_t size;
    public:
        // Создание объекта
        DynamicArray();
        ~DynamicArray();
        DynamicArray(size_t size);
        DynamicArray(T* items, size_t count);
        DynamicArray(const DynamicArray<T> &dynamicArray);

        // Декомпозиция
        size_t GetSize() const;
        T Get(size_t index) const;

        // Перегрузка операторов
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        
        // Операции
        void Set(size_t index, T value);
        void Resize(size_t newSize);
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
DynamicArray<T>::DynamicArray(size_t size): DynamicArray<T>::DynamicArray() {
    if (size > 0) {
        this->size = size;
        this->data = new T[size];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(T* items, size_t count) {
    this->size = count;
    this->data = new T[count];
    if (count != 0) {
        for (size_t i = 0; i < count; i++) {
            this->data[i] = items[i];
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &dynamicArray) {
    this->size = dynamicArray.size;
    this->data = new T[dynamicArray.size];
    if (dynamicArray.size != 0) {
        for (size_t i = 0; i < dynamicArray.size; i++) {
            this->data[i] = dynamicArray.data[i];
        }
    }
}

// Декомпозиция
template <typename T>
size_t DynamicArray<T>::GetSize() const {
    return this->size;
}

template <typename T>
T DynamicArray<T>::Get(size_t index) const {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return this->data[index];
}

// Перегрузка операторов
template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return data[index];
}

// Операции
template <typename T>
void DynamicArray<T>::Set(size_t index, T value) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    } else {
        this->data[index] = value;
    }
}

template <typename T>
void DynamicArray<T>::Resize(size_t newSize) {
    T* newData = new T[newSize];
    for (size_t i = 0; i <= std::min(newSize, this->size); i++) {
        newData[i] = this->data[i];
    }
    delete[] this->data;
    this->size = newSize;
    this->data = newData;
}

#endif // DYNAMICARRAY_HPP
