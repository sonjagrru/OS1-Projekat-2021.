#include "Nit.h"
#include "kernel.h"
#include <iostream.h>
#include <stdlib.h>

Nit::Nit(StackSize stackSize, Time timeSlice):Thread(stackSize,timeSlice){s=NULL;}

void Nit::run(){
	for (int i =0; i < 30; ++i) {
			lock();
			cout<<"Izvrsava se nit "<<this->getId()<<" ,trenutak "<<i<<endl;
			unlock();
			if(this->getId()==3&&i==0)Kernel::dog=new Event(9);
			if(this->getId()==2&&i==15) s->signal();
			if(this->getId()==3&&i==10)Kernel::dog->wait(); //mora da pozove nit koja je kreirala

			for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
		}
	//while(1);
}

void Nit::dodajSem(Semaphore* ss)
{
	lock();
	s=ss;
	unlock();
}
