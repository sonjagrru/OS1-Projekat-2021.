#include "kernelev.h"
#include "kernel.h"
#include <iostream.h>

Event::Event(IVTNo ivtNo){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	myImpl=new KernelEv(ivtNo,this);
	IVTEntry::objekat(ivtNo)->set_dogadjaj(myImpl);

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	myImpl->wait();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif

	myImpl->signal();

#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
