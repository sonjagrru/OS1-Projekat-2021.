#include "semlist.h"
#include "kernelse.h"
#include <stdio.h>

SemList::SemList(){
	first=NULL;
	last=NULL;
}

void SemList::FIFOadd(KernelSem* ks)
{
	ElemSem* novi = new ElemSem(ks);
	if(!first)
		first = novi;
	else last->next=novi;
	last=novi;
}

SemList::~SemList(){
	for(ElemSem* tmp=first;tmp!=NULL;){
			ElemSem* tmp2 = tmp;
			tmp= tmp->next;
			delete tmp2;
		}
}

void SemList::remove(KernelSem* ks){
	if(first->ks==ks)
		{
			ElemSem* tmp=first;
			first=first->next;
			if(!first)last=NULL;
			//delete tmp;
		}
		for(ElemSem* tmp=first;ks!=tmp->ks&&tmp->next!=NULL;tmp=tmp->next){
			if(ks==tmp->next->ks){
							ElemSem* tmp2=tmp->next;
							if(last==tmp2)last=tmp;
							tmp->next=tmp2->next;
							//delete tmp2;
							break;
						}
		}
}

void SemList::ispis()
{
	for(ElemSem* tmp=first;tmp!=NULL;tmp=tmp->next)
	{
		printf("Clan liste je: %d\n",tmp->ks->value);
	}
}
