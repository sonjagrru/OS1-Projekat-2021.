#ifndef NIT_H_
#define NIT_H_
#include "Thread.h"

class Semaphore;

class Nit:public Thread{
private:
	Semaphore* s;
public:
	Nit(StackSize stackSize = defaultStackSize, Time timeSlice = 1);
	virtual void run();
	void start();
	void dodajSem(Semaphore* ss);
};

#endif
