#include "isr.h"

static char debugEnabled;
static char onDebug;
static cpu* cpuStatus;

void interrupcion_atender(cpu* status) {
	cpuStatus = status;

	switch (cpuStatus->intNum){
		case 32:
			fin_intr_pic1();
			screen_actualizar_reloj_global();
			if (!onDebug){
				sched_atender_tick();
			}
			break;
		case 33:
			fin_intr_pic1();
			teclado_atender();
			break;
		case 70:
			int70();
			break;
		default:
			print("Int: ", 0, 0, 0xF);
			print_dec(cpuStatus->intNum, 5, 0, 0x4);

			print("Eflags: ", 12, 0, 0xF);
			print_hex(cpuStatus->eflags, 20, 0, 0x4);

			print("CS: ", 30, 0, 0xF);
			print_hex(cpuStatus->cs, 34, 0, 0x4);

			print("EIP: ", 42, 0, 0xF);
			print_hex(cpuStatus->eip, 47, 0, 0x4);

			print("Error Cod: ", 59, 0, 0xF);
			print_hex(cpuStatus->errorCd, 70, 0, 0x4);

			print("SS: ", 0, 1, 0xF);
			print_hex(cpuStatus->ss, 5, 1, 0x4);

			print("ESP: ", 10, 1, 0xF);
			print_hex(cpuStatus->esp, 14, 1, 0x4);

			// Si está activado el debug, muestro mensaje
			if (debugEnabled && !onDebug) {
				onDebug = 1;
				printDebug();
				tarea(DTSS_IDLE<<3);
			}
			
			break;
	}
}

uint int70(uint tipo, uint dir){
	// Consigo el perro actual
	perro_t* aPerro = scheduler.tasks[scheduler.current].perro;


	// TODO: Temporal, acciones del perro
	/*
	print("TIPO: ", 0, 2, 15);
	print_dec(tipo, 5, 2, 15);
	print("DIR: ", 0, 3, 15);
	print_dec(dir, 5, 3, 15);
	*/

	uint virt_o;
	uint fisica_o;
	uint virt_n,fisica_n;
	switch(tipo) {
		case 0x1: //Moverse
			virt_o  =mmu_xy2virtual(aPerro->x, aPerro->y);
			fisica_o=mmu_xy2fisica (aPerro->x, aPerro->y);

			game_perro_mover(aPerro, dir);

			virt_n  =mmu_xy2virtual(aPerro->x, aPerro->y);
			fisica_n=mmu_xy2fisica (aPerro->x, aPerro->y);

			mmu_mapear_pagina(virt_o, aPerro->cr3, fisica_o, 1, 1, 1); //Esto porque la pos inicial no esta mapeada
			mmu_mapear_pagina(virt_n, aPerro->cr3, fisica_n, 1, 1, 1);
			mmu_copiar_pagina(virt_o, virt_n);
			break;
		case 0x2: //Cavar
			break;
		case 0x3: //Olfatear
			return game_perro_olfatear(aPerro);
		case 0x4: //Recibir orden
			break;
	}
	//scheduler_desalojame_pls()
	tarea(DTSS_IDLE<<3);
	return 0;
}

void teclado_atender(){
	perro_t* p;
	unsigned char tecla = teclado_leer();

	// Si presiono una tecla
	if (keyboard[tecla].pressed) {
		switch(tecla) {
			case W:
				game_jugador_moverse(&jugadorA,0,-1);
				break;
			case A:
				game_jugador_moverse(&jugadorA,-1,0);
				break;
			case S:
				game_jugador_moverse(&jugadorA,0,1);
				break;
			case D:
				game_jugador_moverse(&jugadorA,1,0);
				break;
			case I:
				game_jugador_moverse(&jugadorB,0,-1);
				break;
			case J:
				game_jugador_moverse(&jugadorB,-1,0);
				break;
			case K:
				game_jugador_moverse(&jugadorB,0,1);
				break;
			case L:
				game_jugador_moverse(&jugadorB,1,0);
				break;
			case O:
				p = game_jugador_dame_perro_libre(&jugadorA);
				sched_agregar_tarea(p);
				break;
			case Q:
				p = game_jugador_dame_perro_libre(&jugadorB);
				sched_agregar_tarea(p);
				break;
			case Y:		// Toggle Debug
				debugEnabled = !debugEnabled;

				// Si está el mensaje de debug, lo borro
				if (onDebug) {
					onDebug = 0;
					screen_inicializar();
				}

				// Borro la 'D' de debug
				if (!debugEnabled)
					screen_pintar(0, 0x00, 0, 79);
				else
					screen_pintar('D', 0x0A, 0, 79);
				break;
			default:			
				break;
		}
	} else {	// Si se solto una tecla
		switch(tecla) {
			default:
				break;
		}
	}
}

void printDebug() {
	int startX, startY, ancho, alto;
	startY = 5; startX = 25; ancho = 30; alto  = 36;

	// Imprimo cuadrado raro
	screen_pintar_rect(0, 0x77, startY, startX, alto, ancho);
	screen_pintar_linea_h(0, 0, startY, startX, ancho);
	screen_pintar_linea_h(0, 0, startY + alto, startX, ancho);
	screen_pintar_linea_v(0, 0, startY, startX, alto);
	screen_pintar_linea_v(0, 0, startY, startX + ancho, alto + 1);

	// Imprimo texto de los registros - Columna 1
	print("eax", startX + 2, startY + 3, 0x70);
	print("ecx", startX + 2, startY + 5, 0x70);
	print("edx", startX + 2, startY + 7, 0x70);
	print("ebx", startX + 2, startY + 9, 0x70);
	print("ebp", startX + 2, startY + 11, 0x70);
	print("esi", startX + 2, startY + 13, 0x70);
	print("edi", startX + 2, startY + 15, 0x70);
	print("esp", startX + 2, startY + 17, 0x70);
	print("eip", startX + 2, startY + 19, 0x70);
	print("cs", startX + 2, startY + 21, 0x70);
	print("ds", startX + 2, startY + 23, 0x70);
	print("es", startX + 2, startY + 25, 0x70);
	print("fs", startX + 2, startY + 27, 0x70);
	print("gs", startX + 2, startY + 29, 0x70);
	print("ss", startX + 2, startY + 31, 0x70);
	print("eflags", startX + 2, startY + 33, 0x70);

	// Columna 2
	print("cr0", startX + 16, startY + 3, 0x70);
	print("cr2", startX + 16, startY + 5, 0x70);
	print("cr3", startX + 16, startY + 7, 0x70);
	print("cr4", startX + 16, startY + 9, 0x70);
	print("Stack", startX + 16, startY + 19, 0x70);


	// Imprimo valores de los registros
	print_hex(cpuStatus->eax, startX + 6, startY + 3, 0x7F);
	print_hex(cpuStatus->ecx, startX + 6, startY + 5, 0x7F);
	print_hex(cpuStatus->edx, startX + 6, startY + 7, 0x7F);
	print_hex(cpuStatus->ebx, startX + 6, startY + 9, 0x7F);
	print_hex(cpuStatus->ebp, startX + 6, startY + 11, 0x7F);
	print_hex(cpuStatus->esi, startX + 6, startY + 13, 0x7F);
	print_hex(cpuStatus->edi, startX + 6, startY + 15, 0x7F);
	print_hex(cpuStatus->esp, startX + 6, startY + 17, 0x7F);
	print_hex(cpuStatus->eip, startX + 6, startY + 19, 0x7F);
	print_hex(cpuStatus->cs, startX + 6, startY + 21, 0x7F);
	print_hex(cpuStatus->ds, startX + 6, startY + 23, 0x7F);
	print_hex(cpuStatus->es, startX + 6, startY + 25, 0x7F);
	print_hex(cpuStatus->fs, startX + 6, startY + 27, 0x7F);
	print_hex(cpuStatus->gs, startX + 6, startY + 29, 0x7F);
	print_hex(cpuStatus->ss, startX + 6, startY + 31, 0x7F);
	print_hex(cpuStatus->eflags, startX + 9, startY + 33, 0x7F);

	// Segunda columna
	print_hex(cpuStatus->cr0, startX + 20, startY + 3, 0x7F);
	print_hex(cpuStatus->cr2, startX + 20, startY + 5, 0x7F);
	print_hex(cpuStatus->cr3, startX + 20, startY + 7, 0x7F);
	print_hex(cpuStatus->cr4, startX + 20, startY + 9, 0x7F);
	int i;
	for (i = 0; i <= 10; i++)
		print_hex(rstack(cpuStatus->esp + (i*4)), startX + 22, startY + 19 + i, 0x7F);
}
