# 🟡 PAC-MAN

> **A modern, high-performance C++ implementation of the classic Pac-Man arcade game with advanced AI and stunning OpenGL graphics.**

![C++](https://img.shields.io/badge/C++-17-blue?style=flat-square&logo=cplusplus)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-green?style=flat-square&logo=opengl)
![CMake](https://img.shields.io/badge/CMake-3.10+-red?style=flat-square&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

---

##  Features

###  **Classic Gameplay**
- **Authentic Pac-Man Experience** - Navigate mazes, collect dots, avoid ghosts
- **Perfect Collision Detection** - Bulletproof wall collision system
- **Smart Movement Controls** - Intelligent direction changes prevent getting stuck
- **Power Pellets** - Turn the tables and chase the ghosts!

###  **Advanced Ghost AI**
- **4 Unique Ghost Personalities**:
  -  **Blinky (Red)** - Aggressive direct pursuit
  -  **Pinky (Pink)** - Strategic ambush tactics
  -  **Inky (Cyan)** - Complex patrol patterns
  -  **Clyde (Orange)** - Unpredictable behavior
- **Multiple Pathfinding Algorithms** - Dijkstra, A*, and Backtracking
- **Dynamic AI** - Ghosts adapt to your playstyle

###  **Visual Excellence**
- **OpenGL-Powered Graphics** - Hardware-accelerated rendering
- **Particle Effects** - Beautiful visual feedback for game events
- **Smooth 60 FPS Gameplay** - Fluid character animations
- **Classic Aesthetic** - Faithful to the original Pac-Man design

###  **Game Features**
- **Progressive Difficulty** - Increasingly challenging levels
- **Multiple Lives System** - Visual life indicators
- **Power-Up Mechanics** - Temporary ghost vulnerability
- **Level Progression** - Complete mazes to advance

---

##  Quick Start

### Prerequisites
- **C++17** compatible compiler (MSVC, GCC, Clang)
- **CMake 3.10+**
- **OpenGL 3.3+**
- **GLFW3** and **GLEW** libraries

### Build Instructions

#### Windows
```cmd
# Clone the repository
git clone https://github.com/Dhuvie/Pac-man.git
cd Pac-man

# Build using the provided script
build.bat

# Or build manually
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Run the game
cd Release
Pacman.exe
```

#### Linux/Mac
```bash
# Clone the repository
git clone https://github.com/Dhuvie/Pac-man.git
cd Pac-man

# Build the project
mkdir build && cd build
cmake ..
make

# Run the game
./Pacman
```

---

##  Controls

| Action | Keys |
|--------|------|
| **Move** | `Arrow Keys` or `WASD` |
| **Start Game** | `Enter` |
| **Pause** | `P` |
| **Exit** | `Escape` |

---

<<<<<<< HEAD
##  Project Structure
=======
### Prerequisites
- **C++17** compatible compiler (MSVC, GCC, Clang)
- **CMake 3.10+**
- **OpenGL 3.3+**
- **GLFW3** and **GLAD** libraries

### Build & Run

```bash
# Clone the repository
git clone https://github.com/Dhuvie/Pac-man.git
cd Pac-man

# Build the project
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Run the game
./Release/Pacman.exe    # Windows
./Pacman               # Linux/Mac
```

### Windows Quick Build
```cmd
# Use the included batch file
build.bat
```

##  Architecture

```
 Pac-Man Game
├──  Game.cpp/h          # Main game loop & state management
├──  Player.cpp/h        # Pac-Man character & controls
├──  Ghost.cpp/h         # AI behaviors & pathfinding
├──  Maze.cpp/h          # Level layout & collision detection
├──  Renderer.cpp/h      # OpenGL rendering & effects
├──  ParticleSystem.cpp/h # Visual effects system
├──  ResourceManager.cpp/h # Asset management
├──  Shader.cpp/h        # OpenGL shader handling
└──  shaders/           # GLSL shader files
```



##  Technical Highlights

- **Modern C++17**: Clean, object-oriented design
- **High Performance**: Optimized rendering and game logic
- **Bulletproof Collision**: Perfect wall detection system
- **Advanced AI**: Multiple pathfinding algorithms
- **Visual Effects**: Particle systems and smooth animations
- **Resource Management**: Efficient asset handling
- **Cross-Platform**: CMake build system

---



##  Contributing

Contributions are **welcome**! Here's how you can help:

1.  **Fork** the repository
2.  **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3.  **Commit** your changes (`git commit -m 'Add amazing feature'`)
4.  **Push** to the branch (`git push origin feature/amazing-feature`)
5.  **Open** a Pull Request

###  Bug Reports
- Use the **Issues** tab to report bugs
- Include **steps to reproduce** and **system information**

###  Feature Requests
- Suggest new features via **Issues**
- Describe the **use case** and **expected behavior**

---

##  License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

##  Acknowledgments

-  **Namco** - Original Pac-Man creators (1980)
-  **OpenGL Community** - Graphics libraries and documentation
-  **C++ Community** - Modern C++ best practices
-  **Game Development Community** - Inspiration and techniques

---

##  Project Stats

- **Language**: C++17
- **Graphics**: OpenGL 3.3+
- **Build System**: CMake
- **Lines of Code**: ~3000+
- **Development Time**: Intensive optimization and polish

---

<div align="center">

**⭐ Star this repository if you enjoyed the game! ⭐**

[Report Bug](https://github.com/Dhuvie/Pac-man/issues) · [Request Feature](https://github.com/Dhuvie/Pac-man/issues)

</div>
