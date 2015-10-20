#include "main.h"

void kmain() {
	//como_yoda_hablar();

    // Inicializar el juego

	game_inicializar();

    // Inicializar pantalla
	screen_inicializar();

    // Inicializar el manejador de memoria

    // Inicializar el directorio de paginas

    // Cargar directorio de paginas
    // Habilitar paginacion

    // Inicializar tss

    // Inicializar tss de la tarea Idle

    // Inicializar el scheduler

    // Inicializar la IDT
	idt_inicializar();
    // Cargar IDT
	idt_cargar(IDT_DESC);

    // Configurar controlador de interrupciones

    // Cargar tarea inicial

    // Habilitar interrupciones

    // Saltar a la primera tarea: Idle


}
