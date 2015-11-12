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
uint CODIGO_PERROS[4] = { 0x10000, 0x11000, 0x12000, 0x13000 };

//tomo la primer pag para kernel
#define K_PAGE_TABLE 0x100000
#define BASE_PAG_USER 0x101000
#define IDENTITY_MAPPING 0x3FFFFF

void mmu_inicializar(){
	lcr3(KERNEL_PDIR);
	mmu_inicializar_dir_kernel();
	paginacion_activar();
	mmu_mapear_pagina(0x200000,KERNEL_PDIR,0xB800,0,1,1);
}

void mmu_inicializar_dir_kernel() {
	pde* pdir = (pde*)KERNEL_PDIR;
	inicializar_pdir(pdir, 0,1,0);

	(*pdir).dir=K_PAGE_TABLE>>12;
	(*pdir).p=1;

	int curPage=0;
	pte* table = (pte*)K_PAGE_TABLE;

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

void mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo, uint cuchax, uint cuchay){
	//Si index_jugador>1 se va todo a la mierda
	pde* pdir = (pde*)mmu_proxima_pagina_fisica_libre();
	pte* ptab = (pte*)mmu_proxima_pagina_fisica_libre();
	inicializar_pdir(pdir,1,1,0);
	inicializar_ptab(ptab,1,0,0);

	(*pdir).dir=K_PAGE_TABLE>>12;
	(*pdir).p=1;
	pdir++;
	(*pdir).dir=((uint)ptab)>>12;
	(*pdir).p=1;

	uint dircucha=mmu_xy2virtual(cuchax,cuchay);
	uint dircuchaf=mmu_xy2fisica(cuchax,cuchay);

	uint dircod=CODIGO_PERROS[index_jugador*2+index_tipo];
	mmu_mapear_pagina(0x401000, (uint)pdir, dircod,1,1,1); 
	mmu_mapear_pagina(dircucha, (uint)pdir, dircuchaf,1,1,1);
	mmu_mapear_pagina(dircucha, (uint)KERNEL_PDIR, dircuchaf,0,1,1); //Mapeo temporalmente la cucha para poder escribirla

	mmu_copiar_pagina(dircod,dircucha);
	mmu_unmapear_pagina(dircucha,KERNEL_PDIR);

	//El primer perro lo deja ok, los siguientes rompen dest
	mmu_mapear_pagina(0x400000, (uint)pdir, 0x300000+0x1000*index_jugador,1,1,1); //FIXME a donde va esto? compartida

	perro->cr3=(uint) pdir;
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

void mmu_mapear_pagina(uint virtual, uint cr3, uint fisica, uint us, uint rw, uint p){

	uint PD_OFFSET=virtual >> 22;
	uint PT_OFFSET=(virtual & 0x003FF000 ) >> 12;

	pde* PDT=(pde*)(cr3 & 0xFFFFF000); 
	pte* PT=(pte*)(PDT[PD_OFFSET].dir<<12);
	if ((uint)PT==0xFFFFF000){ //Nunca hice el ptab
		PT=(pte*)mmu_proxima_pagina_fisica_libre();
		inicializar_ptab(PT,us,rw,0);
		PDT[PD_OFFSET].todos_los_flags_cero=0;
		PDT[PD_OFFSET].us=us;
		PDT[PD_OFFSET].rw=rw;
		PDT[PD_OFFSET].p=0; 
		PDT[PD_OFFSET].dir=((uint)PT)>>12;
		PDT[PD_OFFSET].p=1; 
	}

	PT[PT_OFFSET].us=us;
	PT[PT_OFFSET].rw=rw;
	PT[PT_OFFSET].p=p;
	PT[PT_OFFSET].dir=(fisica&0xFFFFF000)>>12;
	tlbflush();

}
//Fisica a fisica
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

void inicializar_pdir(pde* base,uint us, uint rw, uint p){
	int curPage=0;
	pde pde_entry;
	pde_entry.todos_los_flags_cero=0;
	pde_entry.us=us;
	pde_entry.rw=rw;
	pde_entry.p=p; 
	pde_entry.dir=0xFFFFF; //Lee algo no inicializado al mapear durante int70

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
	pt_entry.us=us;
	pt_entry.rw=rw;
	pt_entry.p=p;

	while(curPage<1024){
		pt_entry.dir=0xFFFFF;
		*base=pt_entry;
		base++;
		curPage++;
	}
}

uint mmu_xy2virtual(uint x, uint y){
	return 0x800000+(y*MAPA_ANCHO+x)*0x1000;
}


uint mmu_xy2fisica(uint x, uint y){
	return 0x500000+(y*MAPA_ANCHO+x)*0x1000;
}
