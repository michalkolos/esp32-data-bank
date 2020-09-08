#ifndef DATA_BANK_H
#define DATA_BANK_H

#include <Arduino.h>



class DataBank
{
private:

    struct BufferState
    {
        uint16_t readIndex;
        uint16_t writeIndex;
        bool empty;    
    };


    void* startAddress;
    size_t dataLen;
    size_t memoryLen; 
    uint16_t memorySize;

    BufferState* bufferState;


    void advanceIndex(uint16_t* startingPos);
    void retractIndex(uint16_t* startingPos);
    
    void* resolveMemOffset(uint16_t pos);


public:
    DataBank(void* startAddress, size_t dataLen, size_t memoryLen);
    
    ~DataBank();

    void init();
    void push(void* inData);
    void pop(void* outdata, bool move);
    void dump(void* outData, int buffSize, bool move); 
    int getUsage();
    int getMaxUsage();

    String toString();
};
 


#endif