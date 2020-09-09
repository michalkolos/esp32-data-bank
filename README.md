# esp32-data-bank

ESP32 microcontroller allows to store data during sleep state in its RTC memory. 
However only statically allocated primitive or c-struct objects are allowed.

This implementation of cicular buffer allows to store any continous data 
including c++ class objects when ESP32 is in sleep mode and its main memory is 
powered off.


## Usage

### 1. Memory allocation:

``` C
RTC_DATA_ATTR uint8_t bufferMemory[MEMORY_SIZE];
```

Allocated memory does not need to be in the RTC_DATA space. However then all 
storred data will be lost on wake-up.

### 2. Declaring DataBank object:

``` C
DataBank<FooClass> dataBank(bufferMemory, MEMORY_SIZE);
```

DataBank is a generic type FooClass represents a class of objects to store in the buffer.

BufferMemory is a pointer to the beginning of the allocated space.

### 3. Initializing buffer:

``` C
dataBank.init();
```

Clears the memory and sets internal indexes to the beginning. Can be called multiple times to reset the DataBank.

### 4. Operations:

* Putting objects on the buffer:

``` C
dataBank.push(obj);
```

This creates a copy of the object in the buffer memory. If the memory allocated for the bank is exceeded, the oldest stored object gets overwritten.

* Getting objects from the buffer:
  
``` C
dataBank.pop(&obj, true);
```

Returns the oldest stored element. Data from the buffer will be copied to the object under given address. Second parameter determines if the source data in the buffer gets deleted.

## TODO

* Implement random access to the data.
* Implement accessing the data without copying memory contents.
