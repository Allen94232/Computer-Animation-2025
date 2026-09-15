# Computer Animation — Physics, Simulation, and Character Systems

A collection of computer-animation projects implemented in C++/OpenGL and Unity. The repository focuses on physically based simulation, skeletal animation, numerical integration, and interactive character storytelling.

The three C++ assignments were completed individually. The Unity final project was developed as a team project; my specific contributions are listed separately below.

> Portfolio note: This repository originated from the Computer Animation course in Spring 2025. Starter code and course-provided assets remain where applicable; the implementation summaries below identify the systems I completed.

## Highlights

| Project | Core implementation | Stack |
| --- | --- | --- |
| [Soft-Body Simulation](#1-soft-body-simulation) | 3D mass–spring model, four time-integration modes, terrain collision | C++, OpenGL, Eigen, ImGui |
| [Inverse Kinematics](#2-inverse-kinematics) | Forward kinematics, Jacobian-based IK, pseudoinverse solve, skeletal hierarchy | C++, OpenGL, Eigen |
| [2D Fluid Simulation](#3-2d-fluid-simulation) | PIC/FLIP transfer, MAC grid, incompressibility solve, density correction | C++, OpenGL, Eigen |
| [Interactive Character Animation](#4-interactive-character-animation-team-project) | Timeline-driven character storytelling and synchronized audiovisual effects | Unity, C#, Timeline, URP |

<!-- Replace the path below after adding a 20–30 second overview GIF. -->
![Project overview](docs/media/overview.gif)

## 1. Soft-Body Simulation

An interactive 3D jelly simulation built with a particle-based mass–spring system. Particles are connected with structural, shear, and bend springs, allowing the model to deform while retaining its overall volume and shape.

### What I implemented

- Constructed and updated the particle–spring system, including per-particle force accumulation.
- Modeled elastic and damping forces for structural, shear, and bend springs.
- Implemented selectable time-stepping modes: Explicit Euler, a predictor/corrector-style update labeled `ImplicitEuler` in the project, Midpoint Euler, and fourth-order Runge–Kutta.
- Implemented collision detection and response between the deformable body and the terrain/elevator scene.
- Connected simulation parameters to an ImGui interface for real-time inspection and tuning.
- Integrated OpenGL visualization for particles, spring types, meshes, lighting, and shadows.

### Engineering focus

This project exposed the stability trade-offs between integration schemes. The interface allows spring stiffness, damping, time step, and integration mode to be changed while the simulation is running, making unstable oscillation and numerical damping directly observable.

<!-- Add a side-by-side GIF or image comparing two integrators under identical parameters. -->
![Soft-body simulation](docs/media/hw1-soft-body.gif)

**Relevant code:** [`integrator.cpp`](HW1/src/simulation/integrator.cpp) · [`massSpringSystem.cpp`](HW1/src/simulation/massSpringSystem.cpp)

## 2. Inverse Kinematics

A skeletal-animation system supporting Acclaim ASF/AMC motion data, hierarchical forward kinematics, and interactive inverse kinematics.

### What I implemented

- Traversed the skeleton hierarchy to compute global bone transforms with forward kinematics.
- Built the Jacobian for an articulated bone chain.
- Solved the IK update with a pseudoinverse-based linear solver.
- Iteratively updated joint rotations to move the end effector toward a user-defined target.
- Applied joint updates back to the animated posture and refreshed the rendered skeleton.

### Engineering focus

The main challenge was connecting local joint rotations, hierarchical transforms, and the end-effector error in a consistent coordinate system. The solver separates Jacobian construction from the linear solve, which makes the numerical method easier to inspect and modify.

<!-- Add a GIF showing the target moving while the chain follows it. -->
![Inverse kinematics](docs/media/hw2-inverse-kinematics.gif)

**Relevant code:** [`kinematics.cpp`](HW2/src/simulation/kinematics.cpp) · [`motion.cpp`](HW2/src/acclaim/motion.cpp)

## 3. 2D Fluid Simulation

A real-time particle fluid simulation using a hybrid Particle-in-Cell / Fluid-Implicit-Particle pipeline on a staggered MAC grid.

### What I implemented

- Advanced particles under gravity and resolved particle–boundary and particle–obstacle collisions.
- Transferred velocity between particles and the MAC grid with bilinear weights.
- Blended PIC and FLIP velocity updates through an adjustable FLIP ratio.
- Classified fluid, air, and solid cells from the particle distribution.
- Enforced approximate incompressibility by iteratively correcting grid divergence.
- Estimated cell density and added density correction to reduce particle compression.
- Added an interactive obstacle and particle/cell visualization modes.

### Simulation pipeline

1. Integrate particle motion.
2. Relax overlapping particles.
3. Handle boundary and interactive-obstacle collisions.
4. Transfer particle velocities to the MAC grid.
5. Estimate density and correct velocity divergence.
6. Transfer the corrected grid velocities back to particles using the PIC/FLIP blend.

<!-- Add one clean GIF with the obstacle interacting with the fluid. -->
![PIC/FLIP fluid simulation](docs/media/hw3-fluid.gif)

**Relevant code:** [`fluid.cpp`](HW3/src/fluid.cpp)

## 4. Interactive Character Animation — Team Project

A Unity-based interactive story composed of eight cinematic sequences. The project combines customizable characters, authored Timeline sequences, facial expressions, inverse-kinematics-assisted interactions, and synchronized environmental effects.

### Team result

- Integrated Ready Player Me characters into a Unity URP project.
- Authored and triggered eight Timeline-based story sequences.
- Coordinated facial expressions, lighting, fog, color temperature, sound effects, and character interactions through Timeline signals and C# components.
- Used hand IK and runtime parent–child changes for character–object interaction.

### My contributions

- Created the character-animation sequences in Unity Timeline.
- Arranged animation clips and Timeline tracks to control the pacing and progression of the story scenes.
- Coordinated character actions with scene timing to produce the final cinematic presentation.

My teammate was responsible for sourcing the environmental and scene assets used in the project.

<!-- Add a short final-project trailer or representative sequence. -->
![Interactive character animation](docs/media/final-project.gif)

## Repository Structure

```text
.
├── HW1/                  # 3D mass–spring soft-body simulation
├── HW2/                  # Forward and inverse skeletal kinematics
├── HW3/                  # 2D PIC/FLIP fluid simulation
├── Final Project/        # Unity team project
└── docs/media/           # Portfolio images and GIFs to add
```

## Build and Run

### C++ assignments

The recommended environment is Windows with Visual Studio 2019 or 2022. Each assignment includes its own solution file.

```text
HW1/SoftSim.sln
HW2/InverseKinematics.sln
HW3/2dFluidSim.sln
```

Open the required solution, select the Release or Debug configuration, build, and run the generated executable. The projects use OpenGL, GLFW, Eigen, ImGui, GLAD, and stb_image; bundled third-party dependencies are located in the project vendor/extern directories.

A CMake configuration is also included for the C++ assignments, but Windows/Visual Studio is the tested setup.

```bash
cmake -S HW1 -B build/HW1
cmake --build build/HW1 --config Release --parallel
```

Repeat with `HW2` or `HW3` as needed.

### Unity final project

1. Install Unity 2022 LTS.
2. Add `Final Project` through Unity Hub.
3. Open `Assets/Scenes/Main Scene.unity`.
4. Enter Play Mode.
5. Press number keys `1`–`8` to trigger the corresponding story sequences.

## Controls

| Project | Controls |
| --- | --- |
| HW1 | Use ImGui to select the integrator and tune simulation parameters; press `Space` to pause/resume. |
| HW2 | Load an ASF/AMC motion and manipulate the IK target through the application controls. |
| HW3 | Drag with the mouse to move the obstacle; use the interface to change the PIC/FLIP ratio and rendering mode. |
| Final Project | Press `1`–`8` to play Timeline sequences. |

## Technologies

`C++` · `OpenGL` · `GLSL` · `Eigen` · `GLFW` · `ImGui` · `CMake` · `Unity 2022 LTS` · `C#` · `URP` · `Timeline`

## Acknowledgments

These projects were developed for a Computer Animation course. The repository includes course-provided starter code, frameworks, and third-party dependencies where applicable. Unless otherwise noted, HW1–HW3 were completed individually; the Unity final project was collaborative.

## Author

**Po-Jen Cheng**  
M.S. student working on computer graphics, mixed reality, and interactive systems.
