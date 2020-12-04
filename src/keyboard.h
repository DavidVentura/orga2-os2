/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de teclado 
*/

#include "i386.h"

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

typedef struct str_key {
	unsigned char value;
	unsigned char mValue;
	unsigned char scancode;
	unsigned char pressed;
} key;

extern key keyboard[];

typedef enum {
	ESC = 1,
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
	MINUS, EQUALS, BKSP, TAB,
	Q, W, E, R, T, Y, U, I, O, P, LBRK, RBRK, ENTER, LCTRL,
	A, S, D, F, G, H, J, K, L, SCOLON, APOS, APOS2,
	LSHIFT, BSLASH, Z, X, C, V, B, N, M, COMMA, DOT, SLASH
} keys;


void teclado_inicializar();
keys teclado_leer();
unsigned char get_ascii(keys key);
#endif
