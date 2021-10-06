#ifndef KERNELSE_H_
#define KERNELSE_H_
#include "semaphor.h"

class PCBList;

class KernelSem{
	public:
		KernelSem (int init=1,Semaphore*s);
		~KernelSem ();
		int wait (Time maxTimeToWait);
		void signal();
		int val () const;  // Returns the current value of the semaphore

		Semaphore* myImpl;
		volatile int value;
		PCBList* lista_cekanja;
};


#endif
