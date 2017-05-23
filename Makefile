BOOT = boot.s

KERNEL_SOURCE = PiTest.c
KERNEL_ELF = kernel.elf
KERNEL_OBJECT = kernel.o
KERNEL_IMAGE = kernel.img

BUILD = build/

all: build assemble 
 
build: $(KERNEL_SOURCE)
	arm-none-eabi-gcc -O0 -march=armv8-a $(KERNEL_SOURCE) -nostartfiles -c -o $(KERNEL_OBJECT)
	arm-none-eabi-as -march=armv8-a $(BOOT) -o boot.o

assemble: link
	arm-none-eabi-objcopy $(KERNEL_ELF) -O binary $(KERNEL_IMAGE)

link: 
	arm-none-eabi-ld boot.o $(KERNEL_OBJECT) -o $(KERNEL_ELF) -T kernel.ld

disassemble: $(KERNEL_SOURCE)
	arm-none-eabi-gcc -O2 -march=armv8-a -nostartfiles -S $(KERNEL_SOURCE)


$(BUILD):
	mkdir $@

assembly: PiTest.s
	arm-none-eabi-as -march=armv8-a PiTest.s -o kernel.o

assemblyAll: assembly
	arm-none-eabi-ld kernel.o boot.o -o kernel.elf -T kernel.ld
	arm-none-eabi-objcopy kernel.elf -O binary kernel.img 
