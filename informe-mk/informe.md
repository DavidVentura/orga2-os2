<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />

#Orga 2 - TP3

##Sobre el TP
....

##Estructuras de datos del procesador

###GDT
###IDT

##Estructuras de datos del sistema operativo

###TSS
###???

##Disposición de la memoria

##(Corto) Viaje por modo real

Tomando en cuenta que el procesador inicia en modo real y asi no nos sirve de nada, cambiamos a modo protegido lo antes posible; esto lo logramos en cuatro simples pasos:

* Habilitar A20
* Cargamos la GDT
* Prendemos el bit de modo protegido en el CR0
* Saltamos a modo protegido

De yapa, cambiamos el modo de video y mostramos un mensaje muy *chulo* que nos indica que estamos en este tan útil modo.

##Modo protegido
Una vez que el procesador alcanza un estado útil configuramos los selectores de segmento a valores correspondientes (ver [GDT](#GDT)) y también cargamos el valor pedido (0x27000) en el stack pointer.
Configurados los registros correspondientes procedemos a escaparnos a C.

##Entrando en C (kmain)
Esta rutina se encarga de inicializar todas las estructuras relevantes del sistema operativo, cada rutina descripta en su propia sección.

##game

##screen

##MMU
La unidad de manejo de memoria que hicimos se encarga de:

###Inicializar el directorio de kernel
La inicializacion del directorio de kernel

###Activar paginacion

###Mapear una pagina de video
???

