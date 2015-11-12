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

#Sobre el TP
....

#Estructuras de datos del procesador

##GDT
En este TP utilizamos la GDT para almacenar:

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


#Estructuras de datos del sistema operativo

##TSS
##???

#Disposición de la memoria

#(Corto) Viaje por modo real

Tomando en cuenta que el procesador inicia en modo real y asi no nos sirve de nada, cambiamos a modo protegido lo antes posible; esto lo logramos en cuatro simples pasos:

* Habilitar A20
* Cargamos la GDT
* Prendemos el bit de modo protegido en el CR0
* Saltamos a modo protegido

De yapa, cambiamos el modo de video y mostramos un mensaje muy *chulo*<sup>[1]</sup> que nos indica que estamos en este tan útil modo.

#Modo protegido
Una vez que el procesador alcanza un estado útil configuramos los selectores de segmento a valores correspondientes (ver [GDT](#GDT)) y también cargamos el valor pedido (0x27000) en el stack pointer.
Configurados los registros correspondientes procedemos a escaparnos a C.

#Entrando en C (kmain)
Esta rutina se encarga de inicializar todas las estructuras relevantes del sistema operativo, cada rutina descripta en su propia sección.

#game

#screen

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
....

#Interrupciones
##Manejo de IDT
###Inicializando la IDT

Definimos nuestra IDT como un array de 255 entradas de tipo idt\_entry.

###Cargando la IDT

Abusamos del macro IDT\_ENTRY, provisto por la cátedra, para cargar en la tabla las interrupciones [0..19],32,33,70.
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

## Manejo de Interrupciones
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
* Un TSS con valores reales para la tarea Idle. Esta tarea corre en modo Kernel, por lo que utiliza todos los selectores de segmento de modo kernel. **FIXME explicar esto bien**
* Un descriptor de TSS asociado al segmento idle, para poder referenciarlo al hacer el salto.

#Habilitando el scheduler

#Tarea idle: "EL SALTO"
Una vez que tenemos todas las estructuras de datos necesarias para poder hacer un Task Switch, simplemente tenemos que hacer un **far jump** al offset correspondiente de la GDT.

Una vez que saltamos, el kernel se queda a la espera de interrupciones.




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
