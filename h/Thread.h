#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Process Control Block
class Thread {
	public:
		void start();
		void waitToComplete();
		virtual ~Thread();
		ID getId();
		int state();
		static ID getRunningId();
		static Thread * getThreadById(ID id);

		//drugi zadatak
		static ID fork();
		static void exit();
		static void waitForForkChildren();
		virtual Thread* clone() const;

	protected:
		friend class PCB;
		friend class Kernel;
		Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
		virtual void run() {}
	private:
		PCB* myPCB;
};

void syncPrintf(const char* c,int i=-999,int j=-999,char cr=' ');
void dispatch();
extern void tick();

#endif
