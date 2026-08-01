# tratty

An experimental, low-level pseudo-terminal (PTY) emulator written in C using SDL2 and SDL2_ttf.

`tratty` is currently in **development mode**. It has successfully transitioned from a standard CLI proxy into a graphical terminal emulator prototype. It spawns a pseudo-terminal shell and utilizes an SDL2 hardware-accelerated context to render an active character text grid, multiplexing inputs and outputs directly between system keyboard events and the background PTY process.

---

## 🚀 Features Implemented So Far

- **PTY Session Spawning & Shell Execution**: Forks the current process and establishes a pseudo-terminal session (`forkpty`) executing `/bin/bash` with full master/slave PTY routing.
- **SDL2 Graphical Window Integration**: Discards terminal takeover in favor of an interactive GUI window context using hardware-accelerated rendering.
- **Asynchronous I/O & Event Multiplexing**: Merges a non-blocking PTY reader thread utilizing `select(2)` with the SDL2 event loop (`SDL_PollEvent`) to handle real-time input/output concurrently.
- **Character Grid Layout (Cell Matrix)**: Employs a fixed `24x80` grid (`screen[ROWS][COLS]` of custom `Cell` structures) to maintain terminal characters and viewport memory state.
- **Text Cell Rendering**: Renders individual characters from the grid onto the window layout via `SDL2_ttf`, preconfigured to use JetBrains Mono Nerd Font for optimal monospaced rendering.
- **Interactive Keyboard Input Mapping**: Captures GUI window event inputs via `SDL_TEXTINPUT` and raw key strokes (such as `BACKSPACE`, `RETURN`), writing them directly into the background shell.
- **Basic Control Character Parser**: Decodes core stream control codes—specifically backspaces (`\b`), carriage returns (`\r`), and newlines (`\n`)—to coordinate cursor repositioning and manipulate grid character state.
- **TTY Raw Mode Toggle & Recovery**: Safely configures the active host terminal's discipline via `tcsetattr` (`enableRawMode()`) and guarantees clean `termios` restoration on exit.

---

## 📁 Repository Structure

```text
tratty/
├── Makefile             # GCC compilation with SDL2 and SDL2_ttf linkage
├── include/
│   ├── core.h           # Shared headers, raw mode, and PTY fork declarations
│   ├── renderer.h       # SDL2 window context, framing, and ttf font rendering utilities
│   └── screen.h         # Terminal dimension configuration and Cell struct definition
├── src/
│   ├── core.c           # Raw mode terminal discipline utilities and PTY initialization
│   ├── renderer.c       # SDL2 windowing, frame setup, and text-rendering implementation
│   ├── screen.c         # Allocation of the character cell matrix (24x80 grid)
│   └── tratty.c         # Main execution loop combining SDL2 poll, select(2) on PTY, and character parsing
└── assets/
    └── fonts/           # Hand-picked font assets for terminal grid layout:
        ├── Monocraft.ttc
        ├── Perfect DOS VGA 437 Win.ttf
        ├── Perfect DOS VGA 437.ttf
        ├── JetBrains/
        │   ├── JetBrains-Mono-Nerd-Font-Complete.ttf
        │   └── JetBrainsMonoNerdFont-Regular.ttf
        └── MapleMono/
            └── ...
```

---

## 🛠️ Prerequisites

To compile and run `tratty`, ensure you have standard C build utilities and SDL2 development libraries installed.

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
To run the compiled terminal emulator GUI:
```bash
make run
```
*Once running, an SDL2 window will open, routing user keyboard strokes to `/bin/bash` running inside the PTY and displaying text output in a custom monospace font grid.*

### 3. Clean Build Files
To remove the compiled binary:
```bash
make clean
```

---

## 🗺️ Roadmap & Next Steps

With basic GUI terminal rendering and input/output multiplexing in place, upcoming milestones include:

1. **VT100 / ANSI Escape Sequence Parser**: Build a full ANSI escape sequence state machine parser to handle terminal color codes, text attributes (bold, underline), and cursor movement commands (e.g. `\e[H`, `\e[2J`).
2. **Scrolling and History Buffer**: Add virtual screen buffers to capture off-grid historical content and allow scrollback navigation.
3. **Dynamic Window Resizing**: Capture SDL window resize events to dynamically scale the character cell grid and notify the PTY child process (`ioctl` with `TIOCSWINSZ`).
4. **Enhanced Keyboard Mapping**: Translate additional special keys (arrow keys, Esc, Ctrl, Alt, etc.) into standard ANSI escape sequences for proper CLI interaction.
