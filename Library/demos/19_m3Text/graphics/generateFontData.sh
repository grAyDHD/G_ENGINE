#!/bin/bash

# Input Markdown file
input_file="fontTable.md"
output_file="fontData.c"

# Initialize variables
xOffset=0

# Start writing the C file
cat <<EOL > "$output_file"
#include <stdint.h>

typedef struct {
  uint16_t xOffset;
  uint16_t yOffset;
  uint16_t width;
  uint16_t height;
} GlyphInfo;

GlyphInfo fontData[94] = {
EOL

# Process the table
tail -n +3 "$input_file" | while IFS='|' read -r _ char ht wd y _; do
    # Trim whitespace
    char=$(echo "$char" | tr -d '[:space:]')
    ht=$(echo "$ht" | tr -d '[:space:]')
    wd=$(echo "$wd" | tr -d '[:space:]')
    y=$(echo "$y" | tr -d '[:space:]')

    # Skip empty lines
    if [[ -z "$char" || -z "$ht" || -z "$wd" || -z "$y" ]]; then
        continue
    fi

    # Debugging output
    echo "Processing char: $char, ht: $ht, wd: $wd, y: $y"

    # Assign index based on character
    case "$char" in
        [A-Z])
            index=$(( $(printf "%d" "'$char") - $(printf "%d" "'A") )) ;;
        [a-z])
            index=$(( $(printf "%d" "'$char") - $(printf "%d" "'a") + 26 )) ;;
        [0-9])
            index=$(( $(printf "%d" "'$char") - $(printf "%d" "'0") + 52 )) ;;
        *)
            # Handle special characters explicitly
            index=$((index + 1)) ;; # Assign indices sequentially
    esac

    # Skip characters outside the range
    if [[ $index -lt 0 || $index -ge 94 ]]; then
        continue
    fi

    # Write the data to the file
    printf "  { %d, %s, %s, %s }, // '%s'\n" "$xOffset" "$y" "$wd" "$ht" "$char" >> "$output_file"

    # Update xOffset for the next character
    xOffset=$((xOffset + wd))
done

# End the C file
echo "};" >> "$output_file"

echo "Generated $output_file successfully."
