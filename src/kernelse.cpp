#include "kernelse.h"
#include "kernel.h"
#include "SCHEDULE.H"
#include <iostream.h>

KernelSem::KernelSem(int init,Semaphore* s){
	value=init;
	lista_cekanja=new PCBList();
	myImpl=s;
	Kernel::semafori->FIFOadd(this);
	cout<<"Napravljen semafor"<<endl;
}

KernelSem::~KernelSem (){
	delete lista_cekanja;
	Kernel::semafori->remove(this);
}

int KernelSem::wait (Time maxTimeToWait){
		PCB* trenutna= (PCB*)Kernel::running;
		value=value-1;
		if(value<0){
			trenutna->stanje=PCB::BLOCKED;
			trenutna->_maxTimeToWait=maxTimeToWait;
			lista_cekanja->FIFOadd(trenutna);

//PSW SE CUVA NA STEKU NE MORA OVDE NECE BITI ZAKLJUCANO POSLE DISPATCHA!!!!!GLUPACO!!!
/*#ifndef BCC_BLOCK_IGNORE
			unlock();
#endif*/

			Kernel::dispatch();
		}
		return 0; //PAZI ZA POVRATNU VREDNOST!!!
}

void KernelSem::signal(){
		value=value+1;
		if(value<=0){
			PCB* tmp=lista_cekanja->FIFOget();
			tmp->_maxTimeToWait=noWait;
			tmp->stanje=PCB::READY;
			Scheduler::put(tmp);
		}
}

int KernelSem::val () const{
	return value;
}
