/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "pic.h"
#include "i386.h"
#include "screen.h"


#ifndef __ISR_H__
#define __ISR_H__
extern unsigned int teclado_leer();

void interrupcion_atender(unsigned int num, unsigned int eflags, unsigned short cs, unsigned int eip, unsigned short errorCd);
void teclado_atender();

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
void _isr70();

#endif  /* !__ISR_H__ */
