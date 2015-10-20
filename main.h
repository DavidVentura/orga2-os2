/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

main
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "gdt.h"
#include "idt.h"
#include "game.h"
#include "screen.h"
#include "mmu.h"
#include "i386.h"
//#include "pic.h"
extern void resetear_pic();
extern void habilitar_pic();

void kmain();

extern void idt_cargar();
#endif
