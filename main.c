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

	idt_inicializar(); // Inicializar la IDT
	idt_cargar(IDT_DESC); // Cargar IDT

    // Configurar controlador de interrupciones

    // Cargar tarea inicial

    // Habilitar interrupciones

    // Saltar a la primera tarea: Idle


}
