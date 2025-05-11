#ifndef ARRAY_PRIORITY_QUEUE_H
#define ARRAY_PRIORITY_QUEUE_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

template <typename T>
class ArrayPriorityQueue {
private:
    std::vector<std::pair<int, T>> elements;

    int findElementIndex(const T& element) const {
        for (int i = 0; i < elements.size(); i++) {
            if (elements[i].second == element) {
                return i;
            }
        }
        return -1;
    }

public:
    ArrayPriorityQueue() {}

    void insert(const T& element, int priority) {
        elements.push_back(std::make_pair(priority, element));
    }

    T extractMax() {
        if (elements.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }

        int maxIndex = 0;
        for (int i = 1; i < elements.size(); i++) {
            if (elements[i].first > elements[maxIndex].first) {
                maxIndex = i;
            }
        }

        T maxElement = elements[maxIndex].second;

        elements[maxIndex] = elements.back();
        elements.pop_back();

        return maxElement;
    }

    T findMax() const {
        if (elements.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }

        int maxIndex = 0;
        for (int i = 1; i < elements.size(); i++) {
            if (elements[i].first > elements[maxIndex].first) {
                maxIndex = i;
            }
        }

        return elements[maxIndex].second;
    }

    int getPriority(const T& element) const {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element not found in the priority queue");
        }
        return elements[index].first;
    }

    void modifyKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element not found in the priority queue");
        }

        elements[index].first = newPriority;
    }

    void increaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element not found in the priority queue");
        }

        if (newPriority <= elements[index].first) {
            throw std::runtime_error("New priority must be greater than current priority");
        }

        elements[index].first = newPriority;
    }

    void decreaseKey(const T& element, int newPriority) {
        int index = findElementIndex(element);
        if (index == -1) {
            throw std::runtime_error("Element not found in the priority queue");
        }

        if (newPriority >= elements[index].first) {
            throw std::runtime_error("New priority must be less than current priority");
        }

        elements[index].first = newPriority;
    }

    int returnSize() const {
        return elements.size();
    }

    bool isEmpty() const {
        return elements.empty();
    }

    void print() const {
        std::cout << "Priority Queue (Array): " << std::endl;
        for (const auto& pair : elements) {
            std::cout << "Priority: " << pair.first << ", Element: " << pair.second << std::endl;
        }
    }
};

#endif