#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"
#include "semaphor.h"
#include <stdlib.h>

const Time noWait = 999;

class PCB{
private:
	Thread* _myThread;

	//parametri steka
	unsigned sp;
    unsigned ss;
    unsigned bp;
	unsigned* stack;
	StackSize _stackSize;
	void prepareStack();

	//brojac za dodelu ID
	static ID brojac;

protected:
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);

	friend class Thread;
	friend class Kernel;

	//parametri PCBa
	ID _id;
	Time _timeSlice;
	PCB* _parent;

	//drugi zadatak
	int children;
	Semaphore* waitFork;


	struct ElemPCB{
			ElemPCB* sledeci;
			PCB* moj;

			ElemPCB(PCB* p){
				moj=p;
				sledeci=NULL;
			}
		};

	ElemPCB* waitingToCompleteQueue;

public:
	enum STATE{
				    NEW, READY, FINISHED, BLOCKED, NONAME
				};
	volatile STATE stanje;
	volatile Time _maxTimeToWait;
	~PCB();

	void start();
	void run();
	void waitToComplete();
	ID getId();
	Thread* myThread();
};

#endif
