#!/bin/zsh

# Get the current project directory
PROJECT_PATH=$(pwd)
PROJECT_NAME=$(basename "$PROJECT_PATH")
BUILD_DIR="$PROJECT_PATH/build"

# Navigate up to G_ENGINE/Library directory to set proper paths
LIBRARY_PATH=$(dirname $(dirname "$PROJECT_PATH"))
INCLUDE_DIR="$LIBRARY_PATH/include"
LIB_DIR="$LIBRARY_PATH/build"

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
        base_name=$(basename ${c_file%.*})
        obj_file="$BUILD_DIR/${base_name}.o"
        asm_file="$BUILD_DIR/${base_name}.s"
        
        # Generate assembly file
        echo "Generating assembly for $c_file"
  arm-none-eabi-gcc -Os -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork -fno-asynchronous-unwind-tables -fomit-frame-pointer \
        -I"$PROJECT_PATH/C" \
        -I"$INCLUDE_DIR" \
        -S "$c_file" -o "$asm_file"
        
        if [ $? -ne 0 ]; then
            echo "Error: Assembly generation failed for $c_file"
            exit 1
        fi
        
        # Compile to object file
        echo "Compiling $c_file to $obj_file"
        arm-none-eabi-gcc -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -mthumb -mthumb-interwork \
        -I"$PROJECT_PATH/C" \
        -I"$INCLUDE_DIR" \
        -c "$c_file" -o "$obj_file"
        
        if [ $? -ne 0 ]; then
            echo "Error: Compilation failed for $c_file"
            exit 1
        fi
    fi
done

# Check if the asm directory exists and has .s files
if [[ -d "$PROJECT_PATH/asm" ]]; then
    # Use nullglob to handle case where no .s files exist
    setopt nullglob
    asm_files=($PROJECT_PATH/asm/*.s)
    if (( ${#asm_files[@]} > 0 )); then
        echo "Assembling assembly files in $PROJECT_PATH/asm"
        for s_file in "${asm_files[@]}"; do
            obj_file="$BUILD_DIR/$(basename ${s_file%.*}).o"
            echo "Assembling $s_file to $obj_file"
            arm-none-eabi-gcc -g -mthumb -mthumb-interwork -c "$s_file" -o "$obj_file"
            
            if [ $? -ne 0 ]; then
                echo "Error: Assembly failed for $s_file"
                exit 1
            fi
        done
    else
        echo "No assembly files found in $PROJECT_PATH/asm"
    fi
    unsetopt nullglob
fi

# Check if any object files were created
if [[ ! -n "$(ls -A $BUILD_DIR/*.o 2>/dev/null)" ]]; then
    echo "Error: No object files were created in $BUILD_DIR"
    exit 1
fi

# Collect all object files in the build directory
OBJECT_FILES=($BUILD_DIR/*.o)

# Link the object files and library into an ELF
echo "Linking object files and library into ELF"
arm-none-eabi-gcc -g -mthumb -mthumb-interwork -Wl,-Map="$BUILD_DIR/$PROJECT_NAME.map" -specs=gba.specs \
-o "$BUILD_DIR/$PROJECT_NAME.elf" "${OBJECT_FILES[@]}" \
-L"$LIB_DIR" -l:g_engine.a

if [ $? -ne 0 ]; then
    echo "Error: Linking failed"
    exit 1
fi

# Generate assembly listing with source code
echo "Generating assembly listing"
arm-none-eabi-objdump -S -d "$BUILD_DIR/$PROJECT_NAME.elf" > "$BUILD_DIR/$PROJECT_NAME.lst"

if [ $? -ne 0 ]; then
    echo "Warning: Failed to generate assembly listing"
else
    echo "Assembly listing created: $BUILD_DIR/$PROJECT_NAME.lst"
fi

# Convert the ELF to a GBA ROM
echo "Converting ELF to GBA ROM"
arm-none-eabi-objcopy -v -O binary "$BUILD_DIR/$PROJECT_NAME.elf" "$BUILD_DIR/$PROJECT_NAME.gba"

# Check if the ROM was created successfully
if [[ -f "$BUILD_DIR/$PROJECT_NAME.gba" ]]; then
    echo "GBA ROM created successfully: $BUILD_DIR/$PROJECT_NAME.gba"
else
    echo "Failed to create GBA ROM."
    exit 1
fi
