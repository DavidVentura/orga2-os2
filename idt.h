/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __IDT_H__
#define __IDT_H__

static const unsigned char idt_entry_int  = 0b01110;
static const unsigned char idt_entry_task = 0b00101;

/* Struct de descriptor de IDT */
typedef struct str_idt_descriptor {
    unsigned short  idt_length;
    unsigned int    idt_addr;
} __attribute__((__packed__)) idt_descriptor;

/* Struct de una entrada de la IDT */
typedef struct str_idt_entry_fld {
    unsigned int offset_0_15:16;
	unsigned int ceros:3;
    unsigned int segsel:13;
    unsigned int ceros2:8;
	unsigned int tipo:5;
	unsigned int dpl:2;
	unsigned int p:1;
    unsigned int offset_16_31:16;
} __attribute__((__packed__, aligned (8))) idt_entry;

extern idt_entry idt[];
extern idt_descriptor IDT_DESC;

void idt_inicializar();


#endif  /* !__IDT_H__ */
