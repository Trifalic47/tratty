# tratty

An experimental, low-level pseudo-terminal (PTY) proxy and terminal emulator stub written in C.

`tratty` is currently in **early beta/development mode**. It sets up raw terminal interactions and multiplexes standard input/output directly with a spawned pseudo-terminal shell. The project includes dependency configurations for SDL2 and SDL2_ttf, paving the way for transition into a full GUI-rendered terminal emulator.

---

## 🚀 Features Implemented So Far

- **PTY Session Spawning & Shell Execution**: Forks the process and sets up a new pseudo-terminal session (`forkpty`) executing `/bin/bash` with proper master/slave PTY routing.
- **Asynchronous I/O Multiplexing**: Uses `select(2)` to monitor both the host terminal's input (`STDIN_FILENO`) and the master TTY's output (`masterfd`) in a non-blocking loop.
- **TTY Raw Mode Toggle**: Programmatically changes the host terminal's line-discipline via `tcsetattr` (`enableRawMode()`) to disable canonical mode, echo, and default signal handlers. This ensures all keys and escape codes are captured directly.
- **Clean State Recovery**: Gracefully restores the host terminal's original `termios` state upon program termination or on encountering errors.
- **Graphic Engine Boilerplate**: Preconfigured to link against **SDL2** and **SDL2_ttf** (`Makefile` and `include/core.h`), preparing the architecture for transition to visual window rendering.
- **Asset Bundling**: Includes a rich collection of high-quality monospace and nerd fonts (under `assets/fonts/`) intended for custom text-grid rendering.

---

## 📁 Repository Structure

```text
tratty/
├── Makefile             # GCC compilation with SDL2 and SDL2_ttf linkage
├── include/
│   └── core.h           # Shared headers, includes, page-size, and raw mode declaration
├── src/
│   ├── core.c           # Raw mode terminal discipline utilities
│   └── tratty.c         # TTY multiplexer & process fork main entry
└── assets/
    └── fonts/           # Hand-picked font assets for terminal grid layout:
        ├── CascadiaCove/
        ├── JetBrains/
        ├── MapleMono/
        ├── Mononoki/
        ├── Noto_Sans/
        └── ...
```

---

## 🛠️ Prerequisites

To compile and run `tratty`, ensure you have the standard C build utilities and SDL2 development libraries installed.

### Debian / Ubuntu
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev
```

### Fedora / RHEL
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install SDL2-devel SDL2_ttf-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel sdl2 sdl2_ttf
```

---

## 📦 Building & Running

A simple Makefile is provided to build and run the binary easily.

### 1. Compile the Project
To compile the `tratty` executable (output is saved to `build/tratty`):
```bash
make
```

### 2. Run the Application
To run the compiled TTY shell wrapper:
```bash
make run
```
*Once running, you will be interacting directly with a raw-mode `/bin/bash` shell running inside the `tratty` PTY broker.*

### 3. Clean Build Files
To remove the compiled binary:
```bash
make clean
```

---

## 🗺️ Roadmap & Next Steps

Currently, `tratty` runs as a raw terminal proxy, routing standard TTY input/output. The planned milestones for this project include:

1. **SDL2 Window Initialization**: Creating a GUI window context instead of taking over the active terminal.
2. **Text Grid Layout & Cell Rendering**: Utilizing the bundled fonts in `assets/fonts/` with `SDL2_ttf` to render individual character cells on a texture grid.
3. **VT100 / ANSI Escape Sequence Parser**: Implementing a parser to interpret terminal color codes, cursor movements, and text styling commands.
4. **Input Handling & Custom Keymaps**: Capturing keyboard events via SDL2 events and translating them into standard ANSI sequences to send to the master TTY.
