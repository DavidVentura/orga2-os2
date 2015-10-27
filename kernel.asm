; asmsyntax=nasm
; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"
extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern game_inicializar
extern screen_inicializar
extern kmain



global paginacion_activar
global cosa_loca_paginacion
global start
global idt_cargar
global cr3_cargar
global teclado_leer


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; Habilitar A20
	call habilitar_A20

    ; Cargar la GDT
	lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
	mov eax,cr0
	or eax,1
	mov cr0,eax

    ; Saltar a modo protegido
	jmp 0x40:modoprotegido ;0x40==64==8<<3
BITS 32
	modoprotegido:

    ; Establecer selectores de segmentos
	xor eax,eax
	mov ax, 1001000b ; { index:  | gdt (0) | rpl: 00 }
	mov ds, ax ;data
	mov es, ax
	mov gs, ax

	mov ax, 1100000b ; { index: 12 | gdt(0) | rpl: 00 }
	mov fs, ax ;video

    ; Establecer la base de la pila
	mov esp, 0x2700 ;lo pide el tp


    ; Imprimir mensaje de bienvenida

    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0


	call kmain


    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

paginacion_activar:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret

cosa_loca_paginacion:

	xchg bx, bx

	xor ebx, ebx
	mov ebx, 0xDEADBEEF 
	mov [0x123], ebx

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	xor ebx, ebx
	mov ebx, [0x123]

	and eax, 0x7FFFFFFF
	mov cr0, eax
	ret
idt_cargar:
	lidt [esp+4]
	ret

cr3_cargar:
	mov eax,[esp+4]
	mov cr3, eax
	ret

teclado_leer:
	in al, 0x60
	push eax
	ret
%include "a20.asm"
