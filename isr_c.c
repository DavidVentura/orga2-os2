#include "isr.h"


void interrupcion_atender(unsigned int num, unsigned int eflags, unsigned short cs, unsigned int eip, unsigned short errorCd) {
	switch (num){
		case 32:
			screen_actualizar_reloj_global();
			fin_intr_pic1();
			break;
		case 33:
			teclado_atender();
			fin_intr_pic1();
			break;
		case 70:
			int70();
		default:
			print("ERROR: ", 0, 0, 0xF);
			print_dec(num, 7, 0, 0x4);
			break;
	}

}

void int70() {
	uint tipo, dir;
	uint xOrig, yOrig;
	__asm __volatile("movl %%eax,%0" : "=r" (tipo));
	__asm __volatile("movl %%ecx,%0" : "=r" (dir));

	// Consigo el perro actual
	perro_t* aPerro = scheduler.tasks[scheduler.current].perro;
	
	switch(tipo) {
		case 0x1: //Moverse
			xOrig = aPerro->x;
			yOrig = aPerro->y;
			game_perro_mover(aPerro, dir);
			if (xOrig != aPerro->x || yOrig != aPerro->y) {
				mmu_copiar_pagina(mmu_xy2fisica(xOrig, yOrig), mmu_xy2fisica(aPerro->x, aPerro->y));
				mmu_mapear_pagina(mmu_xy2virtual(aPerro->x, aPerro->y), rcr3(), mmu_xy2fisica(aPerro->x, aPerro->y), 1, 1);
			}
			break;
		case 0x2: //Cavar
			break;
		case 0x3: //Olfatear
			break;
		case 0x4: //Recibir orden
			break;
	}
	//scheduler_desalojame_pls()
}

void teclado_atender(){
	unsigned char scancode = teclado_leer();
	tecla_actualizar(scancode);
	if (scancode > 128 || scancode == LSHIFT || scancode == BKSP)
		return;
	/*
	if (scancode <= 128 && scancode != LSHIFT && scancode != BKSP)
		screen_pintar(ascii, 6, 6, 15);
	*/
	perro_t* p;
	switch(scancode){
		case O:
			p=game_jugador_dame_perro_libre(&jugadorA);
			sched_agregar_tarea(p);
			break;
		case Q:
			p=game_jugador_dame_perro_libre(&jugadorB);
			sched_agregar_tarea(p);
			break;
	}
}
