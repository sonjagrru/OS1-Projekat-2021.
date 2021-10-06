#include "kernel.h"

extern int userMain(int argc, char* argv[]);

class Main : public Thread {
	int argc;
	char **argv;
public:

	Main(int argc, char **argv) : Thread(defaultStackSize, defaultTimeSlice), argc(argc), argv(argv) {}

	int value;
	void run() { value = userMain(argc, argv); }

	~Main() { waitToComplete(); }
};

//void tick(){} ovo prebaciti u novi .cpp fajl u kom ce ti biti testovi na modif

int main(int argc, char* argv[]) {
	//pokretanje sistema jezgra
	Kernel::inic();
	//pokretanje korisnicke glavne funkcije
	Main* main = new Main(argc,argv);
	main->start();
	main->waitToComplete();
	//gasenje sistema jezgra
	Kernel::restore();
	return 0;//value;
}
