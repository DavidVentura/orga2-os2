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

uint prox_tarea=1;
void sched_inicializar() {
	sched_task_t task = (sched_task_t) { 0, NULL };

	// TODO: En el typedef tiene MAX_CANT_TAREAS_VIVAS+1
	int i;
	for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		scheduler.tasks[i] = task;
	}
	scheduler.current = NULL;
}



int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre() {
	int ret=prox_tarea;
	prox_tarea=(prox_tarea+1)%MAX_CANT_TAREAS_VIVAS;
    return ret;
}



perro_t* sched_tarea_actual() {
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro) {
	uint nuevo_stack=mmu_proxima_pagina_fisica_libre();
	mmu_mapear_pagina(nuevo_stack, perro->cr3,nuevo_stack,0,1,1); //Map

	uint tss_new =crear_tss(GDT_IDX_KDATA_DESC<<3, nuevo_stack, perro->cr3, 0x401000, 0x402000-12, 0x402000-12,GDT_IDX_UCODE_DESC<<3|3,GDT_IDX_UDATA_DESC<<3|3,GDT_IDX_UDATA_DESC<<3|3);

	//cargar un descriptor de tss y meterlo en gdt
	uint gdt_index=cargar_tss_en_gdt(tss_new,3);

	//pasarle al scheduler la entrada de la gdt
	uint libre = sched_buscar_tarea_libre();

	scheduler.tasks[libre].perro = perro;
	scheduler.tasks[libre].gdt_index = gdt_index;

	scheduler.current=libre;
	cr3_cargar(perro->cr3);
	tarea_p(gdt_index<<3);
}

void sched_remover_tarea(unsigned int gdt_index) {
}


uint sched_proxima_a_ejecutar() {
	// TODO: En el struct pone MAX_CANT_TAREAS_VIVAS+1
	unsigned short next = (scheduler.current + 1) % MAX_CANT_TAREAS_VIVAS;
	short currentPlayer = scheduler.tasks[scheduler.current].perro->jugador->index;

	// Busco el siguiente
	while (next != scheduler.current && scheduler.tasks[next].gdt_index == 0) {
		if(currentPlayer != scheduler.tasks[next].perro->jugador->index) {
			//TODO: Ver si el perro está vivo
			break;
		}
		next = (next + 1) % MAX_CANT_TAREAS_VIVAS;
	}
	return next;
}


ushort sched_atender_tick() {
	if (scheduler.current==NULL)
		return 0;
//	uint prox = sched_proxima_a_ejecutar();
//	if (prox == scheduler.current)
//		return 0;
//	scheduler.current=prox;
//	tarea_p(scheduler.current<<3);
	screen_pintar_perro(scheduler.tasks[scheduler.current].perro);
	screen_pintar_reloj_perro(scheduler.tasks[scheduler.current].perro);
    return scheduler.tasks[scheduler.current].gdt_index;
//	return 1;
}
