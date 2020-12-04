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
#include "keyboard.h"
#include "tss.h"
#include "sched.h"

extern void idt_cargar();
extern void resetear_pic();
extern void habilitar_pic();
extern void tarea_idle();
extern perro_t* game_jugador_dame_perro_libre(jugador_t *j);


void kmain();

#endif
