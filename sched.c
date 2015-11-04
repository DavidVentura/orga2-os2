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


uint sched_proxima_a_ejecutar()
{
    return MAX_CANT_TAREAS_VIVAS;
}


ushort sched_atender_tick()
{
    return scheduler.tasks[scheduler.current].gdt_index;
}


