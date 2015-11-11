#include "main.h"

void kmain() {
	//como_yoda_hablar();

	game_inicializar(); // Inicializar el juego
	screen_inicializar(); // Inicializar pantalla

    // Inicializar el manejador de memoria
    // Inicializar el directorio de paginas
    // Cargar directorio de paginas
    // Habilitar paginacion
	mmu_inicializar();

	// Inicializar la IDT
	idt_inicializar(); 

	// Cargar IDT
	idt_cargar();
	
	// Cargo el teclado en memoria
	teclado_inicializar();

    // Configurar controlador de interrupciones
	resetear_pic();
	habilitar_pic();

	// Habilito interrupciones
	interrupciones_activar();

    // Inicializar tss
    // Inicializar tss de la tarea Idle
    // Inicializar el scheduler
	tss_inicializar();
    // Cargar tarea inicial
    // Saltar a la primera tarea: Idle
	tarea(DTSS_IDLE<<3);


}
