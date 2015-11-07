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
			screen_pintar(num+48, 4, 4, 15);
			break;
	}

}

void int70() {
	uint tipo, ecx;
    __asm __volatile("movl %%eax,%0" : "=r" (tipo));
    __asm __volatile("movl %%ecx,%0" : "=r" (ecx));
	switch(tipo){
		case 0x1: //Moverse
			//en ECX: 4 arriba, 7 abajo, 10 derecha, 13 izquierda
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
	switch(scancode){
		case O:
			sched_agregar_tarea(game_jugador_dame_perro_libre(&jugadorA));
			break;
		case Q:
			sched_agregar_tarea(game_jugador_dame_perro_libre(&jugadorB));
			break;
	}
}
