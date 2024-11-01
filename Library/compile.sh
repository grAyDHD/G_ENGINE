#!/bin/zsh

# Get the current project directory
PROJECT_PATH=$(pwd)
PROJECT_NAME=$(basename "$PROJECT_PATH")
BUILD_DIR="$PROJECT_PATH/build"
LIB_DIR="$HOME/Code/GBA/G_ENGINE/07_lib/build"

# Check if the C directory exists in the current directory
if [[ ! -d "$PROJECT_PATH/C" ]]; then
    echo "Error: This script should be run in a project directory containing a 'C' directory."
    exit 1
fi

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Compile all .c files in the C directory
echo "Compiling C files in $PROJECT_PATH/C"
for c_file in $PROJECT_PATH/C/*.c; do
    if [[ -f $c_file ]]; then
        obj_file="$BUILD_DIR/$(basename ${c_file%.*}).o"
        echo "Compiling $c_file to $obj_file"
        arm-none-eabi-gcc -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork \
        -I"$PROJECT_PATH/C" \
        -I"$LIB_DIR/../C/include" \
        -c "$c_file" -o "$obj_file"
    fi
done

# Check if the asm directory exists and has .s files
if [[ -d "$PROJECT_PATH/asm" && $(ls -1 $PROJECT_PATH/asm/*.s 2>/dev/null | wc -l) -gt 0 ]]; then
    echo "Assembling assembly files in $PROJECT_PATH/asm"
    for s_file in $PROJECT_PATH/asm/*.s; do
        if [[ -f $s_file ]]; then
            obj_file="$BUILD_DIR/$(basename ${s_file%.*}).o"
            echo "Assembling $s_file to $obj_file"
            arm-none-eabi-gcc -g -mthumb -mthumb-interwork -c "$s_file" -o "$obj_file"
        fi
    done
else
    echo "No assembly files found in $PROJECT_PATH/asm"
fi

# Collect all object files in the build directory
OBJECT_FILES=($BUILD_DIR/*.o)

# Link the object files and library into an ELF
echo "Linking object files and library into ELF"
arm-none-eabi-gcc -g -mthumb -mthumb-interwork -Wl,-Map="$BUILD_DIR/$PROJECT_NAME.map" -specs=gba.specs \
-o "$BUILD_DIR/$PROJECT_NAME.elf" "${OBJECT_FILES[@]}" \
-L"$LIB_DIR" -lg_engine

# Convert the ELF to a GBA ROM
echo "Converting ELF to GBA ROM"
arm-none-eabi-objcopy -v -O binary "$BUILD_DIR/$PROJECT_NAME.elf" "$BUILD_DIR/$PROJECT_NAME.gba"

# Check if the ROM was created successfully
if [[ -f "$BUILD_DIR/$PROJECT_NAME.gba" ]]; then
    echo "GBA ROM created successfully: $BUILD_DIR/$PROJECT_NAME.gba"
else
    echo "Failed to create GBA ROM."
fi
