#include "SCHEDULE.H"
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "kernel.h"
#include "kernelse.h"
#include "event.h"

volatile int Kernel::context_switch_on_demand=0;
volatile PCB* Kernel::running = NULL;
volatile PCB* Kernel::child = NULL;
volatile ID Kernel::childId=-1;
volatile PCBList* Kernel::lista = new PCBList();
volatile SemList* Kernel::semafori=new SemList();
Thread* Kernel::mainThread=NULL;
NoName* Kernel::noName=NULL;

//za testove
volatile Event* Kernel::dog=NULL;

ElemSem* semIter=NULL;
Elem* PCBIter=NULL;

unsigned tsp;
unsigned tss;
unsigned tbp;

// stara prekidna rutina
pInterrupt oldTimer;
pInterrupt oldTastatura;

// deklaracija nove prekidne rutine
volatile int Kernel::cntr=20;

//IVTEntry* ulaz = new IVTEntry(0x09,0, Kernel::keyboardISR);

void Kernel::inic(){
#ifndef BCC_BLOCK_IGNORE
	lock();
	//promena prekidne rutine
	oldTimer = getvect(0x08);
	setvect(0x08, timer);
#endif

	mainThread=new Thread(defaultStackSize,2);
	noName = new NoName();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

// vraca staru prekidnu rutinu
void Kernel::restore(){
#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(0x08, oldTimer);
	delete noName;
	unlock();
#endif
}


void interrupt Kernel::timer(...){
	if (!context_switch_on_demand) {
		cntr--;
		(*oldTimer)();
		tick();

		if(semafori->first!=NULL){
			for(semIter= semafori->first;semIter!=NULL;semIter=semIter->next)
			{
				for(PCBIter = semIter->ks->lista_cekanja->first;PCBIter!=NULL;PCBIter=PCBIter->next)
				{
					if(PCBIter->pcb->_maxTimeToWait!=noWait&&PCBIter->pcb->_maxTimeToWait!=0)//!=NoWait
					{
						--PCBIter->pcb->_maxTimeToWait;//radi regularno ulazi do ovde
						if(PCBIter->pcb->_maxTimeToWait==0)
						{
							semIter->ks->lista_cekanja->remove(PCBIter->pcb->_id);
							PCBIter->pcb->stanje=PCB::READY;
							Scheduler::put(PCBIter->pcb);
						}
					}
				}
			}
		}
	}

	if (cntr == 0 || context_switch_on_demand) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		if(running->stanje==PCB::READY)
			Scheduler::put((PCB*)running);

		// scheduler
		running = Scheduler::get();// getNextPCBToExecute();
		if(running==NULL){running=noName->myPCB;}

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		cntr = running->_timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif
	}

	context_switch_on_demand = 0;
}

unsigned offsetParent;
unsigned offsetChild;
unsigned *stackPointerParent;
unsigned *stackPointerChild;

unsigned tmpbp;
unsigned tmpss;

void interrupt Kernel::fork(){
		delete[] child->stack;
		size_t size = running->_stackSize;
		child->stack = new unsigned[size];
		child->_stackSize = size;
		child->_timeSlice = running->_timeSlice;
		child->_parent = (PCB*)running;
		child->_parent->children++;
		size *= sizeof(unsigned);

		//kopiraj stack
		memcpy((void*)child->stack, (void*)running->stack, size);

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tmpss, ss
			mov tmpbp, bp
		}

		child->ss = FP_SEG(child->stack);
		offsetParent = FP_OFF(running->stack);
		offsetChild = FP_OFF(child->stack);

#endif
		child->bp = child->sp = tmpbp - offsetParent + offsetChild; //BP sa kraja

		offsetParent = tmpbp;
		offsetChild = child->sp;

		while (1) { //PREPRAVLJANJE SVIH BP NA STEKU ZA DETE!
#ifndef BCC_BLOCK_IGNORE
			stackPointerParent = (unsigned*)MK_FP(tmpss, offsetParent);
			stackPointerChild = (unsigned*)MK_FP(child->ss, offsetChild);
#endif
			if (*stackPointerParent == 0) {//kada smo stigli na kraj steka
				*stackPointerChild = 0;
				break;
			}
			*stackPointerChild = *stackPointerParent - offsetParent + offsetChild;
			offsetParent = *stackPointerParent;
			offsetChild = *stackPointerChild;
		}//probelm je kod svega ovoga!

		child->_myThread->start();
}

// sinhrona promena konteksta
void Kernel::dispatch(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	//cout<<"Uso u dispatch"<<endl;
	context_switch_on_demand = 1;
	timer();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Kernel::exitThread(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	running->stanje=PCB::FINISHED;
	cout<<"Nit "<<running->_id<<" zavrsila!"<<endl;
		for(ElemPCB* tmp= running->waitingToCompleteQueue;tmp!=NULL;tmp=tmp->sledeci){
					running->waitingToCompleteQueue->moj->stanje=PCB::READY;
					Scheduler::put((PCB*)running->waitingToCompleteQueue->moj);
			}
		running->waitingToCompleteQueue=NULL;
		//unlock();
	if(running->_parent!=NULL){
		running->_parent->children--;
		running->_parent->waitFork->signal();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

	dispatch();
}

void Kernel::wrapper(){
	running->run();
	exitThread();
}
