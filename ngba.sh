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
PROJECT_C_DIR="$PROJECT_DIR/C"
PROJECT_MAIN_C="$PROJECT_C_DIR/$PROJECT_NAME.c"

# Create the project directory
mkdir -p "$PROJECT_C_DIR"

# Copy the Makefile template into the new project directory
cp "$MFT_TEMPLATE" "$PROJECT_DIR/Makefile"

# Create the main C file with a basic template
cat <<EOL > "$PROJECT_MAIN_C"
int main() {

    return 0;
}
EOL

# Change to the project directory
cd "$PROJECT_DIR" || exit

# Run bear to generate compile_commands.json
bear -- make

# Remove the incompatible "-mthumb-interwork" line from compile_commands.json
sed -i '/-mthumb-interwork/d' compile_commands.json

echo "Project $PROJECT_NAME setup complete!"
