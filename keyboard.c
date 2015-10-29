/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "keyboard.h"

key keyboard[128] = { };

#define KEY_ENTRY(_scancode, _value, _mValue)            \
    keyboard[_scancode].value = _value;                  \
    keyboard[_scancode].mValue = _mValue;                \
    keyboard[_scancode].scancode = _scancode;            \
    keyboard[_scancode].status = 0;                      \



unsigned char get_ascii(unsigned char scancode) {
	if (keyboard[LSHIFT].status == 1) {
		return keyboard[scancode].mValue;	
	} else {
		return keyboard[scancode].value;
	}
}

void tecla_actualizar(unsigned char scancode) {
	if (scancode > 128){
		keyboard[scancode - 128].status = 0;
	}else{
		keyboard[scancode].status = 1;
	}
}

void teclado_inicializar() {
	KEY_ENTRY(0,0,0);
	KEY_ENTRY(ESC, 27, 27);
	KEY_ENTRY(_1, '1', '!');
	KEY_ENTRY(_2, '2', '@');
	KEY_ENTRY(_3, '3', '#');
	KEY_ENTRY(_4, '4', '$');
	KEY_ENTRY(_5, '5', '%');
	KEY_ENTRY(_6, '6', '^');
	KEY_ENTRY(_7, '7', '&');
	KEY_ENTRY(_8, '8', '*');
	KEY_ENTRY(_9, '9', '(');
	KEY_ENTRY(_0, '0', ')');
	KEY_ENTRY(MINUS, '-', '_');
	KEY_ENTRY(EQUALS, '=', '+');
	KEY_ENTRY(BKSP, 8, 8);
	KEY_ENTRY(TAB, 9, 9);
	KEY_ENTRY(Q, 'q', 'Q');
	KEY_ENTRY(W, 'w', 'W');
	KEY_ENTRY(E, 'e', 'E');
	KEY_ENTRY(R, 'r', 'R');
	KEY_ENTRY(T, 't', 'T');
	KEY_ENTRY(Y, 'y', 'Y');
	KEY_ENTRY(U, 'u', 'U');
	KEY_ENTRY(I, 'i', 'I');
	KEY_ENTRY(O, 'o', 'O');
	KEY_ENTRY(P, 'p', 'P');
	KEY_ENTRY(LBRK, '[', '{');
	KEY_ENTRY(RBRK, ']', '}');
	KEY_ENTRY(ENTER, 13, 13);
	KEY_ENTRY(LCTRL, 0, 0);
	KEY_ENTRY(A, 'a', 'A');
	KEY_ENTRY(S, 's', 'S');
	KEY_ENTRY(D, 'd', 'D');
	KEY_ENTRY(F, 'f', 'F');
	KEY_ENTRY(G, 'g', 'G');
	KEY_ENTRY(H, 'h', 'H');
	KEY_ENTRY(J, 'j', 'J');
	KEY_ENTRY(K, 'k', 'K');
	KEY_ENTRY(L, 'l', 'L');
	KEY_ENTRY(SCOLON,';', ':');
	KEY_ENTRY(APOS,'\'', '"');
	KEY_ENTRY(APOS2,'`', '~');
	KEY_ENTRY(LSHIFT, 0, 0);
	KEY_ENTRY(BSLASH, '\\', '|');
	KEY_ENTRY(Z, 'z', 'Z');
	KEY_ENTRY(X, 'x', 'X');
	KEY_ENTRY(C, 'c', 'C');
	KEY_ENTRY(V, 'v', 'V');
	KEY_ENTRY(B, 'b', 'B');
	KEY_ENTRY(N, 'n', 'N');
	KEY_ENTRY(M, 'm', 'M');
	KEY_ENTRY(COMMA,',', '<');
	KEY_ENTRY(DOT,'.', '>');
	KEY_ENTRY(SLASH,'/', '?');
}
