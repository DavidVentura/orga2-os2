#include "isr.h"


void interrupcion_atender(unsigned int num, unsigned int eflags, unsigned short cs, unsigned int eip, unsigned short errorCd) {
	
	breakpoint();
	switch (num){
		case 32:
			teclado_atender();
			break;
		case 33:
			screen_actualizar_reloj_global();
			fin_intr_pic1();
			break;
		// TODO: case 70: SYSCAL
		default:
			screen_pintar(num, 4, 4, 15);
			break;
	}

//	breakpoint();
}

void teclado_atender(){
	unsigned char tecla = teclado_leer();
	screen_pintar(tecla, 6, 6, 15);
	fin_intr_pic1();
}
