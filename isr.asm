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


global _isr_generico
extern int70

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


; -------------------------------------------------
; Macro para interrupciones con errorCd
%macro ISRE 1
global _isr%1
_isr%1:
	push dword %1				; Pusheo numero de int
	jmp _isr_generico
%endmacro


; --------------------------------------------------
; Macro para interrupciones sin errorCd
%macro ISR 1
global _isr%1
_isr%1:
	push dword 0x0				; Pusheo codError 0
	push dword %1				; Pusheo numero de int
	jmp _isr_generico
%endmacro


_isr_generico:
	pushad							; Salvo todos los registros grales
	
	push ebp						; Parametro ebp
	call guardar_estado_cpu			; Salvo registros
	add esp, 4						; Restauro esp

	lea ebp, [esp + 32]				; Cargo puntero a stack switch
	push ebp
	call interrupcion_atender
	add esp, 4

	popad
	add esp, 8						; Elimino numero de int y erroCode
	iret

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
ISRE 8
ISR 9
ISRE 10
ISRE 11
ISRE 12
ISRE 13
ISRE 14
ISR 15
ISR 16
ISRE 17
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
global _isr70
_isr70:
	push ecx
	push eax
	call int70
	add esp, 8
	iret

