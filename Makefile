DIRECTORIES = $(filter-out ./ ./.%, $(shell find ./ -maxdepth 10 -type d))

LOCOAL_DIRS = -I DIRECTORIES

SRC_C = $(wildcard *.c) $(wildcard **/*.c)
SRC_ASS = $(wildcard *.s) $(wildcard **/*.s)

OBJECTS_C = $(addprefix $(OBJECT_DIR)c/, $(notdir $(SRC_C:.c=.o)))
OBJECTS_ASS = $(addprefix $(OBJECT_DIR)ass/, $(notdir $(SRC_ASS:.s=.o)))
OBJECTS = $(OBJECTS_C) $(OBJECTS_ASS)

LINKERS = $(wildcard *.ld) $(wildcard **/*.ld)

LOCAL_INCLUDE_DIRS =$(addprefix -I, $(DIRECTORIES))

KERNEL_IMAGE = kernel.img
KERNEL_ELF = kernel.elf

OBJECT_DIR = objects/
BUILD_DIR = build/
LOG_DIR = logs/

SRC_DIR = source/

all: $(OBJECTS) $(BUILD_DIR)$(KERNEL_ELF) $(KERNEL_IMAGE)

%/:
	mkdir $@

$(OBJECTS_C): $(OBJECT_DIR) $(OBJECT_DIR)c/
	arm-none-eabi-gcc -O0 -march=armv8-a $(wildcard */$(@F:.o=.c)) -nostartfiles -c -o $@

$(OBJECTS_ASS): $(OBJECT_DIR) $(OBJECT_DIR)ass/
	arm-none-eabi-as -march=armv8-a $(wildcard */$(@F:.o=.s)) -c -o $@

$(BUILD_DIR)$(KERNEL_ELF): $(BUILD_DIR)
	arm-none-eabi-ld $(OBJECTS) -o $(BUILD_DIR)$(KERNEL_ELF) -T $(LINKERS)

$(KERNEL_IMAGE): $(BUILD_DIR)$(KERNEL_ELF)
	arm-none-eabi-objcopy $(BUILD_DIR)$(KERNEL_ELF) -O binary $(KERNEL_IMAGE)

disassemble: $(LOG_DIR)
	arm-none-eabi-objdump -D $(BUILD_DIR)$(KERNEL_ELF) > $(LOG_DIR)kenrel.list

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(LOG_DIR)
	if [ -f $(KERNEL_IMAGE) ]; then rm $(KERNEL_IMAGE); fi;

print-%: ; @echo $* = $($*)


define \n


endef