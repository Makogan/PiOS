# The following makefile was made by Camilo Talero

MICRO_SD ?= /media/camilo/GPMICROSD
#$(shell find /media/camilo -name "GPMICROSD")

DATE := $(shell date +'%y.%m.%d %H:%M:%S')
USER ?= $(shell -u -n)

# Compilers
CC = gcc
CCPP = g++
CAS = gcc
CROSS_COMPILER = arm-none-eabi

# Compilation flags
ARCH = -mcpu=cortex-a53 -mfloat-abi=hard -mfpu=vfpv4
OPT_LEVEL = O3

CFLAGS = -Wall -nostdlib -nostartfiles -nodefaultlibs -fno-exceptions -static -c -o 

# Find all directories under the current one

PERMA_DIRS := ./Documentation $(source)

SUBDIRECTORIES ?= $(filter-out ./Documentation ./Documentation/%, $(filter-out ./ ./.%, $(shell find ./ -maxdepth 10 -type d)))

I_DIRS ?= $(addprefix -I , $(filter ./source ./source/%, $(SUBDIRECTORIES)))

L_DIRS ?= $(addprefix -L , $(subst $(SOURCE_DIR),$(OBJECT_DIR), $(SUBDIRECTORIES)))

VPATH ?= $(SUBDIRECTORIES)

# Declare key directories
SOURCE_DIR ?= source
OBJECT_DIR ?= objects
BUILD_DIR ?= build
LOG_DIR ?= logs

# Define the output directories (i.e directories that may be deleted by clean)
OUTPUT_DIRS ?= $(sort $(subst $(SOURCE_DIR),$(OBJECT_DIR), $(SUBDIRECTORIES)) ./$(BUILD_DIR) ./$(LOG_DIR))

# Find all source files
SRC_C := $(shell find -name \*.c)
SRC_CPP := $(shell find -name \*.cpp)
SRC_ASS := $(shell find -name \*.S)
LINKERS := $(shell find -name \*.ld)

# Create output object files that obey the same directory structure as the
# source files
OBJECTS_C := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_C:.c=.o))
OBJECTS_ASS := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_ASS:.S=.o))
OBJECTS_CPP := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_CPP:.cpp=.o))
OBJECTS := $(OBJECTS_C) $(OBJECTS_ASS) $(OBJECTS_CPP)

# Define the main binary file and the final kernel image
KERNEL_IMAGE ?= kernel.img
KERNEL_ELF ?= $(BUILD_DIR)/kernel.elf

.PHONY: all disassemble clean git

# Compile and link the entire project
all: $(KERNEL_IMAGE) $(LOG_DIR)/kernel.list #copy

# Create any missing output directories
$(OUTPUT_DIRS):
	@echo "Making dirs"
	@$(foreach _dir,$@,mkdir -p $(_dir);)

# Create all .c object files
$(OBJECTS_C): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c | $(OUTPUT_DIRS)
	@echo "Compiling:" $<
	@$(CROSS_COMPILER)-$(CC) -$(OPT_LEVEL) $(ARCH) $< $(CFLAGS) $@ $(I_DIRS)

$(OBJECTS_CPP): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp | $(OUTPUT_DIRS)
	@echo "Compiling:" $<
	@$(CROSS_COMPILER)-$(CCPP) -$(OPT_LEVEL) $(ARCH) $< $(CFLAGS) $@ $(I_DIRS)

# Create all .s object files
$(OBJECTS_ASS): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.S | $(OUTPUT_DIRS)
	@echo "Compiling:" $<
	@$(CROSS_COMPILER)-$(CCPP) $(ARCH) $< -c -o $@

# Link all .o object files into the final .elf binary
$(KERNEL_ELF): $(OBJECTS)
	@echo "Linking"
	@$(CROSS_COMPILER)-ld $(OBJECTS) --gc-sections -o $(KERNEL_ELF) -T $(LINKERS)

# Extract the final kernel image
$(KERNEL_IMAGE): $(KERNEL_ELF)
	@echo "Creating final image"
	@$(CROSS_COMPILER)-objcopy $(KERNEL_ELF) -O binary $(KERNEL_IMAGE)

$(LOG_DIR)/kernel.list: $(KERNEL_ELF) | $(LOG_DIR)
	@echo "Output log"
	@$(CROSS_COMPILER)-objdump -D $(KERNEL_ELF) > $(LOG_DIR)/kernel.list

# remove all non source directories and files
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(LOG_DIR)
	if [ -f $(KERNEL_IMAGE) ]; then rm $(KERNEL_IMAGE); fi;

# Print variable values
print-%: ; @echo $* = $($*)
