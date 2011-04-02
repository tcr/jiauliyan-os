nasm -f elf -o loader.o loader.s
gcc -o kernel.o -c kernel.c -Wall -Wextra -m32 -nostdlib -nostartfiles -nodefaultlibs # -Werror
ld -melf_i386 -T linker.ld -o kernel.bin loader.o kernel.o
cat stage1 stage2 pad kernel.bin > floppy.img
