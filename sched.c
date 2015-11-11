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
	mmu_mapear_pagina(nuevo_stack, perro->cr3,nuevo_stack,0,1,1); //pagina de stack para nivel 0 (int)

	//cargar un descriptor de tss y meterlo en gdt
	uint tss_new =crear_tss(GDT_OFF_KDATA_DESC, nuevo_stack, perro->cr3, 0x401000, 0x402000-12, 0x402000-12,GDT_OFF_UCODE_DESC|3,GDT_OFF_UDATA_DESC|3,GDT_OFF_UDATA_DESC|3);
	uint gdt_index=cargar_tss_en_gdt(tss_new,3);

	//pasarle al scheduler la entrada de la gdt
	uint libre = sched_buscar_tarea_libre();

	scheduler.tasks[libre].perro = perro;
	scheduler.tasks[libre].gdt_index = gdt_index;

	scheduler.current=libre;
	ya_hay_una_puta_tarea=1;
}

void sched_remover_tarea(unsigned int gdt_index) {
}

uint prox_tarea_valida(){
	uint ret=0;
	while(scheduler.tasks[ret].gdt_index==0)
		ret++;
	ya_hay_una_puta_tarea=1;
	return ret;
}
uint sched_proxima_a_ejecutar() {
	// TODO: En el struct pone MAX_CANT_TAREAS_VIVAS+1
	if (ya_hay_una_puta_tarea==0)
		return prox_tarea_valida();

	unsigned short next = (scheduler.current + 1) % MAX_CANT_TAREAS_VIVAS;
	short currentPlayer = scheduler.tasks[scheduler.current].perro->jugador->index;

	// Busco el siguiente
	while (next != scheduler.current && scheduler.tasks[next].gdt_index == 0) {
		if(currentPlayer != scheduler.tasks[next].perro->jugador->index) {
			if (scheduler.tasks[next].perro->vivo==TRUE){

			}
			//TODO: Ver si el perro está vivo
			break;
		}
		next = (next + 1) % MAX_CANT_TAREAS_VIVAS;
		if (next==scheduler.current){
		//	HALT_AND_CATCH_FIRE();
			return 0;
		}
	}
	return next;
}


ushort sched_atender_tick() {
//	uint prox = sched_proxima_a_ejecutar();
//	if (prox == scheduler.current)
//		return 0;
//	scheduler.current=prox;
//	tarea(scheduler.current<<3);
	if( ya_hay_una_puta_tarea==0)
		return 0;

	perro_t* p_act=scheduler.tasks[scheduler.current].perro;
	screen_actualizar_reloj_perro (p_act);
	screen_pintar_reloj_perro(p_act);


	uint proximo=sched_proxima_a_ejecutar();
	if (proximo==scheduler.current)
		return 0;
	perro_t* p=scheduler.tasks[proximo].perro;
	cr3_cargar(p->cr3);
	tarea(scheduler.tasks[scheduler.current].gdt_index<<3);
	return 1;
}
