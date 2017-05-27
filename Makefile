# The following makefile was made by Camilo Talero

DATE := $(shell date +'%y.%m.%d %H:%M:%S')
USER ?= $(shell -u -n)

# Compilers
CC = gcc
CCPP = g++
CAS = as
CROSS_COMPILER = arm-none-eabi

# Compilation flags
ARCH = march=armv8-a
OPT_LEVEL = O0

CFLAGS = -nostartfiles -c -o

# Find all directories under the current one

PERMA_DIRS := ./Documentation $(source)

SUBDIRECTORIES ?= $(filter-out ./Documentation ./Documentation/%, $(filter-out ./ ./.%, $(shell find ./ -maxdepth 10 -type d)))

I_DIRS ?= $(addprefix -I , $(SUBDIRECTORIES))

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
SRC_ASS := $(shell find -name \*.s)
LINKERS := $(shell find -name \*.ld)

# Create output object files that obey the same directory structure as the
# source files
OBJECTS_C := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_C:.c=.o))
OBJECTS_ASS := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_ASS:.s=.o))
OBJECTS_CPP := $(subst $(SOURCE_DIR),$(OBJECT_DIR),$(SRC_CPP:.cpp=.o))
OBJECTS := $(OBJECTS_C) $(OBJECTS_ASS) $(OBJECTS_CPP)

# Define the main binary file and the final kernel image
KERNEL_IMAGE ?= kernel.img
KERNEL_ELF ?= $(BUILD_DIR)/kernel.elf


.PHONY: all disassemble clean git

# Compile and link the entire project
all: $(KERNEL_IMAGE) $(LOG_DIR)/kernel.list

# Create any missing output directories
$(OUTPUT_DIRS):
	$(foreach _dir,$@,mkdir -p $(_dir);)

# Create all .c object files
$(OBJECTS_C): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c | $(OUTPUT_DIRS)
	$(CROSS_COMPILER)-$(CC) -$(OPT_LEVEL) -$(ARCH) $< $(CFLAGS) $@

$(OBJECTS_CPP): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp | $(OUTPUT_DIRS)
	$(CROSS_COMPILER)-$(CCPP) -$(OPT_LEVEL) -$(ARCH) $< $(CFLAGS) $@

# Create all .s object files
$(OBJECTS_ASS): $(OBJECT_DIR)%.o: $(SOURCE_DIR)%.s | $(OUTPUT_DIRS)
	$(CROSS_COMPILER)-$(CAS) -$(ARCH) $< -c -o $@

# Link all .o object files into the final .elf binary
$(KERNEL_ELF): $(OBJECTS)
	$(CROSS_COMPILER)-ld $(OBJECTS) -o $(KERNEL_ELF) -T $(LINKERS)

# Extract the final kernel image
$(KERNEL_IMAGE): $(KERNEL_ELF)
	$(CROSS_COMPILER)-objcopy $(KERNEL_ELF) -O binary $(KERNEL_IMAGE)

# Dissassemble the kernel elf for debugging
disassemble: $(KERNEL_ELF) | $(LOG_DIR)

$(LOG_DIR)/kernel.list: $(KERNEL_ELF) | $(LOG_DIR)
	$(CROSS_COMPILER)-objdump -D $(KERNEL_ELF) > $(LOG_DIR)/kernel.list

diss:
	$(CROSS_COMPILER)-objdump -D $(INSPECT)

# remove all non source directories and files
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(LOG_DIR)
	if [ -f $(KERNEL_IMAGE) ]; then rm $(KERNEL_IMAGE); fi;

git:
	git add -A
	git commit -m "$(USER) made a minor commit on $(DATE)"
	git push

# Print variable values
print-%: ; @echo $* = $($*)
