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
uint ya_hay_una_puta_tarea = 0;


void sched_inicializar() {
	sched_task_t task = (sched_task_t) { 0, NULL };

	// TODO: En el typedef tiene MAX_CANT_TAREAS_VIVAS+1
	int i;
	for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		scheduler.tasks[i] = task;
	}
	scheduler.current = 8;
}


int sched_buscar_indice_tarea(uint gdt_index) {
	int i;
	for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		if (scheduler.tasks[i].gdt_index == gdt_index){
			return i;
		}
	}
    return -1;
}

// Busco la proxima tarea libre del jugador que me pasan
int sched_buscar_tarea_libre(unsigned int jugador) {
	unsigned int libre = jugador;
	unsigned int cont = 0;

	while (scheduler.tasks[libre].gdt_index != 0 && cont < MAX_CANT_TAREAS_VIVAS) {
		libre = (libre + 2) % MAX_CANT_TAREAS_VIVAS;
		cont++;
	}
	return cont < MAX_CANT_TAREAS_VIVAS ? libre : -1;	
}


perro_t* sched_tarea_actual() {
    return scheduler.tasks[scheduler.current].perro;
}


void sched_agregar_tarea(perro_t *perro) {
	int libre = sched_buscar_tarea_libre(perro->jugador->index);
	
	// Si no hay tareas libres no hago nada
	if (libre == -1)
		return;
	
	// It's alive!... IT'S ALIVE!
	perro->libre = FALSE;
	
	// Preparo nuevo stack y mapeo
	uint nuevo_stack=mmu_proxima_pagina_fisica_libre();
	mmu_mapear_pagina(nuevo_stack, perro->cr3,nuevo_stack,0,1,1); //pagina de stack para nivel 0 (int)

	//cargar un descriptor de tss y meterlo en gdt
	uint tss_new =crear_tss(GDT_OFF_KDATA_DESC, nuevo_stack, perro->cr3, 0x401000, 0x402000-12, 0x402000-12,GDT_OFF_UCODE_DESC|3,GDT_OFF_UDATA_DESC|3,GDT_OFF_UDATA_DESC|3);
	uint gdt_index=cargar_tss_en_gdt(tss_new,3);
	
	// Lo cargo en el scheduler
	scheduler.tasks[libre].perro = perro;
	scheduler.tasks[libre].gdt_index = gdt_index;
	ya_hay_una_puta_tarea = 1;
}


void sched_remover_tarea_actual(){
	sched_remover_tarea(scheduler.current);
}

void sched_remover_tarea(unsigned int index) {
	//El perro se pone vivo==0
	scheduler.tasks[index]=(sched_task_t) { 0, NULL };
}


// Busco el siguiente
uint sched_proxima_a_ejecutar() {
	unsigned int next = 0;
	perro_t* sig_perro = NULL;
	//perro_t* perro_actual = scheduler.tasks[scheduler.current].perro;

	// Recorro buscando los del otro jugador
	next = (scheduler.current + 1) % MAX_CANT_TAREAS_VIVAS;
	do {
		sig_perro = scheduler.tasks[next].perro;
		if (sig_perro != NULL && !sig_perro->libre) {
			return next;
		}
		next = (next + 2) % MAX_CANT_TAREAS_VIVAS;
	} while (next != scheduler.current + 1);


	// Recorro buscando los del mismo jugador
	next = (scheduler.current + 2) % MAX_CANT_TAREAS_VIVAS;
	while (next != scheduler.current) {
		sig_perro = scheduler.tasks[next].perro;
		if (sig_perro != NULL && !sig_perro->libre) {
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

//	breakpoint();
	// Consigo proximo perro a ejecutar
	uint proximo = sched_proxima_a_ejecutar();

	// Si el proximo es el mismo, no vuelvo a saltar
	perro_t* p = scheduler.tasks[proximo].perro;

	if (p->libre) {
		ya_hay_una_puta_tarea = 0;
		return 0;
	}
	//if ((p->id<<3) == rtr())
	if ((scheduler.tasks[scheduler.current].gdt_index <<3) == rtr())
		return 0;

	// Actualizo reloj 
	screen_actualizar_reloj_perro (p);
	screen_pintar_reloj_perro(p);

	scheduler.current = proximo;

	// Sino salto al siguiente perro
	lcr3(p->cr3);
	tarea(scheduler.tasks[proximo].gdt_index<<3);

	return 1;
}
