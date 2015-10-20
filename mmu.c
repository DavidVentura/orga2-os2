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

uint K_PDT;
void mmu_inicializar(){
	mmu_inicializar_dir_kernel();
}

uint mmu_inicializar_dir_kernel() {

	uint cr3=mmu_proxima_pagina_fisica_libre();
	mmu_inicializar_pagina(&cr3);
	
	
	int curPage=BASE_PAG_USER;
	while(curPage<IDENTITY_MAPPING){
		mmu_mapear_pagina(curPage, cr3, curPage, 0b11);
		curPage+=PAGE_SIZE;
	}

	return cr3;

}

void mmu_inicializar_pagina(uint* pagina){
	int i =0;
	const unsigned int zero = 0;
	while(i<1024){
		*(pagina+i)=zero; //32bit
		i++;

	}
}

uint mmu_proxima_pagina_fisica_libre(){
	ultimaPagina++;
	return ultimaPagina;
}

void mmu_mapear_pagina(uint virtual, uint cr3, uint fisica, uint attrs){

}
