; asmsyntax=nasm

; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; Atendedor externo
extern interrupcion_atender

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

extern guardar_estado_cpu

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
	pusha
    mov ebx, %1
	push ebx

	call guardar_estado_cpu
	call interrupcion_atender

	add esp, 4
	popa
	iret
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
ISR 32

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
ISR 33

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
ISR 70

