ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/gba_rules

# Directories
SRC_DIR := src
C_SRC_DIR := $(SRC_DIR)/C
ASM_SRC_DIR := $(SRC_DIR)/asm
BUILD_DIR := build
INCLUDE_DIR := include $(C_SRC_DIR)

# Library target
LIBRARY := $(BUILD_DIR)/g_engine.a

# Tools
CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
AR := arm-none-eabi-ar

# Flags
CFLAGS := -mthumb -mthumb-interwork -O2 -Wall -fno-strict-aliasing $(foreach dir, $(INCLUDE_DIR), -I$(dir))

# Source files
C_SOURCES := $(shell find $(C_SRC_DIR) -type f -name '*.c')
ASM_SOURCES := $(shell find $(ASM_SRC_DIR) -type f -name '*.s')

# Object files (mirror source structure in build directory)
OBJ_FILES := $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) \
             $(ASM_SOURCES:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

all: $(LIBRARY)

# Archive library
$(LIBRARY): $(OBJ_FILES)
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(AR) rcs $@ $^

# Rule for C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for ASM source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(AS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(LIBRARY)

.PHONY: all clean
