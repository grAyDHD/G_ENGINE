# GBA Text Drawing Program Design Document

## Project Overview
The sole purpose of this project is implementing text in mode 3 to start, eventually in all other modes.
The general flow should be, include a bitmap font in png format in graphics, load it in the program,
Create a function that can parse text strings of chars, and point to the relevant char in the font image file
drawing that to the screen.
2 separate implementations, only starting with the first of the two:

First:
    give function an x,y coordinate and string 
    for a given char, print at x, y, create temp x and increment it be font width + distance between chars
    on \n char, return to start x and increment y by (font height + distance between lines in pxls) * 240

Second:
    give function a UI frame object (will add after next project- develop UI framework)
    will incorporate checks to stay within the frame objects xy boundaries, with padding

---

## Steps for Implementation

### Step 1:
    Print basic font as entire alphabet/number sequence
[] Add bitmap font in .png format to graphics directory
[] Print entire font bitmap to screen as one image
[] Print entire font bitmap as individual characters

### Step 2:
    Font Data Integration
- [ ] Define a data structure for font glyphs (e.g., 8x8 pixel bitmaps).
- [ ] Implement a function to extract glyph data for a given character.

### Step 3:
    Text Drawing Function
- [ ] Implement a `drawCharacter(x, y, char, color)` function.
  - [ ] Draw a single character at specified coordinates.
  - [ ] Use a single solid color for the character.

- [ ] Implement a `drawText(x, y, *text, color)` function.
  - [ ] Draw multiple characters as a single line of text.
  - [ ] Verify correct spacing between characters.

### Step 4:
    Text Color and Background Handling
- [ ] Add support for customizable text colors (16-bit RGB15).
- [ ] Implement a background color option for text rendering.
- [ ] Ensure background does not overwrite other screen content.


### Step 5: 
    Multi-Line Text Support
- [ ] Add support for line breaks (e.g., `\n` or a predefined width).

- [ ] Implement a `draw_multiline_text(x, y, *text, width, color)` function.
  - [ ] Automatically wrap text when reaching the specified width.
  - [ ] Add line spacing options.

- [ ] Verify rendering correctness for multiple lines with varied lengths.


### Step 6:
    Advanced Features
- [ ] Implement text scaling (e.g., 2x or 4x size).
- [ ] Add gradient text coloring options.
- [ ] Create a text shadow/outline effect.
- [ ] Support dynamic text updates (e.g., real-time counters).
- [ ] Enable smooth scrolling for long text strings.

### Step 7:
    Testing and Debugging
- [ ] Develop test cases for each function.
- [ ] Ensure rendering correctness for all edge cases (e.g., extreme alignments, overlapping lines).
- [ ] Test program stability with rapid text updates.
- [ ] Verify compatibility with other Mode 3 features (e.g., images).

### Step 8:
    Documentation and Finalization
- [ ] Document all public functions and their usage.
- [ ] Create example programs demonstrating text rendering features.
- [ ] Write a README explaining the library's features and limitations.
- [ ] Perform final code review and cleanup.

---

## Additional Features and Ideas
- once UI framework is created:
- [ ] Implement text selection feature (for things like naming a character in a game)
- [ ] Add horizontal alignment options: left, center, right.
- [ ] Add vertical alignment options: top, middle, bottom.
- [ ] Implement text positioning relative to a bounding box.
- [ ] Test all alignment combinations with multi-line text.

---

## Memory and Performance Considerations

- Mode 3 uses 240x160 pixels and requires careful VRAM management.
- Font data and rendering functions will prioritize minimal memory usage.
- Rendering operations will be aligned with VBLANK to avoid tearing or flickering.
