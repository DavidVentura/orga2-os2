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
		// TODO: case 70: SYSCAL
		default:
			screen_pintar(num+48, 4, 4, 15);
			break;
	}

}

void teclado_atender(){
	unsigned char scancode = teclado_leer();
	tecla_actualizar(scancode);
	if (scancode <= 128 && scancode != LSHIFT && scancode != BKSP)
//		breakpoint();
		screen_pintar(get_ascii(scancode), 6, 6, 15);
}
