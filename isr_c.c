#include "isr.h"
#include "i386.h"
#include "screen.h"


void interrupcion_atender(unsigned int num, unsigned int eflags, unsigned short cs, unsigned int eip, unsigned short errorCd) {
	if (num == 0)
		print("ERROR 0", 4, 4, 15);
//	printf("ERRORR", 4);
//	print_hex(num, 1, 4, 15, 5);
	breakpoint();
}

