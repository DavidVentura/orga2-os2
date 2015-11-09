/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"



tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

uint INICIO_TSS=0x180000;

void tss_inicializar() {
	uint tss_inicial=crear_tss(GDT_IDX_KDATA_DESC<<3, KSTACK, KERNEL_PDIR, 0x16000, KSTACK, KSTACK,GDT_IDX_KCODE_DESC<<3,GDT_IDX_KDATA_DESC<<3,GDT_IDX_KDATA_DESC<<3);
	uint tss_idle   =crear_tss(GDT_IDX_KDATA_DESC<<3, KSTACK, KERNEL_PDIR, 0x16000, KSTACK, KSTACK,GDT_IDX_KCODE_DESC<<3,GDT_IDX_KDATA_DESC<<3,GDT_IDX_KDATA_DESC<<3);
	//Magic number?

	cargar_tss_en_gdt(tss_inicial,0);
	cargar_tss_en_gdt(tss_idle,0);
	ltr(DTSS_INIT<<3);

}

//eip es virtual
uint crear_tss(uint ss0, uint esp0, uint cr3, uint eip, uint esp, uint ebp, uint cs, uint ds, uint ss){
	uint p =proxima_tss_libre();
	tss* entry=(tss*)p;
	entry->ss0 = ss0;
	entry->esp0 = esp0;
	entry->cr3=cr3; 
	entry->eflags=0x202;
	entry->eip=eip;
	entry->esp=esp;
	entry->ebp=ebp;
	entry->cs=cs;

	entry->ds=ds;
	entry->es=ds;
	entry->fs=ds;
	entry->gs=ds;

	entry->ss=ss;

	return p;
}

uint proxima_tss_libre(){
	uint ret=INICIO_TSS;
	INICIO_TSS+=sizeof(tss);
	return ret;
}

//Dame la posicion de memoria de la base 
uint cargar_tss_en_gdt(uint base, char dpl) {
	gdt_entry g;
	
	g.limit_0_15=0x67;
	g.base_0_15=(base & 0xFFFF);
	g.base_23_16=(base & 0xFF0000) >>16;
	g.type=0b1001;
	g.s=0;
	g.dpl=dpl;
	g.p=1;
	g.limit_16_19=0;
	g.l=0;
	g.db=0;
	g.g=0;
	g.base_31_24=(base >> 24);

	uint p=prox_gdt_libre();
	gdt[p]=g;
	return p;
}
