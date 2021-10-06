#include "kernel.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include <iostream.h>


void Thread::start(){
	myPCB->start();
}
void Thread::waitToComplete(){
	myPCB->waitToComplete();
}

Thread::~Thread(){
	delete myPCB;
}
ID Thread::getId(){
	return myPCB->getId();
}


int Thread::state(){
	return myPCB->stanje;
}

ID Thread::getRunningId(){
	ID val;
	lock();
	val=Kernel::running->getId();
	unlock();
	return val;
}

//vrati nit iz niza svih niti
Thread* Thread::getThreadById(ID id){
	PCB* tmp = Kernel::lista->get(id);
	if(tmp) return tmp->_myThread;
	else return NULL;
}

//kreiraj PCB za nit
Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB= new PCB(this,stackSize,timeSlice);
}

void dispatch(){
	Kernel::dispatch();
}

//za testove
void syncPrintf(const char* c,int i,int j, char cr){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(i!=-999&&j!=-999)
		printf(c,i,j,cr);
	else if(i!=-999)
		printf(c,i);
	else
		printf(c);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Thread* Thread::clone()const{return new Thread();}

void Thread::waitForForkChildren(){
	while(Kernel::running->children>0)
	{
		Kernel::running->waitFork->wait(0);
	}
}

ID Thread::fork(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	cout << "FORK"<< endl;
	Thread *child = NULL;
	child = Kernel::running->_myThread->clone();

	if (child == NULL) {
#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
		return -1;
	}

	cout << "Kreirao dete id: " << child->myPCB->_id << endl;

	int childNum = child->myPCB->_id;
	Kernel::childId = child->myPCB->_id;

	Kernel::child = child->myPCB;
	Kernel::fork();

	cout << "Izasao iz kernel::forka id: " << Kernel::running->_id << endl;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	if (Kernel::running->_id == childNum) return 0;//vraca 0 u kontekstu deteta
	else return childNum;//vraca id deteta u kontekstu roditelja*/
}

void Thread::exit() {
	Kernel::exitThread();
}
