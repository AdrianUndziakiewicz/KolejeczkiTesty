#ifndef ARRAY_PRIORITY_QUEUE_H
#define ARRAY_PRIORITY_QUEUE_H

#include <stdexcept>
#include <iostream>

template <typename T>
class ArrayPriorityQueue {
private:
    //struktura przechowująca element, jego priorytet i czas wstawienia
    struct Element {
        int priority;
        T value;
        unsigned long insertTime;  //czas wstawienia do kolejki (dla FIFO)

        Element() : priority(0), insertTime(0) {}

        Element(int p, const T& v, unsigned long time)
            : priority(p), value(v), insertTime(time) {}
    };

    Element* elements;  //dynamiczna tablica elementów
    int capacity;       //pojemność tablicy
    int size;           //aktualny rozmiar (liczba elementów)
    unsigned long insertCounter;  //licznik wstawień dla realizacji FIFO

    //zmiana rozmiaru tablicy
    void resize(int newCapacity) {
        Element* newElements = new Element[newCapacity];

        //kopiowanie elementów do nowej tablicy
        for (int i = 0; i < size; i++) {
            newElements[i] = elements[i];
        }

        //zwalnianie starej tablicy i aktualizacja wskaźników
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }

    //znajdowanie indeksu elementu o określonej wartości
    int findElementIndex(const T& element) const {
        for (int i = 0; i < size; i++) {
            if (elements[i].value == element) {
                return i;
            }
        }
        return -1; //element nie znaleziony
    }

public:
    //konstruktor
    ArrayPriorityQueue(int initialCapacity = 10) :
        capacity(initialCapacity), size(0), insertCounter(0) {
        elements = new Element[capacity];
    }

    //destruktor
    ~ArrayPriorityQueue() {
        delete[] elements;
    }

    //konstruktor kopiujący
    ArrayPriorityQueue(const ArrayPriorityQueue& other) :
        capacity(other.capacity), size(other.size), insertCounter(other.insertCounter) {
        elements = new Element[capacity];
        for (int i = 0; i < size; i++) {
            elements[i] = other.elements[i];
        }
    }

    //operator przypisania
    ArrayPriorityQueue& operator=(const ArrayPriorityQueue& other) {
        if (this != &other) {
            delete[] elements;

            capacity = other.capacity;
            size = other.size;
            insertCounter = other.insertCounter;

            elements = new Element[capacity];
            for (int i = 0; i < size; i++) {
                elements[i] = other.elements[i];
            }
        }
        return *this;
    }

    //wstawianie elementu z priorytetem
    void insert(const T& element, int priority) {
        //sprawdź, czy potrzebujemy więcej miejsca
        if (size == capacity) {
            resize(capacity * 2);
        }

        //dodaj nowy element na końcu
        elements[size] = Element(priority, element, insertCounter++);
        size++;
    }

    //usunięcie i zwrócenie elementu o najwyższym priorytecie
    T extractMax() {
        if (size == 0) {
            throw std::runtime_error("Kolejka jest pusta");
        }

        //znajdź element o najwyższym priorytecie, przy równych priorytetach wybierz ten, który został wstawiony wcześniej (FIFO)
        int maxIndex = 0;
        for (int i = 1; i < size; i++) {
            if (elements[i].priority > elements[maxIndex].priority ||
                (elements[i].priority == elements[maxIndex].priority &&
                    elements[i].insertTime < elements[maxIndex].insertTime)) {
                maxIndex = i;
            }
        }

        T maxElement = elements[maxIndex].value;

        //usuń element zamieniając go z ostatnim elementem i zmniejszając rozmiar
        elements[maxIndex] = elements[size - 1];
        size--;

        //zmniejsz pojemność, jeśli jest dużo niewykorzystanego miejsca
        if (size > 0 && size <= capacity / 4) {
            resize(capacity / 2);
        }

        return maxElement;
    }

    //zwróć element o najwyższym priorytecie bez usuwania
    T findMax() const {
        if (size == 0) {
            throw std::runtime_error("Kolejka jest pusta");
        }

        int maxIndex = 0;
        for (int i = 1; i < size; i++) {
            if (elements[i].priority > elements[maxIndex].priority ||
                (elements[i].priority == elements[maxIndex].priority &&
                    elements[i].insertTime < elements[maxIndex].insertTime)) {
                maxIndex = i;
            }
        }

        return elements[maxIndex].value;
    }

    //pobierz priorytet określonego elementu
    int getPriority(const T& element) const {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element nie znaleziony w kolejce");
        }
        return elements[index].priority;
    }

    //zmień priorytet elementu
    void modifyKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element nie znaleziony w kolejce");
        }

        elements[index].priority = newPriority;
    }

    //zwiększ priorytet elementu
    void increaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element nie znaleziony w kolejce");
        }

        if (newPriority <= elements[index].priority) {
            throw std::runtime_error("Nowy priorytet musi byc wiekszy niz obecny");
        }

        elements[index].priority = newPriority;
    }

    //zmniejsz priorytet elementu
    void decreaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element nie znaleziony w kolejce");
        }

        if (newPriority >= elements[index].priority) {
            throw std::runtime_error("Nowy priorytet musi byc mniejszy niz obecny");
        }

        elements[index].priority = newPriority;
    }

    //zwróć rozmiar kolejki priorytetowej
    int returnSize() const {
        return size;
    }

    //sprawdź czy kolejka jest pusta
    bool isEmpty() const {
        return size == 0;
    }

    //wyczyść kolejkę
    void clear() {
        size = 0;
        if (capacity > 10) {
            resize(10);
        }
    }

    //wyświetl zawartość kolejki
    void print() const {
        std::cout << "Kolejka Priorytetowa (Tablica): " << std::endl;
        for (int i = 0; i < size; i++) {
            std::cout << "Priorytet: " << elements[i].priority
                << ", Element: " << elements[i].value
                << ", Czas wstawienia: " << elements[i].insertTime << std::endl;
        }
    }
};

#endif
