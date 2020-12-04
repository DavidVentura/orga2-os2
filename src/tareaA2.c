/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

typedef struct extra_t {
	char mapa[80*50];
	int next_id;
	int tesoros[8][2];
	int next_id_minero;
	
} extra_t;


void task(int x_target, int y_target) {
    /* Tarea */
	syscall_moverse(DER);
	syscall_moverse(DER);
	syscall_moverse(DER);
	syscall_moverse(DER);
	syscall_moverse(DER);
	syscall_moverse(DER);

	while(1) {
		syscall_moverse(ABA);
	
	}

}
