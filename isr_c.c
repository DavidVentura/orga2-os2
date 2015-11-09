#include "isr.h"

static char debugEnabled;
static cpu cpuStatus;

void interrupcion_atender(unsigned int num, unsigned int eflags, unsigned short cs, unsigned int eip, unsigned short errorCd) {

	switch (num){
		case 32:
			fin_intr_pic1();
			sched_atender_tick();
			screen_actualizar_reloj_global();
			break;
		case 33:
			teclado_atender();
			fin_intr_pic1();
			break;
		case 70:
			int70();
			break;
		default:
			print("ERROR: ", 0, 0, 0xF);
			print_dec(num, 7, 0, 0x4);

			print(" - Eflags: ", 9, 0, 0xF);
			print_hex(eflags, 20, 0, 0x4);

			print(" - CS: ", 30, 0, 0xF);
			print_dec(cs, 37, 0, 0x4);

			print(" - EIP: ", 47, 0, 0xF);
			print_hex(eip, 0, 55, 0x4);

			if (num >= 10 && num <= 14) {
				print(" - Error Cod: ", 0, 1, 0xF);
				print_dec(errorCd, 9, 1, 0x4);
			}

			// Si está activado el debug, muestro mensaje
			// FIXME: Esto muestra los eflags, eip, etc de la interrupción.
			// Tengo que pasar por parámetro los datos de la interrupción.
			if (debugEnabled) {
				printDebug();
			}
			break;
	}
}

void int70() {
	uint tipo, dir;
	uint xOrig, yOrig;
	
	tipo = cpuStatus.eax;
	dir = cpuStatus.ecx;

	// Consigo el perro actual
	perro_t* aPerro = scheduler.tasks[scheduler.current].perro;


	// TODO: Temporal, acciones del perro
	print("TIPO: ", 0, 2, 15);
	print_dec(tipo, 5, 2, 15);
	print("DIR: ", 0, 3, 15);
	print_dec(dir, 5, 3, 15);

	switch(tipo) {
		case 0x1: //Moverse
			xOrig = aPerro->x;
			yOrig = aPerro->y;
			game_perro_mover(aPerro, dir);
			if (xOrig != aPerro->x || yOrig != aPerro->y) {
				mmu_copiar_pagina(mmu_xy2fisica(xOrig, yOrig), mmu_xy2fisica(aPerro->x, aPerro->y));
				mmu_mapear_pagina(mmu_xy2virtual(aPerro->x, aPerro->y), rcr3(), mmu_xy2fisica(aPerro->x, aPerro->y), 0,1, 1);
			}
			break;
		case 0x2: //Cavar
			break;
		case 0x3: //Olfatear
			break;
		case 0x4: //Recibir orden
			break;
	}
	//scheduler_desalojame_pls()
	return;
}

void teclado_atender(){
	perro_t* p;
	unsigned char tecla = teclado_leer();

	// Si presiono una tecla
	if (keyboard[tecla].pressed) {
		switch(tecla) {
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
				if (!debugEnabled)
					screen_inicializar();
				else
					screen_pintar('D', 0x0A, 0, 79);
				break;
			case T:		// Force print Debug
				printDebug();
				break;
			case LSHIFT:	// Ignoro shift
				break;
			default:			
				screen_pintar(get_ascii(tecla), 15, 0, 0);
				break;
		}
	} else {	// Si se solto una tecla
		switch(tecla) {
			case T:
				screen_inicializar();
				break;
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
	print("ebx", startX + 2, startY + 5, 0x70);
	print("ecx", startX + 2, startY + 7, 0x70);
	print("edx", startX + 2, startY + 9, 0x70);
	print("esi", startX + 2, startY + 11, 0x70);
	print("edi", startX + 2, startY + 13, 0x70);
	print("ebp", startX + 2, startY + 15, 0x70);
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
	print_hex(cpuStatus.eax, startX + 6, startY + 3, 0x7F);
	print_hex(cpuStatus.ebx, startX + 6, startY + 5, 0x7F);
	print_hex(cpuStatus.ecx, startX + 6, startY + 7, 0x7F);
	print_hex(cpuStatus.edx, startX + 6, startY + 9, 0x7F);
	print_hex(cpuStatus.esi, startX + 6, startY + 11, 0x7F);
	print_hex(cpuStatus.edi, startX + 6, startY + 13, 0x7F);
	print_hex(cpuStatus.ebp, startX + 6, startY + 15, 0x7F);
	print_hex(cpuStatus.esp, startX + 6, startY + 17, 0x7F);
	print_hex(cpuStatus.eip, startX + 6, startY + 19, 0x7F);
	print_hex(cpuStatus.cs, startX + 6, startY + 21, 0x7F);
	print_hex(cpuStatus.ds, startX + 6, startY + 23, 0x7F);
	print_hex(cpuStatus.fs, startX + 6, startY + 25, 0x7F);
	print_hex(cpuStatus.gs, startX + 6, startY + 27, 0x7F);
	print_hex(cpuStatus.ss, startX + 6, startY + 29, 0x7F);
	print_hex(cpuStatus.eflags, startX + 9, startY + 31, 0x7F);

	// Segunda columna
	print_hex(rcr0(), startX + 20, startY + 3, 0x7F);
	print_hex(rcr2(), startX + 20, startY + 5, 0x7F);
	print_hex(rcr3(), startX + 20, startY + 7, 0x7F);
	print_hex(rcr4(), startX + 20, startY + 9, 0x7F);
	int i;
	for (i = 0; i <= 10; i++)
		print_hex(rstack(i*4), startX + 22, startY + 19 + i, 0x7F);
}

// Guardo estado de los registros
void guardar_estado_cpu() {
	// TODO: Pierdo estado del ebp
	cpuStatus.eax = reax();
	cpuStatus.ebx = rebx();
	cpuStatus.ecx = recx();
	cpuStatus.edx = redx();
	cpuStatus.esi = resi();
	cpuStatus.edi = redi();
	cpuStatus.ebp = rebp();
	cpuStatus.esp = resp();
	cpuStatus.eip = reip();
	cpuStatus.cs = rcs();
	cpuStatus.ds = rds();
	cpuStatus.es = res();
	cpuStatus.fs = rfs();
	cpuStatus.gs = rgs();
	cpuStatus.ss = rss();
	cpuStatus.eflags = reflags();
	cpuStatus.cr0 = rcr0();
	cpuStatus.cr2 = rcr2();
	cpuStatus.cr3 = rcr3();
	cpuStatus.cr4 = rcr4();
}
