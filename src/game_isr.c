
#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

/// ************** RUTINAS DE ATENCION DE INTERRUPCIONES DE ************** ///
///                              ALTO NIVEL                                ///
///                  (deben ser llamadas desde isr.asm)                    ///
/// ********************************************************************** ///


// gasta un rato en un ciclo infinito, util para hacer pausas y debuguear
void wait(int pseudosecs)
{
	int count;
	for (count = 0; count < pseudosecs * 1000000; count++) {}
}


uint game_syscall_manejar(uint syscall, uint param1)
{
    // ~ completar llamando a las funciones que haga falta ~
    return 0;
}

// ~~~ debe atender la interrupción de reloj para actualizar la pantalla y terminar si es hora,
// ~~~ recibe el perro que está corriendo actualmente
void game_atender_tick()
{
//	screen_inicializar();
	perro_t* p = scheduler.tasks[scheduler.current].perro;
	if(!p->libre)
		game_perro_ver_si_en_cucha(p);
	screen_actualizar_reloj_global();
    ultimo_cambio--;
	if (ultimo_cambio==0)
		game_terminar_si_es_hora();

	sched_atender_tick();
}
