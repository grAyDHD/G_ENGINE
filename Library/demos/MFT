.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

ifndef TOPDIR
export TOPDIR := $(CURDIR)
endif

LIBRARY_DIR := $(abspath $(TOPDIR)/../..)
LIBRARY := $(LIBRARY_DIR)/build/g_engine.a

TARGET := $(notdir $(CURDIR))
BUILD := build
SOURCES := C asm

#include and asset directories
INCLUDES := $(LIBRARY_DIR)/src/include 
DATA := 
MUSIC := 
GRAPHICS := graphics

ARCH	:=	-mthumb -mthumb-interwork
CFLAGS	:=	-g -Wall -O2\
		-mcpu=arm7tdmi -mtune=arm7tdmi\
		$(ARCH)
CFLAGS	+=	$(INCLUDE)
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)


#libraries and paths
LIBS := $(LIBRARY)
LIBDIRS := $(LIBRARY_DIR)/build

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(TOPDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCES),$(TOPDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(TOPDIR)/$(dir)) \
			$(foreach dir,$(GRAPHICS),$(TOPDIR)/$(dir))
export DEPSDIR	:=	$(TOPDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES	:=	$(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
	$(info AUDIOFILES: $(AUDIOFILES))
endif

ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES_GRAPHICS := $(PNGFILES:.png=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES) $(OFILES_GRAPHICS)
export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES))) $(PNGFILES:.png=.h)

# Debug output for object and header files
$(info Object files: $(OFILES))
$(info Header files: $(HFILES))

export INCLUDE := $(foreach dir,$(INCLUDES),-iquote $(dir)) -I$(CURDIR)/$(BUILD)
export LIBPATHS := -L$(LIBRARY_DIR)/build
###export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir))

# Debug output for includes and library paths
$(info INCLUDE paths: $(INCLUDE))
$(info LIBPATHS: $(LIBPATHS))

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba

else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
###all: $(LIBRARY) $(OUTPUT).gba
###$(LIBRARY):
###	$(MAKE) -C $(LIBRARY_DIR)

$(OUTPUT).gba	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)

###$(OUTPUT).gba: $(OUTPUT).elf
###	@echo "Creating GBA binary from ELF..."
###	@$(OBJCOPY) -O binary $< $@

###$(OUTPUT).elf: $(OFILES) $(LIBRARY)
###	@echo "Linking ELF file..."
###	@$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)

# Debug output for linking phase
$(info Linking $(OUTPUT).elf with object files: $(OFILES))
$(info Linking with libraries: $(LIBS))
$(OFILES_SOURCES) : $(HFILES)

soundbank.bin soundbank.h : $(AUDIOFILES)
	@mmutil $^ -osoundbank.bin -hsoundbank.h

%.bin.o	%_bin.h :	%.bin
	@echo $(notdir $<)
	@$(bin2o)

%.s %.h: %.png %.grit
	@echo "Processing PNG file $(notdir $<) with Grit..."
	@grit $< -fts -o $*

.SECONDARY:

-include $(DEPSDIR)/*.d
#---------------------------------------------------------------------------------------
endif#---------------------------------------------------------------------------------------
