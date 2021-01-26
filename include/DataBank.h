/**
 * @file DataBank.h
 * @author Michal Kolosowski (you@domain.com)
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

#include <Arduino.h>


template<typename T>
class DataBank
{
private:

    /**
     * Buffer state data placed at the beginning of the reserved memory.
     */
    struct BufferState
    {
        uint16_t readIndex;
        uint16_t writeIndex;
        bool empty;    
    };

    
    /// Pointer to the beginning of reserved memory.
    void* startAddress;

    /// Bytesize of a single storred object .
    size_t dataLen;
    
    /// Bytesize of the total allocated memory.
    size_t memoryLen; 

    /// Maximum number of objects allowed on the buffer.
    uint16_t memorySize;

    /// Pointer to the object storred in the allocated memory that holds current
    /// state of the buffer
    BufferState* bufferState;

    /**
     * @brief Moves a given index one position forwards. At the end returns it
     * to the beginning of the range.
     * 
     * @param startingPos Pointer to the index.
     */
    void advanceIndex(uint16_t* startingPos){
        *startingPos = ++(*(uint8_t*)startingPos) % memorySize ;

        return;
    }

    /**
     * @brief Converts index position to memory address it represents.
     * 
     * @param pos Index.
     * @return void* Data address in buffer memory.
     */
    void* resolveMemOffset(uint16_t pos){
        return (uint8_t*)startAddress + (dataLen * pos);
    }


    


public:

    /**
     * @brief Construct a new DataBank object
     * 
     * @param startAddress Pointer to the beginning of the memory for the buffer.
     * @param memoryLen Length of the memory for the buffer
     */
    DataBank(void* startAddress, size_t memoryLen){
        
        dataLen = sizeof(T);
        this->memoryLen =  memoryLen - sizeof(BufferState);
        if(this->memoryLen < 0) { this->memoryLen = 0; }
        memorySize = (uint16_t)(this->memoryLen / dataLen);
        bufferState = (BufferState*)startAddress;
        this->startAddress = (uint8_t*)startAddress + sizeof(BufferState);

    return;
    }
    
    
    ~DataBank(){
    }


    /**
     * @brief Clears the memory and resets all indexes.
     * 
     */
    void init(){
        memset((uint8_t*)startAddress - sizeof(BufferState), 0, memoryLen);
        this->bufferState->empty = true;    
    }


    /**
     * @brief Returns a number of storrend objects in the buffer.
     * 
     * @return uint16_t Number of objects.
     */
    uint16_t use(){
        if(bufferState->empty){ return 0; }

        uint16_t count = 0;
        uint16_t localReadIndex = bufferState->readIndex;

        do{ 
            count++; 
            advanceIndex(&localReadIndex);
        }while(bufferState->writeIndex != localReadIndex);

        return count;
    }

    /**
     * @brief Puts a copy of given object into the buffer.
     * 
     * @param inData Reference to the source object 
     */
    void push(T& inData){
         if(memorySize <= 0){ return; }

        memcpy(resolveMemOffset(bufferState->writeIndex), &inData, dataLen);
        

        if(!bufferState->empty && bufferState->writeIndex == bufferState->readIndex){
            advanceIndex(&(bufferState->readIndex));
        }

        advanceIndex(&(bufferState->writeIndex));
        bufferState->empty = false;

    return;
    }



    /**
     * @brief Gets oldest element storred in the buffer. 
     * 
     * @param outData Address of an object that will be overwritten by data 
     * from the buffer.
     * @param move Flag that decides whether returning object is moved or copied 
     * form the buffer.
     * @return uint16_t Number of objects left in the buffer.
     */
    uint16_t pop(T* outData, bool move){
                
        if(bufferState->empty){ 
            // outData = nullptr; 
            return 0;
        }

        memcpy(outData, resolveMemOffset(bufferState->readIndex), dataLen);

        if(move){
            advanceIndex(&(bufferState->readIndex));

            if(bufferState->readIndex == bufferState->writeIndex){
                bufferState->empty = true;
            }
        }

        return use();
    }


    bool get(T* outData, int index) {
        if(bufferState->empty || use() <= index) { return false; }
        
        uint16_t queryIndex  = (bufferState->readIndex + index) % memorySize;

        memcpy(outData, resolveMemOffset(queryIndex), dataLen);

        return true;
    }


    
    String toString(){
        String outString = "MemLen: ";
        outString += memoryLen;
        outString += " / DataLen: ";
        outString += (int)dataLen;
        outString += " / MemSize: ";
        outString += memorySize;
        outString += "  // RIndex: ";
        outString += bufferState->readIndex;
        outString += " / WIndex: ";
        outString += bufferState->writeIndex;
        outString += " / Use: ";
        outString += use();
        outString += " / StateStruct size: ";
        outString += sizeof(BufferState);
        outString += "\n";

        outString += "Real memory composition: \n";

        for(int i = 0; i < memorySize; i++){
            void* memPos = resolveMemOffset(i);
            for(int j = 0; j < dataLen; j++){
                outString += *((uint8_t*)memPos + j); 
            }
            outString += " ";
        }

        outString += "\nAbstracted memory composition: \n";

        uint16_t localReadIndex = bufferState->readIndex;

        if(!bufferState->empty){
            do{
                void* memPos = resolveMemOffset(localReadIndex);
                
                for(int j = 0; j < dataLen; j++){
                    outString += *((uint8_t*)memPos + j); 
                }
                outString += " ";

                advanceIndex(&localReadIndex);
            
            }while(localReadIndex != bufferState->writeIndex);
        }

        outString += "\n";

        return outString;
    }
}; 
 


#endif