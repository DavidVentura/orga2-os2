#include "main.h"

void kmain() {
	//como_yoda_hablar();

	game_inicializar(); // Inicializar el juego
	screen_inicializar(); // Inicializar pantalla

    // Inicializar el manejador de memoria
	mmu_inicializar();

    // Inicializar el directorio de paginas

    // Cargar directorio de paginas

    // Habilitar paginacion

    // Inicializar tss

    // Inicializar tss de la tarea Idle

    // Inicializar el scheduler
	
	// Inicializar la IDT
	idt_inicializar(); 

	// Cargar IDT
	// FIXME: Pasar por parametro el puntero al descriptor no funciona bien
	idt_cargar();
	
	// Cargo el teclado en memoria
	teclado_inicializar();

	int cero = 0;
	cero = cero / cero;

    // Configurar controlador de interrupciones
	resetear_pic();
	habilitar_pic();

	// Habilito interrupciones
	interrupciones_activar();

    // Cargar tarea inicial
	tss_inicializar();
	tarea(DTSS_IDLE);
//	__asm("int $0x46");
    // Habilitar interrupciones

    // Saltar a la primera tarea: Idle


}
