#environment setup
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/gba_rules

#directory and file paths
ifndef TOPDIR
export TOPDIR := $(CURDIR)
endif

LIBRARY_DIR := $(abspath $(TOPDIR)/../..)
LIBRARY := $(LIBRARY_DIR)/build/g_engine.a

$(info LIBRARY_DIR is $(LIBRARY_DIR))
$(info LIBRARY path is $(LIBRARY))

#build targets and source directories
TARGET := $(notdir $(CURDIR))
BUILD := build
SOURCES := C asm

#include and asset directorie
INCLUDES := $(LIBRARY_DIR)/src/include $(LIBRARY_DIR)/build 
DATA := 
MUSIC := 
GRAPHICS :=

#Compiler and Linker flags
ARCH := -mthumb -mthumb-interwork
CFLAGS := -g -Wall -O2\
	-mcpu=arm7tdmi -mtune=arm7tdmi\
	$(ARCH)

CFLAGS += $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -g $(ARCH) -Wl,-Map,$(notdir $*.map) -specs=gba.specs

#Toolchain definitions
CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

#libraries and paths
LIBS := $(LIBRARY)
LIBDIRS := $(LIBRARY_DIR)/build

#build in
ifneq ($(BUILD),$(notdir $(CURDIR)))

#exported vars/file lists-----
export OUTPUT := $(TOPDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(TOPDIR)/$(dir)) \
		$(foreach dir,$(DATA),$(TOPDIR)/$(dir)) \
		$(foreach dir,$(GRAPHICS),$(TOPDIR)/$(dir))
export DEPSDIR := $(TOPDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PNGFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
 
#Conditional logic for music files
ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
endif

#language based linker selection
ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

#object and header files
export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES_GRAPHICS := $(PNGFILES:.png:.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES) $(OFILES_GRAPHICS)
export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES))) $(PNGFILES:.png:.h)

$(info CFILES are $(CFILES))
$(info SFILES are $(SFILES))
$(info OFILES_SOURCES are $(OFILES_SOURCES))
$(info OFILES are $(OFILES))



#include paths and library paths, for headers and libs
export INCLUDE := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)
export LIBPATHS := -L$(CURDIR)/$(LIBRARY_DIR)/build

#Build Targets
.PHONY: $(BUILD) clean all
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	$(info Current SFILES: $(SFILES))
	$(info Current VPATH: $(VPATH))
	$(info Current OFILES_SOURCES: $(OFILES_SOURCES))
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba $(LIBRARY)
else
#build rules else block
all: $(LIBRARY) $(OUTPUT).gba
$(LIBRARY):
	$(MAKE) -C $(LIBRARY_DIR)
$(OUTPUT).gba: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES) $(LIBRARY)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)
$(OFILES_SOURCES): $(HFILES)
# additional asset rules
soundbank.bin soundbank.h: $(AUDIOFILES)
	@mmutil $^ -osoundbank.bin -hsoundbank.h
%.bin.o %_bin.h: %.bin
	@echo $(notdir $<)
	@$(bin2o)
%.s %.h: %.png %.grit
	@echo "grit $(notdir $<)"
	@grit $< -fts -o$*
.SECONDARY:
-include $(DEPSDIR)/*.d
$(info Updated SOURCES are $(SOURCES))
$(info Updated SFILES are $(SFILES))
$(info Updated OFILES_SOURCES are $(OFILES_SOURCES))
endif
