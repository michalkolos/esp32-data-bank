#ifndef DATA_BANK_H
#define DATA_BANK_H

#include <Arduino.h>


template<typename T>
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


    void advanceIndex(uint16_t* startingPos){
        *startingPos = ++(*(uint8_t*)startingPos) % memorySize ;

        return;
    }

    
    void* resolveMemOffset(uint16_t pos){
        return (uint8_t*)startAddress + (dataLen * pos);
    }


    


public:
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




    void init(){
        memset((uint8_t*)startAddress - sizeof(BufferState), 0, memoryLen);
        this->bufferState->empty = true;    
    }


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




    void dump(void* outData, int buffSize, bool move); 
    int getUsage();
    int getMaxUsage();

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