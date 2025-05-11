#ifndef HEAP_PRIORITY_QUEUE_H
#define HEAP_PRIORITY_QUEUE_H

#include <stdexcept>
#include <iostream>

template <typename T>
class HeapPriorityQueue {
private:
 //struktura  do przechowywania  elementu  jego priorytetu i czas wstawienia
    struct HeapElement {
        int priority;
        T value;
        unsigned long insertTime;  

        HeapElement() : priority(0), insertTime(0) {}                     //czas wstawienia do kolejki (dla FIFO)

        HeapElement(int p, const T& v, unsigned long time)
            : priority(p), value(v), insertTime(time) {}
    };

    HeapElement* heap;  //dynamiczna tablica elementow kopca
    int capacity;         //pojemnosc tablicy
    int size;            //aktualny rozmiar (liczba elementow)
    unsigned long insertCounter;       //Licznik wstawien dla realizacji FIFO

    //zmiana rozmiaru tablicy
    void resize(int newCapacity) {
        HeapElement* newHeap = new HeapElement[newCapacity];

        //kopiowanie elementow do nowej tablicy
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }

        //zwalnianie starej tablicy i aktualizacja wskaznikow
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

   //przywraca wlasciwosc kopca 
    void heapifyUp(int index) {
        int parent = (index - 1) / 2;

        while (index > 0) {                           
            if (heap[parent].priority < heap[index].priority) {//Jesli priorytet rodzica jest mniejszy  zamieniamy 
                std::swap(heap[parent], heap[index]);
                index = parent;
                parent = (index - 1) / 2;
            }
            // Jesli priory są takie same  sprawdzamy czas wstawienia (FIFO)
            else if (heap[parent].priority == heap[index].priority &&
                heap[parent].insertTime > heap[index].insertTime) {
                std::swap(heap[parent], heap[index]);
                index = parent;
                parent = (index - 1) / 2;
            }
            else {
                break;  // wlasciwiosci  kopca zostaja takie same
            }
        }
    }

    void heapifyDown(int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

       //sprawdzamy  czy lewe dziecko ma wieksze priory lub rowny z wczesniejszym czasem wstawienia
        if (left < size &&
            (heap[left].priority > heap[largest].priority ||
                (heap[left].priority == heap[largest].priority &&
                    heap[left].insertTime < heap[largest].insertTime))) {
            largest = left;
        }

           //sprawdzamy  czy prawe dziecko ma wieksze priory lub rowny z wczesniejszym czasem wstawienia
        if (right < size &&
            (heap[right].priority > heap[largest].priority ||
                (heap[right].priority == heap[largest].priority &&
                    heap[right].insertTime < heap[largest].insertTime))) {
            largest = right;
        }

        // Jeśli znaleziono wiekszy element  zamieniamy i idziemy w dol
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

                                
    int findElementIndex(const T& element) const {
        for (int i = 0; i < size; i++) {                      //znajdowanie indeksu elementu
            if (heap[i].value == element) {
                return i;
            }
        }
        return -1; //Ni ma
    }

public:
    //konstruktor
    HeapPriorityQueue(int initialCapacity = 10) :
        capacity(initialCapacity), size(0), insertCounter(0) {
        heap = new HeapElement[capacity];
    }

    
    ~HeapPriorityQueue() {         //destruktor
        delete[] heap;
    }

    
    HeapPriorityQueue(const HeapPriorityQueue& other) :           //konstruktor kopiuj
        capacity(other.capacity), size(other.size), insertCounter(other.insertCounter) {
        heap = new HeapElement[capacity];
        for (int i = 0; i < size; i++) {
            heap[i] = other.heap[i];
        }
    }

    // Operator przypisania
    HeapPriorityQueue& operator=(const HeapPriorityQueue& other) {    
        if (this != &other) {
            delete[] heap;

            capacity = other.capacity;
            size = other.size;
            insertCounter = other.insertCounter;

            heap = new HeapElement[capacity];
            for (int i = 0; i < size; i++) {
                heap[i] = other.heap[i];
            }
        }
        return *this;
    }

    //dodaj element i jego priorytet
    void insert(const T& element, int priority) {
        
        if (size == capacity) {       //sprawdzamy czy trzeba wiecej miejsca
            resize(capacity * 2);
        }

        
        heap[size] = HeapElement(priority, element, insertCounter++); // nowy element na koncu

        //przywroc wlasciwosci kopca
        heapifyUp(size);
        size++;
    }

    //usun element z najwiekszym priorytetem
    T extractMax() {
        if (size == 0) {
            throw std::runtime_error("Kolejka jest pusta");
        }

        T maxElement = heap[0].value;

        //przenies ostatni element na poczatek i zmniejsz rozmiar
        heap[0] = heap[size - 1];
        size--;

        //Przywroc wlasnoci kopca
        if (size > 0) {
            heapifyDown(0);
        }

        //Zmniejsz pojemnosc jeśli jest dużo niewykorzystanego miejsca
        if (size > 0 && size <= capacity / 4) {
            resize(capacity / 2);
        }

        return maxElement;
    }

    
    T findMax() const {
        if (size == 0) {                             
            throw std::runtime_error("Kolejka jest pusta");
        }
        return heap[0].value;                         //poka element z maksymalnym priorytetem
    }

    
    int getPriority(const T& element) const {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Nie znaleziono elementu");           //pokaz priorytet konkretnego elementu
        }
        return heap[index].priority;
    }

    
    void modifyKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {                            //zmien priorytet elementu
            throw std::runtime_error("Nie znaleziono takiego elementu");
        }

        int oldPriority = heap[index].priority;
        heap[index].priority = newPriority;

        if (newPriority > oldPriority) {
            heapifyUp(index);
        }
        else if (newPriority < oldPriority) {
            heapifyDown(index);
        }
    }

    //zwieksz priorytet
    void increaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Nie znaleziono takiego elementu");
        }

        if (newPriority <= heap[index].priority) {
            throw std::runtime_error("Nowy priorytet musi byc wiekszy niz stary");
        }

        heap[index].priority = newPriority;
        heapifyUp(index);
    }

    //zmniejsz priorytet
    void decreaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Nie znaleziono takiego elementu");
        }

        if (newPriority >= heap[index].priority) {
            throw std::runtime_error("Nowy priorytet musi byc mniejszy niz stary");
        }

        heap[index].priority = newPriority;
        heapifyDown(index);
    }

    
    int returnSize() const { //pokaz rozmiar
        return size;
    }

    // czy pusta
    bool isEmpty() const {
        return size == 0;
    }

    //wyczysc kolejke
    void clear() {
        size = 0;
        
        if (capacity > 10) {     // jesli trzeba zwieksz pojemnosc
            resize(10);
        }
    }

   
    void print() const {
        std::cout << "Kolejka Priorytetowa (Kopiec): " << std::endl;    //pokaz zawartosc kolejki
        for (int i = 0; i < size; i++) {
            std::cout << "Priorytet: " << heap[i].priority
                << ", Element: " << heap[i].value
                << ", Czas wstawienia: " << heap[i].insertTime << std::endl;
        }
    }
};

#endif // HEAP_PRIORITY_QUEUE_H
