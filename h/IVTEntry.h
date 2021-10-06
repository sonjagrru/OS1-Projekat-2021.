#ifndef IVTENTRY_H_
#define IVTENTRY_H_

class KernelEv;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
private:
	unsigned char ivtno;
	pInterrupt stara;
	KernelEv* dogadjaj;
	int zovem_staru;
public:
	static IVTEntry* ivt_tabela[];
	static IVTEntry* objekat(unsigned char br);

	IVTEntry(unsigned char ivtno,int zovem, pInterrupt nova);

	void pozovi_staru();
	void signal();
	void set_dogadjaj(KernelEv* dogadjaj);

	~IVTEntry();
};


#endif
