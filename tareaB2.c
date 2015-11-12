/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "syscall.h"

int rand();

void task(int x_target, int y_target) {
//  int dir[4]  =  {0x4,0x7,0xA,0xD}; 
  //int seed = 123456789;
  while(1) {
	//__asm __volatile("xchg %%bx, %%bx" : :);
	
  	//syscall_moverse(dir[rand(&dir)]);
  	syscall_moverse(0xD);
  }
}

int rand(int* seed) {
//  seed = (1103515245 * seed + 12345)  % 4;
//  uint ret = (uint)&seed;
//  return ret;
	return 0x4;
}
