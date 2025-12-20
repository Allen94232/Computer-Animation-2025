# Computer Animation 2025

This repository contains homework assignments for Computer Animation course (2025 Spring Semester).

## Assignments Overview

### HW1: Soft Body Simulation (Jelly)
A particle-based soft body simulation using mass-spring systems to simulate jelly-like objects.

**Features:**
- Mass-spring system for deformable objects
- Multiple integration methods (Explicit Euler, Implicit Euler, Midpoint, Runge-Kutta 4)
- Collision detection with terrain
- Real-time rendering with shadows and skybox

**Key Technologies:**
- OpenGL for graphics rendering
- Eigen for linear algebra
- GLFW for window management
- ImGui for user interface

### HW2: Inverse Kinematics
Implementation of inverse kinematics for character animation using skeletal systems.

**Features:**
- Forward kinematics
- Inverse kinematics solver
- Acclaim motion file format support
- Real-time skeletal animation

**Key Technologies:**
- OpenGL + GLSL shaders
- Eigen for matrix operations
- Acclaim file format parsing

### HW3: 2D Fluid Simulation
A 2D fluid simulation using particle-based methods (PIC/FLIP).

**Features:**
- PIC/FLIP hybrid solver
- Particle advection
- Pressure projection
- Density correction
- Interactive obstacle placement

**Key Technologies:**
- OpenGL for rendering
- Eigen for numerical computations
- FLIP (Fluid Implicit Particle) method

## Build Instructions

### Windows with Visual Studio 2017/2019/2022

Each homework has its own Visual Studio solution file:
- HW1: Open `HW1/SoftSim.sln`
- HW2: Open `HW2/InverseKinematics.sln`
- HW3: Open `HW3/2dFluidSim.sln`

Build in Visual Studio (Release or Debug mode). Executables will be in the respective `bin` folders.

### Cross-Platform Build with CMake

**Note**: CMake build is not well-tested. Windows + Visual Studio is recommended.

#### Prerequisites
- CMake (version >= 3.14)
- C++ Compiler (GCC, Clang, or MSVC)

#### Linux Dependencies
Install one of the following:
- X11: `sudo apt install xorg-dev`
- Wayland: `sudo apt install libwayland-dev wayland-protocols extra-cmake-modules libxkbcommon-dev`
- OSMesa: `sudo apt install libosmesa6-dev`

#### Build Commands
```bash
# For each homework (HW1, HW2, HW3)
cd HW1  # or HW2, HW3
cmake -S . -B build
cmake --build build --config Release --parallel 8
```

Executables will be in the `bin` folder.

## 🎮 Usage

### HW1: Soft Body Simulation
1. Run the executable
2. Use the GUI to adjust simulation parameters:
   - Integration method (Explicit/Implicit Euler, Midpoint, RK4)
   - Spring stiffness and damping
   - Terrain type (Plane/Elevator)
3. Press Space to start/pause simulation
4. Use mouse to control camera

### HW2: Inverse Kinematics
1. Run the executable
2. Load Acclaim motion files (.asf/.amc) from `assets/Acclaim/`
3. Use IK controls to manipulate the skeleton
4. Adjust solver parameters in the GUI

### HW3: 2D Fluid Simulation
1. Run the executable
2. Click and drag to add obstacles
3. Adjust simulation parameters:
   - Particle count
   - FLIP ratio (0 = pure PIC, 1 = pure FLIP)
   - Stiffness coefficient
   - Time step
4. Toggle density correction
5. Switch between different render modes

## Project Structure

```
Computer Animation/
├── HW1/                    # Soft body simulation
│   ├── src/
│   │   ├── simulation/    # Physics simulation code
│   │   └── gfx/          # Graphics rendering code
│   ├── assets/           # Shaders and textures
│   └── main.cpp
├── HW2/                    # Inverse kinematics
│   ├── src/
│   │   ├── simulation/    # IK solver
│   │   ├── graphics/     # Rendering
│   │   └── acclaim/      # Motion file parsing
│   ├── assets/           # Motion files and shaders
│   └── InverseKinematics/
├── HW3/                    # 2D fluid simulation
│   ├── src/
│   │   └── fluid.cpp     # Fluid simulation implementation
│   ├── include/
│   └── 2dFluidSim/
└── README.md
```

## Dependencies

All dependencies are included in the `vendor/` or `extern/` folders of each homework:
- **GLFW**: Window and input handling
- **GLAD**: OpenGL loader
- **Eigen**: Linear algebra library
- **ImGui**: Immediate mode GUI
- **stb_image**: Image loading

## Implementation Notes

### HW1 - Key Implementation Points
- Mass-spring system with structural, shear, and bend springs
- Multiple integration schemes for different stability/performance tradeoffs
- Collision detection and response with terrain
- Shadow mapping for realistic rendering

### HW2 - Key Implementation Points
- Forward kinematics using transformation matrices
- Inverse kinematics using Jacobian-based methods
- Skeletal hierarchy traversal
- Acclaim file format parsing for motion data

### HW3 - Key Implementation Points
- MAC grid for velocity field
- Particle-grid transfer (P2G and G2P)
- Pressure projection using Gauss-Seidel solver
- FLIP/PIC hybrid for stability and detail
- Density correction for incompressibility