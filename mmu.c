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
#define KERNEL_PDIR	0x27000	

void mmu_inicializar(){
	mmu_inicializar_dir_kernel();
	paginacion_activar();
	mmu_mapear_pagina(0x200000,KERNEL_PDIR,0xB8000,1,1);
}

void mmu_inicializar_dir_kernel() {
	pde* K_PDT=(pde*)KERNEL_PDIR; 
	cr3_cargar(KERNEL_PDIR);
	const int PAGE_TABLE=0x100000; //mmu_proxima_pagina_fisica_libre();

	int curPage=0;
	pde pde_entry;
	pde_entry.todos_los_flags_cero=0;
	pde_entry.us=0;
	pde_entry.rw=1;
	pde_entry.p=1; //Ninguna presente salvo la primera.
	pde_entry.dir=PAGE_TABLE>>12; 

	*K_PDT=pde_entry;
	K_PDT++;

	pde_entry.dir=0xFFFFF;
	pde_entry.p=0; //Ninguna presente salvo la primera.
	while(curPage<1023){
		*K_PDT=pde_entry;
		K_PDT++;
		curPage++;
	}
	
	pte* table = (pte*)PAGE_TABLE;

	pte pt_entry;
	pt_entry.todos_los_flags_cero=0;
	pt_entry.us=0;
	pt_entry.rw=1;
	pt_entry.p=1;

	curPage=0;
	while(curPage<1024){
		pt_entry.dir=(curPage*0x1000)>>12;
		*table=pt_entry;
		table++;
		curPage++;
	}
}

void mmu_inicializar_pagina(uint* pagina){
	int i =0;
	for(i=0;i<1024;i++)
		*(pagina+i)=0x0;
}

uint mmu_proxima_pagina_fisica_libre(){
	int ret=BASE_PAG_USER+(ultimaPagina*0x1000);
	ultimaPagina++;
	return ret;
}

void mmu_mapear_pagina(uint virtual, uint cr3, uint fisica, uint rw, uint p){
	uint PD_OFFSET=(virtual & 0xFFC00000 ) >> 22;
	uint PT_OFFSET=(virtual & 0x003FF000 ) >> 12;

	pde* PDT=(pde*)(cr3); 
	pte* PT=(pte*)(PDT[PD_OFFSET].dir<<12);
	PT[PT_OFFSET].p=p;
	PT[PT_OFFSET].rw=rw;
	PT[PT_OFFSET].dir=fisica&0xFFFFF000;

}
