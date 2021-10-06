#include "IVTEntry.h"
#include <dos.h>
#include "stdio.h"
#include "kernelev.h"
#include "kernel.h"

IVTEntry* IVTEntry::ivt_tabela[256];

IVTEntry::IVTEntry(unsigned char ivtno,int zovem, pInterrupt nova){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	this->ivtno = ivtno;
	zovem_staru=zovem;

#ifndef BCC_BLOCK_IGNORE
	stara = getvect(ivtno);
	setvect(ivtno, nova);
#endif

	ivt_tabela[ivtno] = this;
	dogadjaj = NULL;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::set_dogadjaj(KernelEv* dogadjaj){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	this->dogadjaj = dogadjaj;

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::pozovi_staru(){
	(*stara)();
}

void IVTEntry::signal(){
	if(dogadjaj != NULL){
	    dogadjaj->signal();
	}
}

IVTEntry::~IVTEntry(){
	(*stara)();

#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(ivtno, stara);
	unlock();
#endif
}

IVTEntry* IVTEntry::objekat(unsigned char br)
{
	return IVTEntry::ivt_tabela[br];
}

