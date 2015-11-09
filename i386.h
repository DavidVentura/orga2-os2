/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  rutinas comunes para lectura y escritura de registros desde C
*/

#ifndef __i386_H__
#define __i386_H__

#define LS_INLINE static __inline __attribute__((always_inline))

LS_INLINE void lcr0(unsigned int val);
LS_INLINE unsigned int rcr0(void);
LS_INLINE void lcr1(unsigned int val);
LS_INLINE unsigned int rcr1(void);
LS_INLINE void lcr2(unsigned int val);
LS_INLINE unsigned int rcr2(void);
LS_INLINE void lcr3(unsigned int val);
LS_INLINE unsigned int rcr3(void);
LS_INLINE void lcr4(unsigned int val);
LS_INLINE unsigned int rcr4(void);
LS_INLINE void tlbflush(void);
LS_INLINE void ltr(unsigned short sel);
LS_INLINE unsigned short rtr(void);
LS_INLINE void hlt(void);
LS_INLINE void breakpoint(void);
LS_INLINE void interrupciones_desactivar(void);
LS_INLINE void interrupciones_activar(void);
LS_INLINE unsigned char teclado_l(void);

// Leo registros varios
LS_INLINE unsigned int reax(void);
LS_INLINE unsigned int rebx(void);
LS_INLINE unsigned int recx(void);
LS_INLINE unsigned int redx(void);
LS_INLINE unsigned int resi(void);
LS_INLINE unsigned int redi(void);
LS_INLINE unsigned int rebp(void);
LS_INLINE unsigned int resp(void);
LS_INLINE unsigned int reip(void);
LS_INLINE unsigned int rcs(void);
LS_INLINE unsigned int rds(void);
LS_INLINE unsigned int res(void);
LS_INLINE unsigned int rfs(void);
LS_INLINE unsigned int rgs(void);
LS_INLINE unsigned int rss(void);
LS_INLINE unsigned int reflags(void);
LS_INLINE unsigned int rstack(int offset);

/*
 * Implementaciones
 */

LS_INLINE void lcr0(unsigned int val) {
    __asm __volatile("movl %0,%%cr0" : : "r" (val));
}

LS_INLINE unsigned int rcr0(void) {
    unsigned int val;
    __asm __volatile("movl %%cr0,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr1(unsigned int val) {
    __asm __volatile("movl %0,%%cr1" : : "r" (val));
}

LS_INLINE unsigned int rcr1(void) {
    unsigned int val;
    __asm __volatile("movl %%cr1,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr2(unsigned int val) {
    __asm __volatile("movl %0,%%cr2" : : "r" (val));
}

LS_INLINE unsigned int rcr2(void) {
    unsigned int val;
    __asm __volatile("movl %%cr2,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr3(unsigned int val) {
    __asm __volatile("movl %0,%%cr3" : : "r" (val));
}

LS_INLINE unsigned int rcr3(void) {
    unsigned int val;
    __asm __volatile("movl %%cr3,%0" : "=r" (val));
    return val;
}

LS_INLINE void lcr4(unsigned int val) {
    __asm __volatile("movl %0,%%cr4" : : "r" (val));
}

LS_INLINE unsigned int rcr4(void) {
    unsigned int cr4;
    __asm __volatile("movl %%cr4,%0" : "=r" (cr4));
    return cr4;
}

 LS_INLINE void tlbflush(void) {
    unsigned int cr3;
    __asm __volatile("movl %%cr3,%0" : "=r" (cr3));
     __asm __volatile("movl %0,%%cr3" : : "r" (cr3));
}

LS_INLINE void ltr(unsigned short sel) {
    __asm __volatile("ltr %0" : : "r" (sel));
}

LS_INLINE unsigned short rtr(void) {
    unsigned short sel;
    __asm __volatile("str %0" : "=r" (sel) : );
    return sel;
}

LS_INLINE void hlt(void) {
    __asm __volatile("hlt" : : );
}

LS_INLINE void breakpoint(void) {
    __asm __volatile("xchg %%bx, %%bx" : :);
}


// Agregadas por nosotros
LS_INLINE void interrupciones_desactivar(void) {
    __asm __volatile("cli" : :);
}

LS_INLINE void interrupciones_activar(void) {
    __asm __volatile("sti" : :);
}

LS_INLINE unsigned char teclado_l(void) {
    unsigned char scancode;
	__asm __volatile("push %eax");
	__asm __volatile("in $0x60, %al");
    __asm __volatile("movb %%al,%0" : "=r" (scancode));
	__asm __volatile("pop %eax");
	return scancode;
}
LS_INLINE unsigned int reax(void) {
    unsigned int reg;
    __asm __volatile("mov %%eax,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rebx(void) {
    unsigned int reg;
    __asm __volatile("mov %%ebx,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int recx(void) {
    unsigned int reg;
    __asm __volatile("mov %%ecx,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int redx(void) {
    unsigned int reg;
    __asm __volatile("mov %%edx,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int resi(void) {
    unsigned int reg;
    __asm __volatile("mov %%esi,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int redi(void) {
    unsigned int reg;
    __asm __volatile("mov %%edi,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rebp(void) {
    unsigned int reg;
    __asm __volatile("mov %%ebp,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int resp(void) {
    unsigned int reg;
    __asm __volatile("mov %%esp,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int reip(void) {
    unsigned int reg;
    __asm __volatile("push %eax");						// Salvo eax
    __asm __volatile("call leer_eip");					// Llamo a eip
    __asm __volatile("leer_eip: pop %eax");				// Levanto el valor de retorno
    __asm __volatile("mov %%eax,%0" : "=r" (reg));		// Lo cargo en la variable
    __asm __volatile("pop %eax");						// Recupero eax
    return reg;
}

LS_INLINE unsigned int rcs(void) {
    unsigned int reg;
    __asm __volatile("mov %%cs,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rds(void) {
    unsigned int reg;
    __asm __volatile("mov %%ds,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int res(void) {
    unsigned int reg;
    __asm __volatile("mov %%es,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rfs(void) {
    unsigned int reg;
    __asm __volatile("mov %%fs,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rgs(void) {
    unsigned int reg;
    __asm __volatile("mov %%gs,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int rss(void) {
    unsigned int reg;
    __asm __volatile("mov %%ss,%0" : "=r" (reg));
    return reg;
}

LS_INLINE unsigned int reflags(void) {
    unsigned int reg;
    __asm __volatile("push %eax");						// Salvo eax
    __asm __volatile("pushf");							// Push eflags
    __asm __volatile("pop %eax");						// Lo leo en eax
    __asm __volatile("mov %%eax, %0" : "=r" (reg));		// Lo cargo en la variable
    __asm __volatile("pop %eax");						// Recupero el valor de eax
    return reg;
}


LS_INLINE unsigned int rstack(int offset) {
	unsigned int value;
	__asm __volatile("push %eax");						// Salvo eax
    __asm __volatile("movl %0,%%eax" : : "r" (offset));	// Cargo en eax el offset (negativo o positivo)
	__asm __volatile("add %esp, %eax");					// Sumo el valor del esp en eax
    __asm __volatile("mov (%%eax),%0" : "=r" (value));	// Cargo el valor de eax en la variable
    __asm __volatile("pop %eax");						// Recupero el valor de eax
	return value;
}
#endif  /* !__i386_H__ */
