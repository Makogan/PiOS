# The following makefile was made by Camilo Talero

# Find all directories under the current one

PERMA_DIRS := ./Documentation $(source)

SUBDIRECTORIES ?= $(filter-out ./Documentation ./Documentation/%, $(filter-out ./ ./.%, $(shell find ./ -maxdepth 10 -type d)))

VPATH ?= $(DIRECTORIES)

# Declare key directories
SOURCE_DIR ?= source
OBJECT_DIR ?= objects
BUILD_DIR ?= build
LOG_DIR ?= logs

# Define the output directories (i.e directories that may be deleted by clean)
OUTPUT_DIRS ?= $(sort $(subst $(SOURCE_DIR),$(OBJECT_DIR), $(SUBDIRECTORIES)) ./$(BUILD_DIR) ./$(LOG_DIR))

# Find all source files
SRC_C := $(shell find -name \*.c)
SRC_ASS := $(shell find -name \*.s)
LINKERS := $(shell find -name \*.ld)

# Create output object files that obey the same directory structure as the 
# source files
OBJECTS_C := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_C:.c=.o))
OBJECTS_ASS := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_ASS:.s=.o))
OBJECTS := $(OBJECTS_C) $(OBJECTS_ASS)

# Define the main binary file and the final kernel image
KERNEL_IMAGE ?= kernel.img
KERNEL_ELF ?= $(BUILD_DIR)/kernel.elf

# Compile and link the entire project
.PHONY: all disassemble clean
all: $(KERNEL_IMAGE) disassemble

# Create any missing output directories
$(OUTPUT_DIRS):
	$(foreach _dir,$@,mkdir -p $(_dir);)

# Create all .c object files
$(OBJECTS_C): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c | $(OUTPUT_DIRS)
	arm-none-eabi-gcc -O0 -march=armv8-a $< -nostartfiles -c -o $@

# Create all .s object files
$(OBJECTS_ASS): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.s | $(OUTPUT_DIRS)
	arm-none-eabi-as -march=armv8-a $< -c -o $@

# Link all .o object files into the final .elf binary
$(KERNEL_ELF): $(OBJECTS)
	arm-none-eabi-ld $(OBJECTS) -o $(KERNEL_ELF) -T $(LINKERS)

# Extract the final kernel image
$(KERNEL_IMAGE): $(KERNEL_ELF)
	arm-none-eabi-objcopy $(KERNEL_ELF) -O binary $(KERNEL_IMAGE)

# Dissassemble the kernel elf for debugging
disassemble: $(KERNEL_ELF) | $(LOG_DIR)
	arm-none-eabi-objdump -D $(KERNEL_ELF) > $(LOG_DIR)/kernel.list

# remove all non source directories and files
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(LOG_DIR)
	if [ -f $(KERNEL_IMAGE) ]; then rm $(KERNEL_IMAGE); fi;

# Print variable values
print-%: ; @echo $* = $($*)