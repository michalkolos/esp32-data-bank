/**
 * @file DataBank.h
 * @author Michal Kolosowski (michalkoloso@gmail.com)
 * @brief Generic circular buffer implementation that stores data in a staticly 
 * allocated memory.
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef DATA_BANK_H
#define DATA_BANK_H


/**
 * Circular buffer implementation that uses preallocated continuous range of
 * memory passed to it in the constructor.
 * @tparam T Type of elements stored in the buffer
 */
template<class T>
class DataBank {
    using sizeType = size_t;
    using indexType = int;
public:

    /**
     * @param bufferMemoryStart Pointer to the beginning of the continuous memory
     * range allocated for the buffer.
     * @param bufferMemoryLen Length of the allocated memory in bytes.
     */
    DataBank(const void *bufferMemoryStart, const sizeType bufferMemoryLen) {
        head = (indexType*)bufferMemoryStart;
        tail = (indexType*)((char*)bufferMemoryStart + sizeof(indexType));
        dataMemory = (T*)((char*)bufferMemoryStart + 2 * sizeof(indexType));

        arraySize = calculateArraySize(bufferMemoryLen - 2 * sizeof(indexType));
    }

    /**
     * Resets the buffer. Required before first use.
     */
    void init() {
        *head = 0;
        *tail = 0;
    }

    /**
     * Puts new element at the end of the buffer. The element is copied in the
     * process. If the buffer is full, last element will be overwritten.
     * @param newElement New element to be put on the buffer.
     */
    void push(const T newElement) {

        *(dataMemory + *head) = newElement;

        advanceIndex(head);
        if(*head == *tail) {
            advanceIndex(tail);
        }
    }

    /**
     * Retrieves the last element stored in the buffer. If the buffer is empty
     * it will keep returning the last available element.
     * @return Oldest element in the buffer.
     */
    T popLast() {
        T result = *(dataMemory + *tail);
        if(*head != *tail) {
            advanceIndex(tail);
        }
        return result;
    }

    /**
     * Retrieves the first element stored in the buffer. If the buffer is empty
     * it will keep returning the last available element.
     * @return Most recent element in the buffer.
     */
    T popRecent() {
        if(*head != *tail) {
            stepBackIndex(head);
        }
        return *(dataMemory + *head);
    }

    /**
     * Retrieves the last element stored in the buffer. But does not remove it
     * from the buffer. If the buffer is empty it will keep returning the last
     * available element.
     * @return Oldest element in the buffer.
     */
    T& peakLast() {
        return *(dataMemory + *tail);
    }

    /**
     * Provides random  access to the buffer. If provided index number exceeds
     * the buffer size it will wrap around to the beginning.
     * @param index Position of the element in the buffer.
     * @return Element retrieved from the buffer.
     */
    T& get(indexType index) {
        randomAccessIndex(&index);
        return *(dataMemory + index);
    }

    /**
     * Provides the number of elements stored in the buffer.
     * @return  Number of elements in the buffer.
     */
    int usage() {
        return (*head >= *tail) ? *head - *tail : arraySize - *tail + *head;
    }

    /**
     * Provides the maximum number of elements that can be stored in the buffer.
     * @return Size of the buffer.
     */
    indexType size() {
        return arraySize - 1;
    }

private:
    T* dataMemory;
    indexType* head;
    indexType* tail;
    indexType arraySize;


    indexType calculateArraySize(const sizeType memoryLen) {
        return memoryLen / sizeof(T);
    }

    void advanceIndex(indexType* index) {
        *index =  ++(*index) % arraySize;
    }

    void stepBackIndex(indexType* index) {
        *index = (*index == 0) ? arraySize - 1 : --(*index);
    }

    void randomAccessIndex(indexType* index) {
        *index = (*tail + *index) % arraySize;
    }
};

#endif //DATA_BANK_H