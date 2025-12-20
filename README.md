# Computer Animation 2025

This repository contains homework assignments and final project for Computer Animation course (2025 Spring Semester).

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
### Final Project: Interactive Character Animation with Timeline Control
A Unity-based interactive storytelling project featuring character animation with Ready Player Me avatars, cinematic timeline sequences, and dynamic environmental effects.

**Features:**
- Ready Player Me avatar integration
- Multiple cinematic timeline sequences (controlled by number keys 1-8)
- Facial expression system (Happy, Anxious, Confused, Cold, etc.)
- Dynamic fog and lighting control
- Character IK (Inverse Kinematics) for hands
- Environmental sound effects
- Parent-child hierarchy management for character interactions

**Key Technologies:**
- Unity 2022 LTS with Universal Render Pipeline (URP)
- Ready Player Me SDK for avatar creation and loading
- Unity Timeline for cinematic sequences
- Unity's Signal system for event-driven animation
- C# scripting for gameplay logic and animation control

**Scripts:**
- `StoryBoardManager.cs`: Controls timeline sequence playback
- `FacialExpressionController.cs`: Manages character facial expressions
- `Fog Controller.cs`: Dynamic fog effect control
- `WhiteBalanceController.cs`: Camera color temperature adjustment
- `WalkingSound.cs`: Footstep audio system
- `AddParent.cs`: Parent-child relationship management
## Build Instructions

### Windows with Visual Studio 2017/2019/2022

**For HW1, HW2, HW3:**
Each homework has its own Visual Studio solution file:
- HW1: Open `HW1/SoftSim.sln`
- HW2: Open `HW2/InverseKinematics.sln`
- HW3: Open `HW3/2dFluidSim.sln`

Build in Visual Studio (Release or Debug mode). Executables will be in the respective `bin` folders.

**For Final Project:**
1. Install Unity Hub and Unity 2022 LTS
2. Open the `Final Project` folder in Unity Hub
3. Wait for Unity to import all assets and packages
4. Open the main scene: `Assets/Scenes/Main Scene.unity`
5. Press Play in Unity Editor to test
6. Build for your target platform via File → Build Settings

### Cross-Platform Build with CMake

**Note**: CMake build is not well-tested. Windows + Visual Studio is recommended.
**Note**: This applies to HW1, HW2, HW3 only. Final Project requires Unity.
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

## Usage

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
### Final Project: Interactive Character Animation
1. Open the project in Unity 2022 LTS
2. Open the main scene: `Assets/Scenes/Main Scene.unity`
3. Press Play to start
4. Use number keys (1-8) to play different cinematic timeline sequences:
   - Key 1-8: Play corresponding storyboard timeline
5. Observe character animations, facial expressions, and environmental effects
6. Check the Timeline window to see the cinematic sequence structure

**Features to Explore:**
- Automatic facial expression changes during timelines
- Dynamic fog and lighting transitions
- Character parent-child relationships
- Hand IK interactions
- Environmental sounds synchronized with animations

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
├── Final Project/          # Unity character animation project
│   ├── Assets/
│   │   ├── Scenes/       # Unity scenes (Main Scene, Map, etc.)
│   │   ├── Scripts/      # C# gameplay scripts
│   │   ├── Animations/   # Animation clips
│   │   ├── Animators/    # Animator controllers
│   │   ├── Timelines/    # Cinematic timeline assets
│   │   ├── Prefabs/      # Reusable game objects
│   │   ├── Ready Player Me/ # Avatar SDK
│   │   └── ...
│   ├── ProjectSettings/  # Unity project configuration
│   ├── Packages/         # Unity package dependencies
│   └── UserSettings/     # User-specific settings
└── README.md
```

## Dependencies

**HW1, HW2, HW3:**
All dependencies are included in the `vendor/` or `extern/` folders:
- **GLFW**: Window and input handling
- **GLAD**: OpenGL loader
- **Eigen**: Linear algebra library
- **ImGui**: Immediate mode GUI
- **stb_image**: Image loading

**Final Project:**
Unity packages (managed via Package Manager):
- **Universal Render Pipeline (URP)**: Modern rendering pipeline
- **Ready Player Me SDK**: Avatar creation and loading
- **Timeline**: Cinematic sequence creation
- **Cinemachine**: Camera control system
- **Input System**: New Unity input handling

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

### Final Project - Key Implementation Points
- Ready Player Me avatar integration for customizable characters
- Unity Timeline system for orchestrating complex animation sequences
- Signal-based event system for triggering animations and effects
- Facial expression blending using blend shapes
- Dynamic environment control (fog, lighting, color grading)
- IK setup for realistic hand interactions with objects
- Audio synchronization with animation events
- Parent-child hierarchy management for character relationships