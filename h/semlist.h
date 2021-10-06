#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "kernelse.h"
#include <stdlib.h>

struct ElemSem{
	KernelSem* ks;
	ElemSem* next;

	ElemSem (KernelSem* ks = NULL){
		this->ks=ks;
		next = NULL;
	}

	void add(ElemSem* e)
	{
		e->next=this->next;
		this->next=e;
	}
};

class SemList{
public:
	ElemSem* first;
	ElemSem* last;

	SemList();
	void FIFOadd(KernelSem* ks);
	void remove(KernelSem* ks);
	void ispis();
	~SemList();
};

#endif
