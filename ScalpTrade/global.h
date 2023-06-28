#pragma once
#include <string>
#include <cstdint>
#include "ConcurrentQueue.h"
#include "ConcurrentQueue.h"
#include <atomic>






//Concurrent Queues defined to Consume Byte data from the Market Data TCP Streams.
extern ConcurrentQueue<char*> *inputByteQueue ;

//Concurrent Queue defined to Push Byte data to the Output Order TCP Stream.
extern ConcurrentQueue<char*> *outputByteQueue ;
