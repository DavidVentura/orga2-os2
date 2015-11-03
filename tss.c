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

extern void tarea_idle();

const uint INICIO_TSS=0x180000; //FIXME
const uint DTSS_IDLE=14; //defines.h?
void tss_inicializar() {

	uint base = (uint)&tss_idle;
	uint limit = 0x67; 
	
	gdt_entry g;
	g.limit_0_15=limit&0xFFFF;
	g.base_0_15	=(base & 0x7FFF);
	g.base_23_16=(base & 0xFF8000) >>15;
	g.type=0b1001;
	g.s=0;
	g.dpl=0;
	g.p=1;
	g.limit_16_19=(limit&0xF0000)>>16;
	g.l=0;
	g.db=0;
	g.g=0;
	g.base_31_24=(base >> 24);

	gdt[DTSS_IDLE]=g;

	base = (uint)&tss_inicial;
	g.limit_0_15= limit&0xFFFF;
	g.limit_16_19=(limit&0xF0000)>>16;
	g.base_0_15=base & 0x7FFF;
	g.base_23_16=(base & 0xFF8000) >>15;
	g.base_31_24=(base >> 24);
	gdt[DTSS_IDLE-1]=g;


	tss_idle.ss0 = GDT_IDX_KDATA_DESC<<3;
	tss_idle.esp0 = 0x2700; //KERNEL_STACK;
	tss_idle.cr3=0x27000;
	tss_idle.eflags=0x202;
	tss_idle.eip=0x16000; //IDLE_CODE
	tss_idle.esp=0x2700; //FIXME: Kernel stackpointer
	tss_idle.ebp=0x2700; //FIXME: Kernel stackbase
	tss_idle.cs=GDT_IDX_KCODE_DESC<<3;
	tss_idle.ds=GDT_IDX_KDATA_DESC<<3;
	tss_idle.ss=GDT_IDX_KDATA_DESC<<3;

	tss_inicial=tss_idle;
	ltr(13<<3);

	tarea(DTSS_IDLE);
}
