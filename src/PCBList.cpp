#include "PCBList.h"
#include <stdio.h>
#include <stdlib.h>

PCBList::PCBList(){
	first=NULL;
	last=NULL;
}

//dodavanje u listu, odrzavamo listu sortiranu po ID-u
void PCBList::add(PCB* pcb){
	Elem* novi = new Elem(pcb);
	if(!first)
	{
		first = novi;
		last = novi;
	} else{
		//provera za prvi element
		if(pcb->getId()<first->pcb->getId())
		{
			novi->next=first;
			first=novi;
		}

		if(pcb->getId()==first->pcb->getId())return;

		for(Elem* tmp=first;tmp->next!=NULL;tmp=tmp->next){

			//za slucaj da se vec nalazi u nizu nemoj je stavljati
			if(pcb->getId()==tmp->next->pcb->getId()){
				delete novi;
				return;
			}

			if(pcb->getId()<tmp->next->pcb->getId())
			{
				tmp->next=novi;
				novi->next=tmp->next;
				if(first==tmp)first=novi;
				return;
			}
		}

		last->next=novi;
		last=novi;
	}
}

void PCBList::FIFOadd(PCB* pcb)
{
	Elem* novi = new Elem(pcb);
	if(!first)
		first = novi;
	else last->next=novi;
	last=novi;
}

//ako ima obrisi, ako nema nista
void PCBList::remove(ID id){
	if(first->pcb->getId()==id)
	{
		Elem* tmp=first;
		first=first->next;
		if(!first)last=NULL;
		//delete tmp; NEMOJ DA BRISES SAM CE OBRISATI KO TREBA
	}
	for(Elem* tmp=first;id>tmp->pcb->getId()||tmp->next!=NULL;tmp=tmp->next){
		if(id==tmp->next->pcb->getId()){
						Elem* tmp2=tmp->next;
						if(last==tmp2)last=tmp;
						tmp->next=tmp2->next;
						//delete tmp2; NEMOJ DA BRISES SAM CE KO TREBA
						break;
					}
	}
}

PCB* PCBList::get(ID id){
	for(Elem* tmp=first;tmp!=NULL;tmp= tmp->next){
				if(id==tmp->pcb->getId())
					return (PCB *)tmp->pcb;
			}
	return NULL;
}

PCB* PCBList::FIFOget()
{
	Elem* tmp=first;
	if(tmp==NULL)return NULL;
	if(first!=NULL)
		first=first->next;
	if(last==tmp) last=NULL;
	PCB* tmp2=tmp->pcb;
	delete tmp;
	return tmp2;
}

PCBList::~PCBList(){
	for(Elem* tmp=first;tmp!=NULL;){
			Elem* tmp2 = tmp;
			tmp= tmp->next;
			delete tmp2;
		}
}

void PCBList::ispis()
{
	for(Elem* tmp=first;tmp!=NULL;tmp=tmp->next)
	{
		printf("Clan liste je: %d\n",tmp->pcb->getId());
	}
}
