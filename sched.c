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

void sched_inicializar() {
	sched_task_t task = (sched_task_t) { 0, NULL };

	// TODO: En el typedef tiene MAX_CANT_TAREAS_VIVAS+1
	for (int i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		scheduler.tasks[i] = task;
	}
	scheduler.current = NULL;
}



int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre()
{
	int i = 0;

    return i;
}



perro_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro)
{
}

void sched_remover_tarea(unsigned int gdt_index)
{
}


uint sched_proxima_a_ejecutar() {
	// TODO: En el struct pone MAX_CANT_TAREAS_VIVAS+1
	unsigned short next = (scheduler.current + 1) % MAX_CANT_TAREAS_VIVAS;
	short currentPlayer = scheduler.tasks[scheduler.current].perro->jugador->index;

	// Busco el siguiente
	// TODO: NO
	while (next != scheduler.current && scheduler.tasks[next].gdt_index == 0) {
		if(currentPlayer != scheduler.tasks[next].perro->jugador->index) {
			//TODO: Ver si el perro está vivo
			// Break
		}
		next = (next + 1) % MAX_CANT_TAREAS_VIVAS;
	}
	return next;
}


ushort sched_atender_tick()
{
    return scheduler.tasks[scheduler.current].gdt_index;
}


