#!/bin/bash

# Check if a project name was provided
if [ -z "$1" ]; then
    echo "Usage: ngba <project_name>"
    exit 1
fi

# Define project name and paths
PROJECT_NAME=$1
G_ENGINE_PATH=$(dirname "$(realpath "$0")")
PROJECT_DIR="$G_ENGINE_PATH/$PROJECT_NAME"
MFT_TEMPLATE="$G_ENGINE_PATH/MFT"

# Define all project subdirectories and files
PROJECT_C_DIR="$PROJECT_DIR/C"
PROJECT_ASM_DIR="$PROJECT_DIR/asm"
PROJECT_GRAPHICS_DIR="$PROJECT_DIR/graphics"
PROJECT_INCLUDES_DIR="$PROJECT_DIR/includes"
PROJECT_MAIN_C="$PROJECT_C_DIR/$PROJECT_NAME.c"
PROJECT_HEADER_H="$PROJECT_INCLUDES_DIR/$PROJECT_NAME.h"

# Prompt the user for video mode
echo "Select a video mode for the project:"
PS3="Enter your choice: "
options=("Mode 3" "Mode 4" "Cancel")
select opt in "${options[@]}"; do
    case $opt in
        "Mode 3")
            VIDEO_MODE="MODE3 | BG2"
            break
            ;;
        "Mode 4")
            VIDEO_MODE="MODE4 | BG2"
            break
            ;;
        "Cancel")
            echo "Setup canceled."
            exit 0
            ;;
        *)
            echo "Invalid option. Please select a valid video mode."
            ;;
    esac
done

# Ensure all necessary directories exist
mkdir -p "$PROJECT_C_DIR" "$PROJECT_ASM_DIR" "$PROJECT_INCLUDES_DIR" "$PROJECT_GRAPHICS_DIR"

# Copy the Makefile template into the new project directory
cp "$MFT_TEMPLATE" "$PROJECT_DIR/Makefile"

# Create a project-specific header file in the includes directory
# Use uppercase for macro names and ensure proper include guard
cat > "$PROJECT_HEADER_H" << EOL
#ifndef ${PROJECT_NAME^^}_H
#define ${PROJECT_NAME^^}_H

// Project-specific constants and declarations
#define PROJECT_NAME "${PROJECT_NAME}"

// Video mode configuration
#define PROJECT_VIDEO_MODE ${VIDEO_MODE}

// Add more project-specific macros or declarations here

#endif // ${PROJECT_NAME^^}_H
EOL

# Verify header file was created
if [ ! -f "$PROJECT_HEADER_H" ]; then
    echo "Error: Failed to create header file at $PROJECT_HEADER_H"
    exit 1
fi

# Create the main C file with a basic template
cat > "$PROJECT_MAIN_C" << EOL
#include "../includes/${PROJECT_NAME}.h"
#include "graphics/video.h"

int main() {
    DSPC = ${VIDEO_MODE};

    return 0;
}
EOL

# Change to the project directory
cd "$PROJECT_DIR" || exit

# Run bear to generate compile_commands.json
bear -- make

# Remove the incompatible "-mthumb-interwork" line from compile_commands.json
sed -i '/-mthumb-interwork/d' compile_commands.json

echo "Project $PROJECT_NAME setup complete with video mode $opt!"
