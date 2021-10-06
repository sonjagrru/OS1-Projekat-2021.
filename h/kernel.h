#ifndef KERNEL_H_
#define KERNEL_H_

#include "Thread.h"
#include "PCB.h"
#include "PCBList.h"
#include "semlist.h"
#include "event.h"
#include "NoName.h"

#define lock() {asm {pushf; cli;}}
#define unlock() {asm{popf}}

typedef void interrupt (*pInterrupt)(...);

class Kernel{
public:
	//kernel podaci
	static volatile PCB* running;
	static Thread* mainThread;
	static volatile PCBList* lista;
	static volatile SemList* semafori;
	static volatile Event* dog;
	static volatile PCB* child;
	static volatile ID childId;
	static volatile int context_switch_on_demand;
	static volatile int cntr;
	static NoName* noName;

	// deklaracija nove prekidne rutine
	static void interrupt timer(...);

	//drugi zadatak
	static void interrupt fork();

	//za testove
	static void interrupt keyboardISR(...);

	static void dispatch();
	static void exitThread();

	//za metodu run kod PCB struktura
	static void wrapper();

	static void inic();
	static void restore();
};



#endif
