#include "NoName.h"

NoName::NoName(StackSize stack,Time time):Thread(stack,time){}

void NoName::run(){
	while(1);
}
