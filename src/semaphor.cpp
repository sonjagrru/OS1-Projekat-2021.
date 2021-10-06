#include "kernelse.h"
#include "kernel.h"

Semaphore::Semaphore(int init){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl = new KernelSem(init,this);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

Semaphore::~Semaphore (){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int Semaphore::wait (Time maxTimeToWait)
{
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	int val=  myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return val;
}
void Semaphore::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

int Semaphore::val()const{
	return myImpl->val();
}
