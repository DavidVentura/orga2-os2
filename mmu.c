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
uint id_ultima_tarea=0;

#define BASE_PAG_USER 0x100000
#define IDENTITY_MAPPING 0x3FFFFF
#define KERNEL_PDIR	0x27000	
#define USER_PDIR 0x3FE000
//USER_PDIR es la ante-ultima pagina posible de pag libres

void mmu_inicializar(){
	mmu_inicializar_dir_kernel();
	paginacion_activar();
	mmu_mapear_pagina(0x200000,KERNEL_PDIR,0xB8000,1,1);
}

void mmu_inicializar_dir_kernel() {
	cr3_cargar(KERNEL_PDIR);
	const int PAGE_TABLE=0x100000; //mmu_proxima_pagina_fisica_libre();

	pde* pdir = (pde*)KERNEL_PDIR;
	inicializar_pdir(pdir, 0,1,0);

	(*pdir).dir=PAGE_TABLE>>12;
	(*pdir).p=1;

	int curPage=0;
	pte* table = (pte*)PAGE_TABLE;

	pte pt_entry;
	pt_entry.todos_los_flags_cero=0;
	pt_entry.us=0;
	pt_entry.rw=1;
	pt_entry.p=1;

	while(curPage<1024){
		pt_entry.dir=(curPage*0x1000)>>12;
		*table=pt_entry;
		table++;
		curPage++;
	}
}

void mmu_inicializar_pagina(uint* pagina){ //No testeado
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
	uint PD_OFFSET=virtual >> 22;
	uint PT_OFFSET=(virtual & 0x003FF000 ) >> 12;

	pde* PDT=(pde*)(cr3 & 0xFFFFF000); 
	pte* PT=(pte*)(PDT[PD_OFFSET].dir<<12);
	PT[PT_OFFSET].p=p;
	PT[PT_OFFSET].rw=rw;
	PT[PT_OFFSET].dir=fisica&0xFFFFF000;
	tlbflush();

}
void mmu_copiar_pagina(uint src, uint dst){ //No testeado
	uint  i =0;
	uint* s =(uint*)src;
	uint* d =(uint*)dst;
	for(i=0;i<1024;i++)
		*(d++)=*(s++);
}

void mmu_unmapear_pagina(uint virtual, uint cr3){
	uint PD_OFFSET=(virtual & 0xFFC00000 ) >> 22;
	uint PT_OFFSET=(virtual & 0x003FF000 ) >> 12;

	pde* PDT=(pde*)(cr3); 
	pte* PT=(pte*)(PDT[PD_OFFSET].dir<<12);
	PT[PT_OFFSET].p=0;
	tlbflush();
}
uint id_nueva_tarea(){
	return id_ultima_tarea++;
}
uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo){
	perro->id=id_nueva_tarea();
	perro->tipo=index_tipo;
	perro->jugador->index=index_jugador;
	pde* pdir = (pde*)mmu_proxima_pagina_fisica_libre();
	pte* ptab = (pte*)mmu_proxima_pagina_fisica_libre();
	inicializar_pdir(pdir, 0,0,0);
	inicializar_ptab(ptab,0,0,0);

	return 0;
}


void inicializar_pdir(pde* base,uint us, uint rw, uint p){
	int curPage=0;
	pde pde_entry;
	pde_entry.todos_los_flags_cero=0;
	pde_entry.us=us;
	pde_entry.rw=rw;
	pde_entry.p=p; 
	pde_entry.dir=0xFFFFF;

	while(curPage<1024){
		*base=pde_entry;
		base++;
		curPage++;
	}
	base-=1024;

}

void inicializar_ptab(pte* base, uint us, uint rw, uint p){

	int curPage=0;
	pte pt_entry;
	pt_entry.todos_los_flags_cero=0;
	pt_entry.us=0;
	pt_entry.rw=1;
	pt_entry.p=1;

	while(curPage<1024){
		pt_entry.dir=0xFFFFF;
		*base=pt_entry;
		base++;
		curPage++;
	}
}
