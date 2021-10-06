#include "kernel.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <dos.h>

ID PCB::brojac=0;

PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	_id= ++brojac;
	_myThread= myThread;
	_stackSize=stackSize/sizeof(unsigned);
	_timeSlice=timeSlice;
	children=0;
	waitFork= new Semaphore(0);
	_maxTimeToWait=noWait;
	waitingToCompleteQueue=NULL;
	Kernel::lista->add(this);
	_parent=NULL;

	prepareStack();

	cout<<"Nit "<<_id<<" kreirana!"<<endl;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	delete waitingToCompleteQueue;
	delete[] stack;
	delete waitFork;
	Kernel::lista->remove(_id);
	cout<<"Nit "<<_id<<" unistena!"<<endl;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

ID PCB::getId(){
	return this->_id;
}

void PCB::prepareStack()
{
	unsigned n = this->_stackSize;
	stack = new unsigned[n];


	if(_id>1){
		stack[n-1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
		stack[n-2] = FP_SEG(&(Kernel::wrapper));
		stack[n-3] = FP_OFF(&(Kernel::wrapper));
		stack[n-12] = 0;

		this->sp = FP_OFF(stack+n-12);
		this->ss = FP_SEG(stack+n-12);
#endif
		this->bp = sp;
		if(_id>2)stanje = PCB::NEW;
		else stanje=PCB::NONAME;
	}
	else{
		Kernel::running = this;
		stanje = PCB::READY;
	}
}

Thread* PCB::myThread()
{
	return this->_myThread;
}

void PCB::run()
{
	this->_myThread->run();
}

void PCB::start()
{
	if(stanje==PCB::NEW)
	{
#ifndef BCC_BLOCK_IGNORE
		lock();
#endif

		stanje=PCB::READY;
		Scheduler::put(this);

#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
	}
}

void PCB::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	if(stanje!=PCB::FINISHED){
		Kernel::running->stanje=PCB::BLOCKED;
		ElemPCB* e=new ElemPCB((PCB*)Kernel::running);
		e->sledeci=waitingToCompleteQueue;
		waitingToCompleteQueue=e;
	}

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	Kernel::dispatch();
}
