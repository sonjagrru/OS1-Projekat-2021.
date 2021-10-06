#ifndef PCBLIST_H_
#define PCBLIST_H_
#include "PCB.h"
#include <stdlib.h>

struct Elem{
	PCB* pcb;
	Elem* next;

	Elem (PCB* pcb = NULL){
		this->pcb=pcb;
		next = NULL;
	}

	void add(Elem* e)
	{
		e->next=this->next;
		this->next=e;
	}
};

class PCBList{
public:
	Elem* first;
	Elem* last;

	PCBList();
	void add(PCB* pcb);
	void FIFOadd(PCB* pcb);
	PCB* FIFOget();
	PCB* get(ID id);
	void remove(ID id);
	void ispis();
	~PCBList();
};


#endif
