#ifndef HEAP_PRIORITY_QUEUE_H
#define HEAP_PRIORITY_QUEUE_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

template <typename T>
class HeapPriorityQueue {
private:
    // kazdy element to para wartosci i prio
    std::vector<std::pair<int, T>> heap;

    //kopiec
    void heapifyUp(int index) {
        int parent = (index - 1) / 2;

        while (index > 0 && heap[parent].first < heap[index].first) {
            std::swap(heap[parent], heap[index]);
            index = parent;
            parent = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left].first > heap[largest].first)
            largest = left;

        if (right < size && heap[right].first > heap[largest].first)
            largest = right;

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

    //znajdowanie elemnetow
    int findElementIndex(const T& element) const {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].second == element) {
                return i;
            }
        }
        return -1; // nic nie ma
    }

public:
    HeapPriorityQueue() {}

    // dodaj elemnt i jego priorytet
    void insert(const T& element, int priority) {
        heap.push_back(std::make_pair(priority, element));
        heapifyUp(heap.size() - 1);
    }

    // usun elemnt z najwiekszym priorytetem
    T extractMax() {
        if (heap.empty()) {
            throw std::runtime_error("Kolejka jest pusta");
        }

        T maxElement = heap[0].second;
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }

        return maxElement;
    }

    // tylko pokaz element z maksymalnym priorytetem
    T findMax() const {
        if (heap.empty()) {
            throw std::runtime_error("Kolejka jest pusta");
        }
        return heap[0].second;
    }

    // pokaz priorytet konkretnego elementu
    int getPriority(const T& element) const {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("nie znaleziono elementu");
        }
        return heap[index].first;
    }

    // zmien priorytet elementu
    void modifyKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("nie znalezionoo takiego elementu");
        }

        int oldPriority = heap[index].first;
        heap[index].first = newPriority;

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

        if (newPriority <= heap[index].first) {
            throw std::runtime_error("Nowy priorytet ma byc wiekszy niz stary");
        }

        heap[index].first = newPriority;
        heapifyUp(index);
    }

    //zmniejsz prioryett
    void decreaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("nie znaleizono takiego elementu");
        }

        if (newPriority >= heap[index].first) {
            throw std::runtime_error("nowy priorytet musi byc wiekszy niz stary");
        }

        heap[index].first = newPriority;
        heapifyDown(index);
    }

    //pokaz rozmiar
    int returnSize() const {
        return heap.size();
    }

    //sprawdz czy pusta
    bool isEmpty() const {
        return heap.empty();
    }

    //pokaz ja
    void print() const {
        std::cout << "Priority Queue (Heap): " << std::endl;
        for (const auto& pair : heap) {
            std::cout << "Priority: " << pair.first << ", Element: " << pair.second << std::endl;
        }
    }
};

#endif // HEAP_PRIORITY_QUEUE_H
