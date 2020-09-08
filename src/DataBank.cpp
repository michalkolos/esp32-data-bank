#include "DataBank.h"


void DataBank::advanceIndex(uint16_t* startingPos) {
    *startingPos = ++(*startingPos) % memorySize ;

    return;
}


void DataBank::retractIndex(uint16_t* startingPos) {
    *startingPos = --(*startingPos);

    if(*startingPos < 0){
        *startingPos = memorySize + *startingPos;
    }

    return;
}


void* DataBank::resolveMemOffset(uint16_t pos) {
    return startAddress + (dataLen * pos);
}






DataBank::DataBank(void* startAddress, size_t dataLen, size_t memoryLen)  : 
        dataLen(dataLen){

    this->memoryLen =  memoryLen - sizeof(BufferState);

    if(this->memoryLen < 0) { this->memoryLen = 0; }

    memorySize = (uint16_t)(this->memoryLen / dataLen);

    bufferState = (BufferState*)startAddress;

    this->startAddress = startAddress + sizeof(BufferState);


    return;
}

DataBank::~DataBank() {
    
}


void DataBank::init(){
    memset(startAddress - sizeof(BufferState), 0, memoryLen);
    this->bufferState->empty = true;
}





void DataBank::push(void* inData) {

    if(memorySize <= 0){ return; }

    memcpy(resolveMemOffset(bufferState->writeIndex), inData, dataLen);
    bufferState->empty = false;

    if(bufferState->writeIndex == bufferState->readIndex){
        advanceIndex(&(bufferState->readIndex));
    }

    advanceIndex(&(bufferState->writeIndex));
  
    
    return;
}

void DataBank::pop(void* outdata, bool move) {
    
}

void DataBank::dump(void* outData, int buffSize, bool move) {
    
}

int DataBank::getUsage() {
    
}

int DataBank::getMaxUsage() {
    
}

String DataBank::toString() {

    // int memLen = memorySize * dataLen;

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
