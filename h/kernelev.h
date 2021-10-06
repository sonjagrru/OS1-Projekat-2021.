#ifndef KERNELEV_H_
#define KRENELEV_H_
#include "event.h"
#include "PCB.h"

class KernelEv{
	private:
		int val;
		IVTNo brojUlaza;
		PCB* nit;
		Event* event;
	public:
		KernelEv(IVTNo ivtNo, Event* e);
		void signal();
		void wait();
		friend class Event;
};


#endif
