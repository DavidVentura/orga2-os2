/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

uint ultimaPagina = 0;

#define BASE_PAG_USER 0x100000
#define IDENTITY_MAPPING 0x3FFFFF


void mmu_inicializar(){
	mmu_inicializar_dir_kernel();
	cosa_loca_paginacion();
}

uint mmu_inicializar_dir_kernel() {
	pde* K_PDT=(pde*)0x27000; 
	cr3_cargar(&K_PDT);
	cr3_cargar(0x27000000);


	int curPage=0;
	pde pde_entry;
	pde_entry.dir=0x0;
	pde_entry.todos_los_flags_cero=0;
	pde_entry.us=0;
	pde_entry.rw=1;
	pde_entry.p=0; //Ninguna presente salvo la primera.

	while(curPage<1024){
		K_PDT[curPage]=pde_entry;
		curPage++;
	}
	breakpoint();
	K_PDT[0].p=1; //Presente
	K_PDT[0].dir=BASE_PAG_USER>>20;//(mmu_proxima_pagina_fisica_libre()>>20);
	//todas las pag son 01 0000000000000.... 1
	//pero la primeraes 11 000000000000..... 1

	int dir = K_PDT[0].dir;
	dir=dir<<12;
	pte* table = (pte*)dir;

	pte pt_entry;
	pt_entry.todos_los_flags_cero=0;
	pt_entry.us=0;
	pt_entry.rw=1;
	pt_entry.p=1;

	curPage=0;
	while(curPage<1){
		pt_entry.dir=curPage*0x1000;
		table[curPage]=pt_entry;
		curPage++;
	}
	breakpoint();

	return 0;

}

void mmu_inicializar_pagina(uint* pagina){
	int i =0;
	while(i<1024){
		*(pagina+i)=0x0; //32bit
		i++;
	}
}

uint mmu_proxima_pagina_fisica_libre(){
	breakpoint();
	int ret=BASE_PAG_USER+(ultimaPagina*0x1000);
	ultimaPagina++;
	return ret;
}

void mmu_mapear_pagina(uint virtual, uint cr3, uint fisica, uint attrs){
	/*
	uint PD_OFFSET=(virtual & 0xFFC00000 ) >> 22;
	uint PT_OFFSET=(virtual & 0x003FF000 ) >> 12;
	uint PG_OFFSET=(virtual & 0x00000FFF );

	//&(cr3+PD_OFFSET)=
	*/
}
