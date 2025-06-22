#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP


template <typename T>
class List {
    public:
        T data;
        List *next;
        List(T data) {
            this->data = data;
            this->next = nullptr;
        }
};

template <typename T>
class LinkedList {
    private:
        friend class List<T>; 
        List<T> *start, *end;
        size_t size;
    public:
        // Создание объекта
        LinkedList();
        ~LinkedList();
        LinkedList(T* items, size_t count);
        LinkedList(const LinkedList<T> &other);

        // Декомпозиция
        size_t GetLength() const;
        T Get(size_t index) const;
        T GetFirst() const;
        T GetLast() const;

        // Перегрузка операторов
        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        // Операции
        void Append(T item);
        void Prepend(T item);
        void Remove(size_t index);
        void InsertAt(T item, size_t index);
        void PutAt(T item, size_t index);
        LinkedList<T>* Concat(LinkedList<T> *other);
        LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex);
};

// Создание объекта
template <typename T>
LinkedList<T>::LinkedList() {
    this->start = nullptr;
    this->end = nullptr;
    this->size = 0;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    List<T> *p, *temp;
    p = this->start;
    while (p != nullptr) {
        temp = p->next;
        delete p;
        p = temp;
    }
}

template <typename T>
LinkedList<T>::LinkedList(T* items, size_t count): LinkedList<T>::LinkedList() {
    for (size_t i = 0; i < count; i++) {
        LinkedList<T>::Append(items[i]);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &other): LinkedList<T>::LinkedList() {
    List<T> *p = other.start;
    while (p != nullptr) {
        LinkedList<T>::Append(p->data);
        p = p->next;
    }
}

// Декомпозиция
template <typename T>
size_t LinkedList<T>::GetLength() const {
    return this->size;
}

template <typename T>
T LinkedList<T>::Get(size_t index) const {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (size_t i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

template <typename T>
T LinkedList<T>::GetFirst() const {
    if (this->start == nullptr) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return this->start->data;
}

template <typename T>
T LinkedList<T>::GetLast() const {
    if (this->end == nullptr) {
        throw std::out_of_range("Некорректный индекс!");
    }
    return this->end->data;
}

// Перегрузка операторов
template <typename T>
T& LinkedList<T>::operator[](size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (size_t i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

template <typename T>
const T& LinkedList<T>::operator[](size_t index) const {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (size_t i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

// Операции
template <typename T>
void LinkedList<T>::Append(T item) {
    List<T> *p = new List<T>(item);
    if (this->end != nullptr) {
        this->end->next = p;
    } else {
        this->start = p;
    }
    this->end = p;
    this->size++;
}

template <typename T>
void LinkedList<T>::Prepend(T item) {
    List<T> *p = new List<T>(item);
    if (this->start != nullptr) {
        p->next = this->start;
    } else {
        this->end = p;
    }
    this->start = p;
    this->size++;
}

template <typename T>
void LinkedList<T>::Remove(size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    if (index == 0) {
        this->start = p->next;
    } else {
        for (size_t i = 0; i < index-1; i++) {
            p = p->next;
        }
        p->next = p->next->next;
    }
    if (index == this->size-1) {
        this->end = p;
    }
    this->size--;
}

template <typename T>
void LinkedList<T>::InsertAt(T item, size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (size_t i = 0; i < index; i++) {
        p = p->next;
    }
    p->data = item;
}

template <typename T>
void LinkedList<T>::PutAt(T item, size_t index) {
    if (index >= this->size) {
        throw std::out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (size_t i = 0; i < index-1; i++) {
        p = p->next;
    }
    List<T> *k = new List<T>(item);
    k->next = p->next;
    p->next = k;
    this->size++;
}

template <typename T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* other) {
    List<T> *p = other.start;
    while (p != nullptr) {
        LinkedList<T>::Append(p->data);
        p = p->next;
    }
    return this;
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t startIndex, size_t endIndex) {
    if (endIndex >= this->size || endIndex < startIndex) {
        throw std::out_of_range("Некорректный индекс!");
    }
    LinkedList<T>* newList = new LinkedList<T>();
    List<T> *p = this->start;
    for (size_t i = 0; i <= endIndex; i++) {
        if (i >= startIndex) {
            newList->Append(p->data);
        }
        p = p->next;
    }
    return newList;
}

#endif // LINKEDLIST_HPP
