#!/bin/bash

which dot >/dev/null
if [ $? -ne 0 ]; then
	echo "Instala graphviz"
	exit 1
fi
which egypt >/dev/null
if [ $? -ne 0 ]; then
	cat << EOF
wget http://www.gson.org/egypt/download/egypt-1.10.tar.gz
tar xvf egypt*gz
cd egypt*
perl Makefile.PL
make
sudo make install
EOF
	exit 2
fi
egypt  --include-external {main,mmu,sched,tss,gdt}*.expand | dot -Grotate=90 -Gsize=50,45 -Tps -o kernel.ps
egypt  --include-external {idt,isr_c,keyboard}*.expand | dot -Grotate=90 -Gsize=50,45 -Tps -o int.ps

ls *ps
echo "Abrilos con okular"
