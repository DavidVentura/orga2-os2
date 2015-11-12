/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"

int dir[4]  =  {0x4,0x7,0xA,0xD}; 
int seed = 123456789;
int rand() {
  seed = (1103515245 * seed + 12345)  % sizeof(dir);
  return seed;
}

void task(int x_target, int y_target) {
    /* Tarea */
  while(1) {
	__asm __volatile("xchg %%bx, %%bx" : :);

  	syscall_moverse(dir[rand()]);
  }
}




