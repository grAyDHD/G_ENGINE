# -------------------------------------------
# Define some directories;
# -------------------------------------------
SRCDIR  = c:\devkitadv\code\soundtest3\voice1
CMPDIR  = c:\DevKitAdv\bin
LIBDIR  = c:\DevKitAdv\lib\gcc-lib\arm-agb-elf\3.0.2\interwork
LIBDIR2 = c:\devkitadv\arm-agb-elf\lib\interwork
INCDIR  = c:\devkitadv\lib\gcc-lib\arm-agb-elf\3.0.2\include
INCDIR2 = c:\devkitadv\arm-agb-elf\include
INCDIR3 = c:\devkitadv\include
INCDIR4 = c:\devkitadv\nylib

# -------------------------------------------
# Define what extensions we use;
# -------------------------------------------
.SUFFIXES : .c .s

# -------------------------------------------
# Define the flags for the compilers;
# -------------------------------------------
CFLAGS  = -I $(INCDIR4) -I $(INCDIR3) -I $(INCDIR2) -I $(INCDIR) -I $(SRCDIR) -mthumb-interwork -c -g  -Wall
SFLAGS  = -I $(INCDIR4) -I $(INCDIR3) -I $(INCDIR2) -I $(INCDIR) -mthumb-interwork
LDFLAGS = -L $(LIBDIR) -L $(LIBDIR2) -T lnkscript

# -------------------------------------------
# Define the list of all O files;
# -------------------------------------------
O_FILES			 = crt0.o		\
				   crtbegin.o	\
				   crtend.o		\
				   main.o		\
				   font.o		\
				   fontdata.o	\
				   logo.o		\
				
				   
# -------------------------------------------
# Define the final dependecy;
# -------------------------------------------
all : soundtest1.gba

# -------------------------------------------
# Define the copy from .elf to .bin file
# -------------------------------------------
soundtest1.gba : rom.elf
	objcopy -v -O binary rom.elf soundtest1.gba
	-@echo ------------------------------------------ 
	-@echo Done 
	-@echo ------------------------------------------ 

# -------------------------------------------
# Define the linker instruction;
# -------------------------------------------
rom.elf : $(O_FILES)
	ld $(LDFLAGS) -o rom.elf $(O_FILES) -lc 
	-@echo ------------------------------------------ 
	-@echo Linking Done
	-@echo ------------------------------------------ 

# -------------------------------------------
# Define each compile;
# -------------------------------------------
{$(SRCDIR)}.c.o::
	gcc  $(CFLAGS) $< 
	-@echo ------------------------------------------ 
	-@echo C-Sources Compiled 
	-@echo ------------------------------------------ 

# -------------------------------------------
# Define each assemble;
# -------------------------------------------
{$(SRCDIR)}.s.o:
	as $(SFLAGS) $(SRCDIR)\$*.s -o$@
	-@echo ------------------------------------------ 
	-@echo ASM-Sources Compiled 
	-@echo ------------------------------------------ 
	

# -------------------------------------------
# EOF;

