/*
 * MutablePriorityQueue.h
 * A simple implementation of mutable priority queues, required by Dijkstra algorithm.
 *
 * Created on: 17/03/2018
 *      Author: João Pascoal Faria
 */

#ifndef DA_TP_CLASSES_MUTABLEPRIORITYQUEUE
#define DA_TP_CLASSES_MUTABLEPRIORITYQUEUE

#include <vector>

/**
 * @class MutablePriorityQueue
 * @brief A mutable priority queue implementation used primarily by Dijkstra's algorithm
 * 
 * This data structure maintains elements in a binary heap, allowing efficient extraction
 * of the minimum element and updating priorities of existing elements. The class T must have:
 * (i) accessible field int queueIndex
 * (ii) operator< defined for comparison
 * 
 * @tparam T The type of elements stored in the queue
 */
template<class T>
class MutablePriorityQueue {
    std::vector<T *> H; /**< Heap array (1-indexed for easier parent/child calculations) */

    /**
     * @brief Moves an element up the heap to restore heap property
     * @param i Index of the element to move up
     * @complexity O(log n) where n is the number of elements in the queue
     */
    void heapifyUp(unsigned i);

    /**
     * @brief Moves an element down the heap to restore heap property
     * @param i Index of the element to move down
     * @complexity O(log n) where n is the number of elements in the queue
     */
    void heapifyDown(unsigned i);

    /**
     * @brief Sets an element at a specific position in the heap
     * @param i Index where to place the element
     * @param x Pointer to the element
     * @complexity O(1)
     */
    inline void set(unsigned i, T *x);

public:
    /**
     * @brief Constructor
     * @complexity O(1)
     */
    MutablePriorityQueue();

    /**
     * @brief Inserts a new element into the queue
     * @param x Pointer to the element to insert
     * @complexity O(log n) where n is the number of elements in the queue
     */
    void insert(T *x);

    /**
     * @brief Extracts and returns the minimum element from the queue
     * @return Pointer to the minimum element
     * @complexity O(log n) where n is the number of elements in the queue
     */
    T *extractMin();

    /**
     * @brief Updates the position of an element whose priority has decreased
     * @param x Pointer to the element whose priority changed
     * @complexity O(log n) where n is the number of elements in the queue
     */
    void decreaseKey(T *x);

    /**
     * @brief Checks if the queue is empty
     * @return True if the queue is empty, false otherwise
     * @complexity O(1)
     */
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

/**
 * @brief Constructor initializes the queue with a null element at index 0
 * @complexity O(1)
 */
template<class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

/**
 * @brief Checks if the queue is empty
 * @return True if the queue is empty, false otherwise
 * @complexity O(1)
 */
template<class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

/**
 * @brief Extracts and returns the minimum element from the queue
 * @return Pointer to the minimum element
 * @complexity O(log n) where n is the number of elements in the queue
 */
template<class T>
T *MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if (H.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}

/**
 * @brief Inserts a new element into the queue
 * @param x Pointer to the element to insert
 * @complexity O(log n) where n is the number of elements in the queue
 */
template<class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size() - 1);
}

/**
 * @brief Updates the position of an element whose priority has decreased
 * @param x Pointer to the element whose priority changed
 * @complexity O(log n) where n is the number of elements in the queue
 */
template<class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex);
}

/**
 * @brief Moves an element up the heap to restore heap property
 * 
 * If the element has a lower priority than its parent, swap them
 * and continue moving up the heap.
 * 
 * @param i Index of the element to move up
 * @complexity O(log n) where n is the number of elements in the queue
 */
template<class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

/**
 * @brief Moves an element down the heap to restore heap property
 * 
 * If the element has a higher priority than its children, swap it with
 * the child of lowest priority and continue moving down the heap.
 * 
 * @param i Index of the element to move down
 * @complexity O(log n) where n is the number of elements in the queue
 */
template<class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k + 1 < H.size() && *H[k + 1] < *H[k])
            ++k; // right child of i
        if (!(*H[k] < *x))
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

/**
 * @brief Sets an element at a specific position in the heap
 * 
 * Updates both the heap array and the queueIndex of the element.
 * 
 * @param i Index where to place the element
 * @param x Pointer to the element
 * @complexity O(1)
 */
template<class T>
void MutablePriorityQueue<T>::set(unsigned i, T *x) {
    H[i] = x;
    x->queueIndex = i;
}

#endif /* DA_TP_CLASSES_MUTABLEPRIORITYQUEUE */
