---
title: TP3 - Systems programming
author: 
- David Ventura (673/13)
- Fabian Álvarez (674/13)
geometry: margin=2cm
---

<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />

\newpage

\hypersetup{
  colorlinks,
  linkcolor=black
}
\tableofcontents

#Sobre el Informe y Trabajo Práctico
En el presente informe se van a definir y aclarar los cambios que hicimos para la implementación del pequeño Sistema Operativo para el trabajo práctico N°3.
Como premisa general intentamos mantener la mayoría de las implementaciones en C. Esta decisión la tomamos teniendo en cuenta que las estructuras o lógica del trabajo iban a cambiar mucho durante las implementaciones, y tener el código en C nos permitía ver, en muchas situaciones, más facilmente que había que cambiar.
Además el compilador nos advertía de cambios en la aridad de funciones u otros detalles que en la implentación en asembler no se notaría hasta el momento de la ejecución.

#Estructuras de datos del procesador
A continuación damos una pequeña explicación de los descriptores utilizados para el Sistema Operativo y las estructuras de datos que se utilizaron para representarlos en el código.

##GDT {#GDT}
En este TP utilizamos la GDT para almacenar:

* Descriptores de segmento
* Descriptores de TSS


###Descriptores de segmento

Nuestra GDT tiene, inicialmente, 6 descriptores de segmento:

+---------------+-----------+-----------+-----------+-----------+
| Descripción   | Índice	| Tamaño	| Base		| Tipo		|
+===============+===============+===========+===========+=======+
| NULL	        | 0			| 			| 			| 			|
+---------------+-----------+-----------------------------------+
| Kernel Data	| 8			| 500MB		| 0			| 2 (RW)	|
+---------------+-----------+-----------------------------------+
| Kernel Code	| 9			| 500MB		| 0			| A (RW+X)	|
+---------------+-----------+-----------------------------------+
| User Data		| A			| 500MB		| 0			| 2 (RW)	|
+---------------+-----------+-----------------------------------+
| User Code		| B			| 500MB		| 0			| A (RW+X)	|
+---------------+-----------+-----------------------------------+
| Video Data	| C			| 4000b		| 0xB800	| 2 (RW)	|
+---------------+-----------+-----------+-----------------------+


###Descriptores de TSS

| Descriptor | Comentario  | SS0 | ESP0   | CR3     | EIP     | SS| CS|DS |
|------------|-------------|-----|--------|---------|---------|---|---|---|
| Inicial    | Inválido    | 0   | 0      | 0       | 0       | 0 | 0 | 0 |
| Idle       | Modo Kernel | 8   | 0x27000| 0x28000 | 0x16000 | 8 | 9 | 8 |
| Perro      | Dinámica    | A   |Dinámico| Dinámico|Dinámico | A | B | A |



###Implementación

Para la implementación de los descriptores se modificaron las estucturas dadas por la cátedra y expandieron para cada propiedad en particular, esto nos da mayor flexibilidad en C y más claridad al definir segmentos, ya que podemos identificar, leer o modificar cada *flag* rápidamente.
En el código [**GDT Entry**](#gdtEntry) puede verse la estructura utilizada para la representación.

**GDT Entry**

~~~~~~~{#gdtEntry .c .numberLines startFrom="20"}
typedef struct str_gdt_entry {
    unsigned short  limit_0_15;
    unsigned short  base_0_15;
    unsigned char   base_23_16;
    unsigned char   type:4;
    unsigned char   s:1;
    unsigned char   dpl:2;
    unsigned char   p:1;
    unsigned char   limit_16_19:4;
    unsigned char   avl:1;
    unsigned char   l:1;
    unsigned char   db:1;
    unsigned char   g:1;
    unsigned char   base_31_24;
} __attribute__((__packed__, aligned (8))) gdt_entry;
~~~~~~~


En el código [**GDT Entry Ejemplo**](#gdtEntryEjemplo) se ve un ejemplo de como se realiza la carga de una entrada de la GDT. En este caso el selector de código de usuario.

**GDT Entry Ejemplo**

~~~~~~~{#gdtEntry .c .numberLines startFrom="84"}
[GDT_IDX_UCODE_DESC] = (gdt_entry) {
        (unsigned short)    0xF400,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */
        (unsigned char)     0x01,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
} __attribute__((__packed__, aligned (8))) gdt_entry;
~~~~~~~


##IDT

Los descriptores de segmento de la IDT son todos similares, salvo la interrupción dedicada a la syscall que tiene DPL 3.

+---------------+---------------+-----------+-----------+-----------+-----------+
| Índice		| Selector		| Offset	| DPL		| Tipo		| Prsente	|
+===============+===============+===============+===========+===========+=======+
| 0		        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------------------------------+
| 1		        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------------------------------+
| 2		        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------------------------------+
| 3		        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------------------------------+
| 										...										|
+---------------+---------------+-----------+-----------------------------------+
| 32	        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------+-----------------------+
| 33	        | 0x08	        | 0			| 0			| 01110b	| 1			|
+---------------+---------------+-----------+-----------+-----------------------+
| 70	        | 0x08	        | 0			| **3**		| 01110b	| 1			|
+---------------+---------------+-----------+-----------+-----------------------+


### Implementación
Como en la GDT se declaró un nuevo struct que de más flexibilidad para cargar los selectores. En este caso se permitió cargar el tipo de selector por separado, para poder usar la misma estructura para Interrupt Gate, Task Gate, etc.
En el código [**IDT Entry**](#idtEntry) puede verse la estructura utilizada para la representación.

**IDT Entry**

~~~~~~~{#gdtEntry .c .numberLines startFrom="21"}
typedef struct str_idt_entry_fld {
    unsigned int offset_0_15:16;
	unsigned int ceros:3;
    unsigned int segsel:13;
    unsigned int ceros2:8;
	unsigned int tipo:5;
	unsigned int dpl:2;
	unsigned int p:1;
    unsigned int offset_16_31:16;
} __attribute__((__packed__, aligned (8))) idt_entry;
~~~~~~~


Luego se adecuó el macro *IDT_ENTRY* para que cargue los nuevos datos y sea más flexible. Como parámetro se le pasa el tipo de entrada que se desea (idt_entry_int o idt_entry_task) y el DPL.
Como en este caso todos los segmentos se encuentran presentes, el macro los inicializa con valor *1*.
En el código [**IDT Entry Macro**](#idtEntryMacro) se ve la estructura nueva del macro y unas líneas de ejemplo del funcionamiento.

**IDT Entry Macro**

~~~~~~~{#idtEntryMacro .c .numberLines startFrom="21"}
#define IDT_ENTRY(numero, _dpl, type)                                               \
    idt[numero].offset_0_15 = (ushort) ((uint)(&_isr ## numero) & (uint) 0xFFFF);   \
    idt[numero].segsel = (ushort) 0x08 ;                                            \
    idt[numero].tipo = type;                                                        \
    idt[numero].dpl = _dpl;                                                         \
    idt[numero].p = 1;                                                              \
    idt[numero].offset_16_31 = (ushort) ((uint)(&_isr ## numero) >> 16 & (uint) 0xFFFF);


void idt_inicializar() {
    // Excepciones
	IDT_ENTRY(0, 0, idt_entry_int);
	IDT_ENTRY(1, 0, idt_entry_int);
	IDT_ENTRY(2, 0, idt_entry_int);
	...
~~~~~~~


#Estructuras de datos del sistema operativo

##TSS

Siguiendo la misma idea que con la GDT y la IDT se completó el *struct tss* con el formato con el que se guarda la tss.
Se añadieron las funciones *uint crear_tss(...)* y *uint cargar_tss_en_gdt(uint base, char dpl)* que se encargan de crear e inicializar una tss y luego cargarla en la GDT.
Se aprovecha las estucturas utilizadas en la GDT para que sea fácil cargar los datos de la entrada de la TSS.
En el código [**TSS Cargar**](#tssCargar) se ve la carga de una TSS en la GDT.

Los bits s/l/db deben ser 0. No utilizamos páginas grandes por lo que g también es 0. La parte alta del límite la mantenemos en 0 dado que el tamaño de cada TSS es 0x67.

**TSS Cargar**

~~~~~~~{#tssCargar .c .numberLines startFrom="58"}
  uint cargar_tss_en_gdt(uint base, char dpl) {
	gdt_entry g;
	
	g.limit_0_15=0x67;
	g.base_0_15=(base & 0xFFFF);
	g.base_23_16=(base & 0xFF0000) >>16;
	g.type=0b1001;
	g.s=0;
	g.dpl=dpl;
	g.p=1;
	g.limit_16_19=0;
	g.l=0;
	g.db=0;
	g.g=0;
	g.base_31_24=(base >> 24);

	uint p=prox_gdt_libre();
	gdt[p]=g;
	return p;
}  
~~~~~~~


#Disposición de la memoria
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

# Pasos de cración del S.O.

##(Corto) Viaje por modo real

Tomando en cuenta que el procesador inicia en modo real y asi no nos sirve de nada, cambiamos a modo protegido lo antes posible; esto lo logramos en cuatro simples pasos:

* Habilitar A20
* Cargamos la GDT
* Prendemos el bit de modo protegido en el CR0
* Saltamos a modo protegido

De yapa, cambiamos el modo de video y mostramos un mensaje muy *chulo*<sup>[1]</sup> que nos indica que estamos en este tan útil modo.

##Modo protegido
Una vez que el procesador alcanza un estado útil configuramos los selectores de segmento a valores correspondientes (ver [GDT](#GDT)) y también cargamos el valor pedido (0x27000) en el stack pointer.
Configurados los registros correspondientes procedemos a escaparnos a C.

##Entrando en C
Una vez finalizado con el código en asembler se salta a la función **kmain()** en *main.c*.
Esta funcion reemplaza al *start* de *kernel.asm*, donde se van a centralizar todas las inicializaciones restantes y métodos a cargar.

**Inicializar Game y Screen**
Como primera instancia se inicializa el juego llamando a la función *game_inicializar()*. Luego se invoca a *screen_inicializar()* que dibuja el tablero de juego otros detalles.
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Dichas funciones no se modificaron.

#MMU
La unidad de manejo de memoria que hicimos se encarga de:

##Cargar el cr3 correspondiente
Dado por el enunciado, 0x28000

##Inicializar el directorio de kernel
Esto se encarga de

* Inicializar un directorio de paginas (Apunta la direccion de cada tabla de páginas a 0xFFFFF y pone flags: U:0, RW:1: P: 0)
* Apunta la primer entrada a la tabla de páginas y la marca presente
* Inicializa la primer tabla de páginas con identity mapping y el bit de presente activado.

##Activar la paginación
Consiste simplemente en activar el bit mas alto del registro cr0.

##Mapear la memoria de video
Como nos pide el enunciado, mapeamos para el kernel 0x200000 a 0xB8000 con r/w.

#Interrupciones
##Manejo de IDT
###Inicializando la IDT

Definimos nuestra IDT como un array de 255 entradas de tipo idt\_entry.

###Cargando la IDT

Abusamos del macro [**IDT\_ENTRY**](#idtEntryMacro), provisto por la cátedra, para cargar en la tabla las interrupciones [0..19],32,33,70.
Solo la interrupción #70 tiene DPL 3, el resto tiene DPL 0.

El código de las interrupciones lo generamos a partir de dos macros, ISRE e ISR (la segunda ajusta el stack haciendo un `push 0` para simular el código de error), las cuales pushean el número de interrupción y llaman a `_isr_generico`, que se encarga de pushear todos los selectores de segmento, registros de control y registros de propósito general, para la pantalla de debug.
Se pushean en un orden determinado para coincidir con el struct **str_cpu** definido en *isr.h*, el mismo sirve para poder acceder fácilmente al estado de los registros desde las interrupciones.

##Inicializando el teclado

Para lo relacionado al manejo del teclado se crearon los archivos *keyboard.h* y *keyboard.c*.

### keyboard.h

En el archivo *keyboard.h* se definen las estructuras de ayuda para la lectura del teclado. Para no tener que andar pasando los scancode entre funciones generamos un *enum __keys__* con los nombres de las teclas para identificarlas. En el enum solo se mapearon las comunes, las funciones u el pad numérico no se mapeó.
Asimismo generamos un *struct __key__* en el cual almacenamos el valor *ASCII* que le corresponde a dicha tecla (con o sin mayus), el scancode y el estado de la misma (presionado o no).
Se definió un *array* de *key* llamado **keyboard** donde van a almacenarse cada una de las teclas.
En el código [**Keyboard.h**](#keyboard) pueden observarse las estructuras definidas para el manejo del teclado.


**Keyboard.h**

~~~~~~~{#keyboard .c .numberLines startFrom="5"}
typedef struct str_key {
	unsigned char value;
	unsigned char mValue;
	unsigned char scancode;
	unsigned char pressed;
} key;

extern key keyboard[];

typedef enum {
	ESC = 1,
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
	MINUS, EQUALS, BKSP, TAB,
	Q, W, E, R, T, Y, U, I, O, P, LBRK, RBRK, ENTER, LCTRL,
	A, S, D, F, G, H, J, K, L, SCOLON, APOS, APOS2,
	LSHIFT, BSLASH, Z, X, C, V, B, N, M, COMMA, DOT, SLASH
} keys;
~~~~~~~

### keyboard.c
Dentro de *keyboard.c* se define el arreglo *keyboard* como un arreglo de 128 posiciones.
Se genera un macro para inicializar cada tecla, el mismo recibe el *scancode*, el *valor ASCII* y *valor ASCII modificado* el mismo corresponde al valor que debe mostrarse cuando se presiona *SHIFT* + *TECLA*. Luego se almacena en la posición *scancode* los valores pasados.
Dentro de **teclado_inicializar()** se llama a esta macro para cada tecla y de esta forma queda inicializado el teclado. Se utilizan los nombres del enum para mayor claridad.
En el código [**Init Teclado**](#initTeclado) se muestran las primeras lineas de inicialización del teclado como ejemplo.


**Init Teclado**

~~~~~~~{#initTeclado .c .numberLines startFrom="43"}
void teclado_inicializar() {
	KEY_ENTRY(0,0,0);
	KEY_ENTRY(ESC, 27, 27);
	KEY_ENTRY(_1, '1', '!');
	KEY_ENTRY(_2, '2', '@');
	KEY_ENTRY(_3, '3', '#');
	KEY_ENTRY(_4, '4', '$');
	KEY_ENTRY(_5, '5', '%');
	...
~~~~~~~

Las interrupciones obtienen la tecla presionada llamando a la función **keys teclado_leer()**. La misma llama a la función *teclado_l()* definida en *i386.h*, con la que se realiza una lectura del puerto del teclado.
Luego analiza si el scancode recibido es **mayor** a 128, le resta 128 y en la posición que resulta asigna el estado **0**, en otro caso le asigna el estado *1*. Luego devuelve el la entrada del enum correspondiente.
De esta forma logramos separar el código *ASCII* de la tecla, para realizar menos comparaciones.

La función *get_ascii(keys tecla)* revisa si el estado de la tecla **SHIFT** es *1*, si es así devuelve el valor en mayúsucla de la tecla. Sino devuelve el valor *ASCII* correspondiente.


## Reseteando y habilitando el pic
El reseteo y habilitación del pic se realiza desde *main.c* antes de activar las interrupciones, para eso se llama a las funciones:

* resetear_pic()
* habilitar_pic()

Que facilitó la cátedra, las mismas se encuentran en *pic.h*.


##Habilitando interrupciones
La activación o desactivación de las interrupciones se realizan con las instrucciones "*sti*" y "*cli*".
Para que el código quede más amistoso con C implementamos las funciones *interrupciones_desactivar* e *interrupciones_activar* dentro de **i386.h**.
Dentro de las funciones se realiza un *inline* con el código en assembler que queremos ejecutar.
En el código [**Habilitar Int**](#habilitarInt) se ven las funciones inline para habilitar o deshabilitar interrupciones.


**Habilitar Int**

~~~~~~~{#habilitarInt .c .numberLines startFrom="116"}
LS_INLINE void interrupciones_desactivar(void) {
    __asm __volatile("cli" : :);
}

LS_INLINE void interrupciones_activar(void) {
    __asm __volatile("sti" : :);
}
~~~~~~~
## Manejo de Interrupciones {#inthandler}
El handler genérico de interrupciones (*interrupcion_atender*) dentro de *isr_c.c* contiene un *switch-case* según el tipo de interrupción.
Una vez que el kernel se encargó de lanzar una tarea, se queda a la espera de ser llamado a través de interrupciones, que pueden ser de hardware o software.
Las interrupciones manejadas son:

###Clock
Esta interrupción la usamos para cambiar de tarea con el scheduler y hacer girar los relojes.

###Teclado
Esta interrupción controla el flujo del juego, nos deja mover a los jugadores y lanzar perros. También puede entrar a modo debug.
Informa al pic que se atendió la interrupción y se llama a la función *teclado_atender* dentro del mismo archivo en el cual se lee el valor del teclado y realiza la tarea que corresponda según la tecla que se presione.

###INT46
Se encarga de atender los pedidos que realizan las tareas.
Llama a  la función *int70(uint tipo, uint dir)* 

###Default
El caso default del *switch-case*, si está activado, imprime por pantalla el cartel de debug con el estado del CPU al momento de la interrupción.
Para imprimirlo se accede al struct *str_cpu* que contiene los valores antes de entrar a la interrupción.


#Habilitando tareas
Para poder lograr nuestro próximo objetivo (Saltar a una tarea) necesitamos:

* Un TSS basura, donde el procesador va a guardar su contexto "actual" en el momento de hacer el cambio de tarea
* Un descriptor de TSS asociado al segmento basura, para poder referenciarlo
* Un TSS con valores reales para la tarea Idle. Esta tarea corre en modo Kernel, por lo que utiliza todos los selectores de segmento de modo kernel (8 y 9, ver [**GDT**](#GDT)). 
* Un descriptor de TSS asociado al segmento idle, para poder referenciarlo al hacer el salto.

#Scheduler

## Estructura
Las estructuras del scheduler no se modificaron respecto a las que dispuso la cátedra. Los cambios más grandes son en los métodos que utiliza el scheduler para el manejo de las tareas.
Para no añadir nuevas estructuras y explotando que solamente van a haber dos jugadores en juego se dispuso de un ordenamiento *par - impar* de las tareas.

Esto quiere decir que todas las tareas del **jugador 0** van a ir en posiciones **pares** y todas las tareas del **jugador 1** van a ir en posiciones **impares** dentro del array de tareas del scheduler.
Se destacan los siguientes métodos dentro del scheduler:

### Inicializar Scheduler
Función: *sched_inicializar()* se encarga de inicializar las estructuras de memoria internas del scheduler.
Se reccore el array de tareas y se carga un puntero a perro *NULL* y gdt_index *0*.
También se establece como tarea actual algúna tarea inválida. Esto permite que cuando el scheduler atienda un tick de la primera tarea, pueda saltar a la misma.

\newpage

~~~~~~~{#schedInit .c .numberLines startFrom="16"}
void sched_inicializar() {
	sched_task_t task = (sched_task_t) { 0, NULL };

	int i;
	for (i = 0; i < MAX_CANT_TAREAS_VIVAS; i++) {
		scheduler.tasks[i] = task;
	}
	scheduler.current = 8; // Tarea inválida
}
~~~~~~~


### TODO


#Tarea idle: "EL SALTO"
Una vez que tenemos todas las estructuras de datos necesarias para poder hacer un Task Switch, simplemente tenemos que hacer un **far jump** al offset correspondiente de la GDT.

Una vez que saltamos, el kernel se queda a la espera de interrupciones (Ver [**Manejo de interrupciones**](#inthandler)).




#El jueguito


~~~~~~~{#codigo .c .numberLines startFrom="30"}
if (a > 3) {
  moveShip(5 * gravity, DOWN);
}
~~~~~~~

~~~~~~~asm
mov eax, 0x0
mov esp, eax
lea ebx, [esp]
~~~~~~~



[1] ![Chulisimo](imagenes/chulo.png "Titulo")
