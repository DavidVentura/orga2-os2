#!/bin/sh
egypt  --include-external {main,mmu,sched,tss,gdt}.c.192r.expand | dot -Grotate=90 -Gsize=50,45 -Tps -o kernel.ps
egypt  --include-external {idt,isr_c,keyboard}.c.192r.expand | dot -Grotate=90 -Gsize=50,45 -Tps -o int.ps
