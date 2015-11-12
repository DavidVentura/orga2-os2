/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "i386.h"
#include "screen.h"
#include "keyboard.h"
#include "game.h"
#include "mmu.h"

#ifndef __ISR_H__
#define __ISR_H__

extern void fin_intr_pic1();
extern void fin_intr_pic2();
extern perro_t* game_jugador_dame_perro_libre(jugador_t *j);

typedef struct str_cpu {
	// Registros de control
	unsigned int cr4;
	unsigned int cr3;
	unsigned int cr2;
	unsigned int cr0;

	// Selectores de segmento
	unsigned int ds;
	unsigned int es;
	unsigned int fs;
	unsigned int gs;

	// Datos del pushad
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int ignorame;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	
	// Datos de la interrupcion
	unsigned int intNum;
	unsigned int errorCd;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
	unsigned int esp;
	unsigned int ss;
} cpu;


void interrupcion_atender(cpu* interrupcion);
void teclado_atender();

void printDebug(unsigned int interrupcion, unsigned int errorCode);

// Handlers de las interrupciones
void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();
void _isr20();

// Clock y teclado
void _isr32();
void _isr33();

// Software
uint _isr70();
uint int70();

#endif  /* !__ISR_H__ */
