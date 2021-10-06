#include "kernelev.h"
#include "IVTEntry.h"
#include "kernel.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo i, Event* e){
	brojUlaza=i;
	val=0;
	nit=(PCB*)Kernel::running;
	event=e;
}

void KernelEv::wait(){
	if(nit==Kernel::running){
			if(val==1)val=0;
			else {
				nit->stanje=PCB::BLOCKED;
				Kernel::dispatch();
			}
		}
}

void KernelEv::signal(){
	if(nit==NULL){val=1;}
	else if(nit->stanje!=PCB::BLOCKED){val=1;}
	else{
			nit->stanje=PCB::READY;
			Scheduler::put(nit);
	}
}
