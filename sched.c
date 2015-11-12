/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

sched_t scheduler;
uint ya_hay_una_puta_tarea=0;
uint prox_tarea=0;


void sched_inicializar() {
	sched_task_t task = (sched_task_t) { 0, NULL };

	// TODO: En el typedef tiene MAX_CANT_TAREAS_VIVAS+1
	int i;
	for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		scheduler.tasks[i] = task;
	}
	// FIXME! AAAA
	scheduler.current = 8;
}


// TODO: Hay que hacerlo
int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


// Busco la proxima tarea libre del jugador que me pasan
// FIXME: Si se llena la cantidad de perros queda en null
int sched_buscar_tarea_libre(unsigned int jugador) {
	unsigned int libre = jugador;

	//print_dec(libre, 20,20, 0xF);
	//print_dec(scheduler.tasks[libre].gdt_index, 22,22, 0xF);
	while (scheduler.tasks[libre].gdt_index != 0) {
		breakpoint();
	//	print_dec(libre, 20,20, 0xF);
	//	print_dec(scheduler.tasks[libre].gdt_index, 22,22, 0xF);
		libre = (libre + 2) % MAX_CANT_TAREAS_VIVAS;
	}
	return libre;	
}


perro_t* sched_tarea_actual() {
    return scheduler.tasks[scheduler.current].perro;
}


void sched_agregar_tarea(perro_t *perro) {
	// TODO: Esto no debería ir acá
	perro->vivo = 1;

	uint nuevo_stack=mmu_proxima_pagina_fisica_libre();
	mmu_mapear_pagina(nuevo_stack, perro->cr3,nuevo_stack,0,1,1); //pagina de stack para nivel 0 (int)

	//cargar un descriptor de tss y meterlo en gdt
	uint tss_new =crear_tss(GDT_OFF_KDATA_DESC, nuevo_stack, perro->cr3, 0x401000, 0x402000-12, 0x402000-12,GDT_OFF_UCODE_DESC|3,GDT_OFF_UDATA_DESC|3,GDT_OFF_UDATA_DESC|3);
	uint gdt_index=cargar_tss_en_gdt(tss_new,3);
	//pasarle al scheduler la entrada de la gdt
	uint libre = sched_buscar_tarea_libre(perro->jugador->index);
	scheduler.tasks[libre].perro = perro;
	scheduler.tasks[libre].gdt_index = gdt_index;

	if (!ya_hay_una_puta_tarea){
	//	scheduler.current=libre;
		ya_hay_una_puta_tarea = 1;
	}
}

// TODO: Falta
void sched_remover_tarea(unsigned int gdt_index) {
}


//uint prox_tarea_valida(){
//	uint ret=0;
//	while(scheduler.tasks[ret].gdt_index==0)
//		ret++;
//	ya_hay_una_puta_tarea=1;
//	return ret;
//}


// Busco el siguiente
uint sched_proxima_a_ejecutar() {
	unsigned int next = 0;
	perro_t* sig_perro = NULL;
	//perro_t* perro_actual = scheduler.tasks[scheduler.current].perro;

	// Recorro buscando los del otro jugador
	next = (scheduler.current + 1) % MAX_CANT_TAREAS_VIVAS;
	do {
		//print_dec(next, 10,2, 0x8);
		sig_perro = scheduler.tasks[next].perro;
		if (sig_perro != NULL && sig_perro->vivo) {
			return next;
		}
		next = (next + 2) % MAX_CANT_TAREAS_VIVAS;
	} while (next != scheduler.current + 1);


	// Recorro buscando los del mismo jugador
	next = (scheduler.current + 2) % MAX_CANT_TAREAS_VIVAS;
	while (next != scheduler.current) {
		//print_dec(next, 10,4, 0x8);
		sig_perro = scheduler.tasks[next].perro;
		if (sig_perro != NULL && sig_perro->vivo) {
			return next;
		}
		next = (next + 2) % MAX_CANT_TAREAS_VIVAS;
	}

	// Si no llego a ningun caso entonces vuelvo al mismo
	return next;
}


ushort sched_atender_tick() {
	if( !ya_hay_una_puta_tarea )
		return 0;


	// Consigo proximo perro a ejecutar
	uint proximo = sched_proxima_a_ejecutar();
	// Si el proximo es el mismo, no vuelvo a saltar
	if ((proximo<<3) == rtr())
		return 0;

	scheduler.current = proximo;
	// Actualizo reloj 
	perro_t* p = scheduler.tasks[scheduler.current].perro;
	screen_actualizar_reloj_perro (p);
	screen_pintar_reloj_perro(p);

	breakpoint();

	// Sino salto al siguiente perro
	cr3_cargar(p->cr3);
	//Esta mierda quiere saltar al 48 cuando toco O
	tarea(scheduler.tasks[proximo].gdt_index<<3);

	return 1;
}


