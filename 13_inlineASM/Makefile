# Environment setup
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/gba_rules

# Directory and file paths
G_ENGINE_DIR := ../../07_lib
G_ENGINE := $(G_ENGINE_DIR)/build/g_engine.a

# Build targets and source directories
TARGET := $(notdir $(CURDIR))
BUILD := build
SOURCES := C asm

# Include and asset directories
INCLUDES := $(G_ENGINE_DIR)/C/include $(G_ENGINE_DIR)/build 
DATA := 
MUSIC := 
GRAPHICS :=

# Compiler and linker flags
ARCH := -mthumb -mthumb-interwork
CFLAGS := -g -Wall -O2\
	-mcpu=arm7tdmi -mtune=arm7tdmi\
	$(ARCH)
CFLAGS += $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -g $(ARCH) -Wl,-Map,$(BUILD)/$(TARGET).map -specs=gba.specs

# Toolchain definitions
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

# Libraries and paths
LIBS := $(G_ENGINE)
LIBDIRS := $(G_ENGINE_DIR)/build

# Source files
CFILES := $(wildcard C/*.c)
SFILES := $(wildcard asm/*.s)
PNGFILES := $(wildcard $(GRAPHICS)/*.png)
BINFILES := $(wildcard $(DATA)/*.*)

# Object files
OFILES_C := $(patsubst C/%.c,$(BUILD)/%.o,$(CFILES))
OFILES_S := $(patsubst asm/%.s,$(BUILD)/%.o,$(SFILES))
OFILES_GRAPHICS := $(patsubst %.png,$(BUILD)/%.png.o,$(PNGFILES))
OFILES_BIN := $(patsubst $(DATA)/%,$(BUILD)/%.bin.o,$(BINFILES))

# All object files
OFILES := $(OFILES_C) $(OFILES_S) $(OFILES_GRAPHICS) $(OFILES_BIN)

# Build and dependency directories
VPATH := C asm $(DATA) $(GRAPHICS)
DEPSDIR := $(CURDIR)/$(BUILD)

# Include paths for headers and libraries
INCLUDE := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)
LIBPATHS := -L$(G_ENGINE_DIR)/build

# Build targets
.PHONY: $(BUILD) clean all

# Create build directory
$(BUILD):
	@[ -d $@ ] || mkdir -p $@

# Main build target
all: $(BUILD)/$(TARGET).gba

# Link .elf and .gba files
$(BUILD)/$(TARGET).gba: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@

$(BUILD)/$(TARGET).elf: $(OFILES) $(G_ENGINE)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)

# Compile C source files to object files
$(BUILD)/%.o: C/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble .s files to object files
$(BUILD)/%.o: asm/%.s | $(BUILD)
	$(CC) $(ASFLAGS) -c $< -o $@

# Compile other assets (e.g., PNG to object)
$(BUILD)/%.png.o: %.png | $(BUILD)
	@echo "Processing $<"
	grit $< -fts -o $@

# Compile binary data to object files
$(BUILD)/%.bin.o: $(DATA)/% | $(BUILD)
	@echo "Processing $<"
	bin2o $< $@

# Clean up build artifacts
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).gba $(G_ENGINE)

# Dependency handling
.SECONDARY:
-include $(DEPSDIR)/*.d

# Log some details for debugging
$(info CFILES are $(CFILES))
$(info SFILES are $(SFILES))
$(info OFILES are $(OFILES))
$(info VPATH is: $(VPATH))

