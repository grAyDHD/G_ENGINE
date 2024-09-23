ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/gba_rules

C_SRC_DIR := C
ASM_SRC_DIR := asm
BUILD_DIR := build
INCLUDE_DIR := $(C_SRC_DIR)/include $(C_SRC_DIR)
G_ENGINE := $(BUILD_DIR)/g_engine.a

CC := arm-none-eabi-gcc
AS := arm-none-eabi-as
AR := arm-none-eabi-ar
CFLAGS := -mthumb -mthumb-interwork -O2 -Wall -fno-strict-aliasing $(foreach dir, $(INCLUDE_DIR), -I$(dir))

C_SOURCES := $(wildcard $(C_SRC_DIR)/**/*.c) $(wildcard $(C_SRC_DIR)/*.c)
ASM_SOURCES := $(wildcard $(ASM_SRC_DIR)/**/*.s)
OBJ_FILES := $(C_SOURCES:$(C_SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(ASM_SOURCES:$(ASM_SRC_DIR)/%.s=$(BUILD_DIR)/%.o)

all: $(G_ENGINE)

$(G_ENGINE): $(OBJ_FILES)
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(C_SRC_DIR)/%.c
	@[ -d $(BUILD_DIR)/$(dir $*) ] || mkdir -p $(BUILD_DIR)/$(dir $*)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ASM_SRC_DIR)/%.s
	@[ -d $(BUILD_DIR)/$(dir $*) ] || mkdir -p $(BUILD_DIR)/$(dir $*)
	$(AS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(G_ENGINE)

.PHONY: all clean
