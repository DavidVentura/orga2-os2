/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "keyboard.h"

unsigned char get_character(unsigned char scancode){
    // TODO: Mayores a 53 = Tecla no mapeada o key relase
    if (scancode <= 53)
        return kb_keys[scancode];
    else
        return 0;
}

