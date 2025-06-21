#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

using namespace std;


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
        int size;
    public:
        // Создание объекта
        LinkedList();
        ~LinkedList();
        LinkedList(T* items, int count);
        LinkedList(const LinkedList<T> &other);

        // Декомпозиция
        int GetLength() const;
        T GetFirst() const;
        T GetLast() const;
        T Get(int index) const;
        T& operator[](int index);
        const T& operator[](int index) const;
        LinkedList<T>* GetSubList(int startIndex, int endIndex) const;

        // Операции
        void Append(T item);
        void Prepend(T item);
        void Remove(int index);
        void InsertAt(T item, int index);
        void PutAt(T item, int index);
        LinkedList<T>* Concat(LinkedList<T> *other);
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
LinkedList<T>::LinkedList(T* items, int count): LinkedList<T>::LinkedList() {
    for (int i = 0; i < count; i++) {
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
int LinkedList<T>::GetLength() const {
    return this->size;
}

template <typename T>
T LinkedList<T>::GetFirst() const {
    if (this->start == nullptr) {
        throw out_of_range("Некорректный индекс!");
    }
    return this->start->data;
}

template <typename T>
T LinkedList<T>::GetLast() const {
    if (this->end == nullptr) {
        throw out_of_range("Некорректный индекс!");
    }
    return this->end->data;
}

template <typename T>
T LinkedList<T>::Get(int index) const {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

template <typename T>
T& LinkedList<T>::operator[](int index) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

template <typename T>
const T& LinkedList<T>::operator[](int index) const {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p->data;
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (endIndex >= this->size || startIndex < 0 || endIndex < startIndex) {
        throw out_of_range("Некорректный индекс!");
    }
    LinkedList<T>* newList = new LinkedList<T>();
    List<T> *p = this->start;
    for (int i = 0; i <= endIndex; i++) {
        if (i >= startIndex) {
            newList->Append(p->data);
        }
        p = p->next;
    }
    return newList;
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
void LinkedList<T>::Remove(int index) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    if (index == 0) {
        this->start = p->next;
    } else {
        for (int i = 0; i < index-1; i++) {
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
void LinkedList<T>::InsertAt(T item, int index) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    p->data = item;
}

template <typename T>
void LinkedList<T>::PutAt(T item, int index) {
    if (index >= this->size || index < 0) {
        throw out_of_range("Некорректный индекс!");
    }
    List<T> *p = this->start;
    for (int i = 0; i < index-1; i++) {
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

#endif // LINKEDLIST_HPP
