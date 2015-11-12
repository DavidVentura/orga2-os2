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
+---------------+-----------+-----------------------------------+


###Descriptores de TSS

##IDT


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
....

##Mapear la memoria de video
....

#Inicializando la IDT
Suerte fabian

#Cargando la idt

#Inicializando el teclado

#Reseteando el pic

#Habilitando el pic

#Habilitando interrupciones

/Suerte fabian
#Habilitando tareas
Para poder lograr nuestro próximo objetivo (Saltar a una tarea) necesitamos:

* Un TSS basura, donde el procesador va a guardar su contexto "actual" en el momento de hacer el cambio de tarea
* Un descriptor de TSS asociado al segmento basura, para poder referenciarlo
* Un TSS con valores reales para la tarea Idle. Esta tarea corre en modo Kernel, por lo que utiliza todos los selectores de segmento de modo kernel. **FIXME explicar esto bien**
* Un descriptor de TSS asociado al segmento idle, para poder referenciarlo al hacer el salto.

#Tarea idle: "EL SALTO"
Una vez que tenemos todas las estructuras de datos necesarias para poder hacer un Task Switch, simplemente tenemos que hacer un **far jump** al offset correspondiente de la GDT.

Una vez que saltamos, el kernel se queda a la espera de interrupciones.



#Interrupciones





















[1] ![Chulisimo](imagenes/chulo.png "Titulo")
