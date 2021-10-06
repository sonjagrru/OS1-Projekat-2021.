#ifndef EVENT_H_
#define EVENT_H_

#include "IVTEntry.h"

#define PREPAREENTRY(brul,pozvatiStaru)\
	void interrupt rutina##brul(...){\
		IVTEntry *ulaz=IVTEntry::objekat(brul);\
		ulaz->signal();\
		if(pozvatiStaru) ulaz->pozovi_staru();\
	}\
	IVTEntry entry##brul(brul,pozvatiStaru,&rutina##brul);

typedef unsigned char IVTNo;
class KernelEv;
class Event {
	public:
		Event(IVTNo ivtNo);
		~Event();
		void wait();
	protected:
		friend class KernelEv;
		void signal(); // can call KernelEv
	private:
		KernelEv* myImpl;
};

#endif
