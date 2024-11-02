#G_ENGINE

## Overview
**G_ENGINE** is a library and toolset for GameBoy Advance (GBA) development, built from scratch for optimal performance and versatility. The project consists of two main components:

- **Library**: A custom C library for GBA development, currently primarily focused on bitmap modes (Modes 3, 4, and 5).
- **NG**: A collection of Angular-based modules focused on simplified asset management with a GUI, with the ultimate intent of building a fully featured web-based GBA development engine. *Note: NG is currently in the setup phase* 

## Features

- **Library** (GBA C Library)
  - Optimized for GBA bitmap display modes: **Mode 3**, **Mode 4**, and **Mode 5**
  - VRAM management and low-level graphics handling
  - Planned expansions for tiled modes and additional hardware features

- **NG** (Web Development Engine - Coming Soon)
  - Framework for creating a GBA development environment accessible online or locally
  - Modular design for flexibility and scalability

  
## Getting Started

### Prerequisites
- **devkitARM / devkitPro** - for compiling the GBA C library
- **Neovim** - recommended for syntax highlighting and debugging
- **mGBA** - for emulating and testing your GBA projects

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/username/G_ENGINE.git
   cd G_ENGINE
   ```

2. **Build the Library**
   ```bash
   make -C Library
   ```

3. **Run in Emulator**
   ```bash
   mgba Library/build/output.gba
   ```


## Project Structure

- **Library/**: Core GBA library (written in C) for game development
- **NG/**: Angular-based project for the development environment (in progress)
- **README.md**: Project documentation


## Usage

To start experimenting with the GBA library, follow these steps:

```bash
# Compile a demo (replace 'demo_name' with a specific example)
make -C Library demos/demo_name



### 7. **Contributing**
Encourage contributions with a simple guide or link to `CONTRIBUTING.md` (if you have one).

```markdown
## Contributing

Contributions are welcome! To get started, please check the open issues or submit a pull request. For major changes, please open an issue to discuss them first.

