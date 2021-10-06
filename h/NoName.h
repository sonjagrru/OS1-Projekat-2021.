#ifndef NONAME_H_
#define NONAME_H_
#include "Thread.h"

class Semaphore;

class NoName:public Thread{
public:
	NoName(StackSize stackSize = 2048, Time timeSlice = 1);
	virtual void run();
};

#endif
