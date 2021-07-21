# esp32-data-bank

ESP32 microcontroller allows to store data during deep sleep state in a special
memory region called RTC memory. However only statically allocated primitive or
c-struct objects are allowed. Also this memory must be allocated early in the
startup process. Before the `setup()` function begins.

This implementation of circular buffer uses preallocated continuous memory to
store any data including c++ class objects when ESP32 is in sleep mode and its
main memory is powered off.


## Usage

### 1. Memory allocation:

``` C
RTC_DATA_ATTR uint8_t bufferMemory[MEMORY_SIZE];
```

Allocated memory does not need to be in the RTC_DATA space. However then all
stored data will be lost on wake-up.

### 2. Declaring DataBank object:

``` C
DataBank<FooClass> dataBank(bufferMemory, MEMORY_SIZE);
```

DataBank is a generic type FooClass represents a class of objects to store in the buffer.

BufferMemory is a pointer to the beginning of the allocated space.

### 3. Initializing buffer:

``` C
void init();
```

Sets internal indexes to the beginning. Can be called multiple times to reset the DataBank.

### 4. Operations - available methods:

#### * Putting objects on the buffer:

``` C
void push(const T newElement);
```

This creates a copy of the object in the buffer memory. If the memory allocated for the bank is exceeded, the oldest stored object gets overwritten.

#### * Getting objects from the buffer:
  
``` C
T popLast();
```

Returns the oldest stored element. Object in the buffer gets deleted. If the buffer
is empty it will keep returning the last available element.

``` C
T popRecent();
```

Same as above, but returns the most recently added element to the buffer.


``` C
T& peakLast();
```

Retrieves the last element stored in the buffer. But does not remove it from the buffer.

``` C
T& get(indexType index);
```

Provides random  access to the buffer.

#### * Getting information about the buffer:

``` C
int usage();
```

Provides the number of elements stored in the buffer.

``` C
int size();
```

Returns the maximum number of elements that can be stored in the buffer.

