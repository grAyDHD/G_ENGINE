#!/bin/zsh

# Define project paths
PROJECT_PATH="$HOME/Code/GBA/G_ENGINE/13_inlineASM"
BUILD_DIR="$PROJECT_PATH/build"
LIB_DIR="$HOME/Code/GBA/G_ENGINE/07_lib/build"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Compile the C file
echo "Compiling C file"
arm-none-eabi-gcc -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork \
-I"$HOME/Code/GBA/G_ENGINE/13_inlineASM/C" \
-I"$HOME/Code/GBA/G_ENGINE/07_lib/C/include" \
-L"$HOME/Code/GBA/G_ENGINE/07_lib/build" \
-c "$PROJECT_PATH/C/13_inlineASM.c" -o "$BUILD_DIR/13_inlineASM.o"

# Assemble the assembly file
echo "Assembling assembly file"
arm-none-eabi-gcc -g -mthumb -mthumb-interwork -c "$PROJECT_PATH/asm/asmFuncs.s" -o "$BUILD_DIR/asmFuncs.o"

# Link the object files and library into an ELF
echo "Linking object files and library into ELF"
arm-none-eabi-gcc -g -mthumb -mthumb-interwork -Wl,-Map="$BUILD_DIR/13_inlineASM.map" -specs=gba.specs \
-o "$BUILD_DIR/13_inlineASM.elf" "$BUILD_DIR/13_inlineASM.o" "$BUILD_DIR/asmFuncs.o" \
-L"$HOME/Code/GBA/G_ENGINE/07_lib/build" -lg_engine

# Convert the ELF to a GBA ROM
echo "Converting ELF to GBA ROM"
arm-none-eabi-objcopy -v -O binary "$BUILD_DIR/13_inlineASM.elf" "$BUILD_DIR/13_inlineASM.gba"

# Check if the ROM was created successfully
if [[ -f "$BUILD_DIR/13_inlineASM.gba" ]]; then
    echo "GBA ROM created successfully: $BUILD_DIR/13_inlineASM.gba"
else
    echo "Failed to create GBA ROM."
fi
