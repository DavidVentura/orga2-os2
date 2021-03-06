/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Tipos basicos */
/* -------------------------------------------------------------------------- */
#define NULL                    0
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

/* Constantes basicas */
/* -------------------------------------------------------------------------- */
#define PAGE_SIZE               0x00001000
#define TASK_SIZE               4096

#define BOOTSECTOR              0x00001000 /* direccion fisica de comienzo del bootsector (copiado) */
#define KERNEL                  0x00001200 /* direccion fisica de comienzo del kernel */

#define KERNEL_PDIR				0x28000

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define KSTACK							0x27000
#define GDT_IDX_NULL_DESC				0

#define GDT_IDX_KCODE_DESC				8
#define GDT_IDX_KDATA_DESC				9
#define GDT_IDX_UDATA_DESC				10
#define GDT_IDX_UCODE_DESC				11
#define GDT_IDX_VDATA_DESC				12

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_KCODE_DESC			(GDT_IDX_KCODE_DESC		<< 3)	
#define GDT_OFF_KDATA_DESC			(GDT_IDX_KDATA_DESC		<< 3)	
#define GDT_OFF_UDATA_DESC			(GDT_IDX_UDATA_DESC		<< 3)	
#define GDT_OFF_UCODE_DESC			(GDT_IDX_UCODE_DESC		<< 3)

/* Selectores de segmentos */
/* -------------------------------------------------------------------------- */

#define DTSS_INIT 13
#define DTSS_IDLE 14

#endif  /* !__DEFINES_H__ */
