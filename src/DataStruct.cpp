#include "DataStruct.h"

void DataStruct::print() {
        Serial.println(String(firstField) + " " + 
                String(secondField) + " " + 
                String(thirdField));
}