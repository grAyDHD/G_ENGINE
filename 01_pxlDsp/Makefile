ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif
include $(DEVKITARM)/gba_rules

G_ENGINE_DIR := ../../07_lib
G_ENGINE := $(G_ENGINE_DIR)/build/g_engine.a


TARGET := $(notdir $(CURDIR))
BUILD := build
SOURCES := C

INCLUDES := $(G_ENGINE_DIR)/C/include $(G_ENGINE_DIR)/build 
DATA := 
MUSIC := 
GRAPHICS := graphics


ARCH := -mthumb -mthumb-interwork
CFLAGS := -g -Wall -O2\
	-mcpu=arm7tdmi -mtune=arm7tdmi\
	$(ARCH)

CFLAGS += $(foreach dir,$(INCLUDES),-I$(dir))
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS := -g $(ARCH)
LDFLAGS := -g $(ARCH) -Wl,-Map,$(notdir $*.map) -specs=gba.specs

CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

LIBS := $(G_ENGINE)
LIBDIRS := $(G_ENGINE_DIR)/build


ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)


export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
		$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
		$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PNGFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
endif

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

export OFILES_BIN := $(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s:.o)
export OFILES_GRAPHICS := $(PNGFILES:.png:.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES) $(OFILES_GRAPHICS)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES))) $(PNGFILES:.png:.h)

export INCLUDE := $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS := -L$(G_ENGINE_DIR)/build

.PHONY: $(BUILD) clean all


$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba $(G_ENGINE)
else

all: $(OUTPUT).gba

$(OUTPUT).gba: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES) $(G_ENGINE)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS)

$(OFILES_SOURCES): $(HFILES)

$(G_ENGINE):
	$(MAKE) -C $(G_ENGINE_DIR) build/g_engine.a


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
endif
