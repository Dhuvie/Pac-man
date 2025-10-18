# 🟡 Pac-Man Game

A **modern, high-performance** implementation of the classic Pac-Man arcade game built with **C++** and **OpenGL**.

![Pac-Man Game](https://img.shields.io/badge/Game-Pac--Man-yellow?style=for-the-badge&logo=game)
![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=cplusplus)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-green?style=for-the-badge&logo=opengl)
![CMake](https://img.shields.io/badge/CMake-3.10+-red?style=for-the-badge&logo=cmake)

## 🎮 Features

### 🎯 **Core Gameplay**
- **Classic Pac-Man Experience**: Authentic maze navigation, dot collection, and ghost avoidance
- **Perfect Collision Detection**: Bulletproof wall collision system - no glitches or wall penetration
- **Smooth Movement**: Responsive controls with precise grid-based movement
- **Power Pellets**: Turn the tables on ghosts with temporary invincibility

### 👻 **Advanced Ghost AI**
- **4 Unique Ghost Personalities**:
  - 🔴 **Blinky (Red)**: Aggressive direct pursuit
  - 🩷 **Pinky (Pink)**: Strategic ambush tactics  
  - 🩵 **Inky (Cyan)**: Complex patrol patterns
  - 🧡 **Clyde (Orange)**: Unpredictable behavior switching
- **Multiple Pathfinding Algorithms**: Dijkstra, A*, and Backtracking
- **Dynamic Difficulty**: AI becomes more aggressive as levels progress

### 🎨 **Visual Excellence**
- **OpenGL-Powered Graphics**: Hardware-accelerated rendering
- **Particle Effects**: Visual flair for enhanced gameplay experience
- **Smooth Animations**: 60 FPS gameplay with fluid character movement
- **Classic Aesthetic**: Faithful to the original Pac-Man design

### 🏆 **Game Systems**
- **Progressive Difficulty**: Increasingly challenging levels
- **High Score Tracking**: Persistent leaderboard system
- **Multiple Game States**: Menu, gameplay, game over, and pause
- **Sound-Ready Architecture**: Prepared for audio integration

## 🎮 Controls

| Action | Keys |
|--------|------|
| **Move** | `Arrow Keys` or `WASD` |
| **Start Game** | `Enter` |
| **Pause** | `P` |
| **Exit** | `Escape` |

## 🚀 Quick Start

### Prerequisites
- **C++17** compatible compiler (MSVC, GCC, Clang)
- **CMake 3.10+**
- **OpenGL 3.3+**
- **GLFW3** and **GLAD** libraries

### Build & Run

```bash
# Clone the repository
git clone https://github.com/yourusername/Pac-man.git
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

## 🏗️ Architecture

```
📦 Pac-Man Game
├── 🎮 Game.cpp/h          # Main game loop & state management
├── 🟡 Player.cpp/h        # Pac-Man character & controls
├── 👻 Ghost.cpp/h         # AI behaviors & pathfinding
├── 🧱 Maze.cpp/h          # Level layout & collision detection
├── 🎨 Renderer.cpp/h      # OpenGL rendering & effects
├── ✨ ParticleSystem.cpp/h # Visual effects system
├── 📦 ResourceManager.cpp/h # Asset management
├── 🔧 Shader.cpp/h        # OpenGL shader handling
└── 📁 shaders/           # GLSL shader files
```

## 🎯 Game Mechanics

### 👻 Ghost AI Behaviors

| Ghost | Color | Behavior | Strategy |
|-------|-------|----------|----------|
| **Blinky** | 🔴 Red | Aggressive | Direct chase |
| **Pinky** | 🩷 Pink | Ambush | Targets 4 tiles ahead |
| **Inky** | 🩵 Cyan | Patrol | Complex positioning |
| **Clyde** | 🧡 Orange | Random | Distance-based switching |

### 🏆 Scoring System

| Item | Points |
|------|--------|
| **Dot** | 10 |
| **Power Pellet** | 50 |
| **Ghost** (1st) | 200 |
| **Ghost** (2nd) | 400 |
| **Ghost** (3rd) | 800 |
| **Ghost** (4th) | 1600 |

## 🛠️ Technical Highlights

- **🔧 Modern C++17**: Clean, object-oriented design
- **⚡ High Performance**: Optimized rendering and game logic
- **🎯 Bulletproof Collision**: Perfect wall detection system
- **🧠 Advanced AI**: Multiple pathfinding algorithms
- **🎨 Visual Effects**: Particle systems and smooth animations
- **📦 Resource Management**: Efficient asset handling
- **🔄 Cross-Platform**: CMake build system

## 📁 Project Structure

```
Pac-man/
├── 📄 Source Files (.cpp/.h)
├── 📁 shaders/          # OpenGL shaders
├── 📁 build/            # Build output
├── 🔧 CMakeLists.txt    # Build configuration
├── 🚀 build.bat         # Windows build script
└── 📖 README.md         # This file
```

## 🤝 Contributing

Contributions are **welcome**! Here's how you can help:

1. 🍴 **Fork** the repository
2. 🌿 **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. 💾 **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. 📤 **Push** to the branch (`git push origin feature/amazing-feature`)
5. 🔄 **Open** a Pull Request

### 🐛 Bug Reports
- Use the **Issues** tab to report bugs
- Include **steps to reproduce** and **system information**

### 💡 Feature Requests
- Suggest new features via **Issues**
- Describe the **use case** and **expected behavior**

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- 🎮 **Namco** - Original Pac-Man creators
- 🛠️ **OpenGL Community** - Graphics libraries and documentation
- 💻 **C++ Community** - Modern C++ best practices
- 🎨 **Game Development Community** - Inspiration and techniques

---

<div align="center">

**⭐ Star this repository if you enjoyed the game! ⭐**

Made with ❤️ and lots of ☕

</div>
