/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"


tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

const uint INICIO_TSS=0x180000; //FIXME
void tss_inicializar() {

	cargar_tss_en_gdt((uint)&tss_idle,0);
	cargar_tss_en_gdt((uint)&tss_inicial,0);

	completar_tss(&tss_idle,GDT_IDX_KDATA_DESC<<3, 0x2700, 0x27000, 0x202, 0x16000, 0x2700, 0x2700,GDT_IDX_KCODE_DESC<<3,GDT_IDX_KDATA_DESC<<3,GDT_IDX_KDATA_DESC<<3);
	//Magic number?

	tss_inicial=tss_idle;
	ltr(DTSS_INIT<<3);

}


void completar_tss(tss* entry, uint ss0, uint esp0, uint cr3, uint eflags, uint eip, uint esp, uint ebp, uint cs, uint ds, uint ss){
	tss_idle.ss0 = ss0;
	tss_idle.esp0 = esp0;
	tss_idle.cr3=cr3; 
	tss_idle.eflags=eflags;
	tss_idle.eip=eip;
	tss_idle.esp=esp;
	tss_idle.ebp=ebp;
	tss_idle.cs=cs;
	tss_idle.ds=ds;
	tss_idle.ss=ss;

}


//Dame la posicion de memoria de la base 
void cargar_tss_en_gdt(uint t_entry, char dpl) {
	//uint base = (uint)&t_entry;
	uint base = t_entry;
	gdt_entry g;
	
	g.limit_0_15=0x67;
	g.base_0_15=(base & 0xFFFF);
	g.base_23_16=(base & 0xFF0000) >>15;
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
}
