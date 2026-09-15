# Computer Animation — Simulation and Character Systems

A collection of computer-animation coursework implemented in C++/OpenGL and Unity. The projects cover physically based simulation, skeletal animation, numerical integration, and interactive character animation.

> This repository contains course starter code, third-party libraries, and project assets where applicable. The descriptions below focus on the systems represented by the source code in this repository.

## Projects

| Project | Focus | Stack |
| --- | --- | --- |
| [HW1 — Soft-Body Simulation](#hw1--soft-body-simulation) | 3D mass–spring simulation, numerical integration, terrain collision | C++, OpenGL, Eigen, ImGui |
| [HW2 — Inverse Kinematics](#hw2--inverse-kinematics) | Skeletal hierarchy, forward kinematics, Jacobian-based IK | C++, OpenGL, Eigen |
| [HW3 — 2D Fluid Simulation](#hw3--2d-fluid-simulation) | Particle/grid transfer, PIC/FLIP blending, incompressibility correction | C++, OpenGL, Eigen |
| [Final Project — Interactive Character Animation](#final-project--interactive-character-animation) | Timeline-driven character scenes and synchronized audiovisual effects | Unity, C#, Timeline, URP |

## HW1 — Soft-Body Simulation

An interactive deformable-body simulation using a particle-based mass–spring system.

Key systems in the source:

- Structural, shear, and bend springs
- Elastic and damping force accumulation
- Explicit Euler, Midpoint Euler, Runge–Kutta, and the project's `ImplicitEuler` update mode
- Terrain collision and response
- Runtime parameter controls and OpenGL visualization

**Source:** [`integrator.cpp`](HW1/src/simulation/integrator.cpp) · [`massSpringSystem.cpp`](HW1/src/simulation/massSpringSystem.cpp) · [`jelly.cpp`](HW1/src/simulation/jelly.cpp)

**Recorded results:** [`result1.mp4`](HW1/utility/result1.mp4) · [`result2.mp4`](HW1/utility/result2.mp4) · [`result3.mp4`](HW1/utility/result3.mp4)

## HW2 — Inverse Kinematics

A skeletal-animation project supporting Acclaim ASF/AMC motion data, hierarchical forward kinematics, and interactive inverse kinematics.

Key systems in the source:

- Hierarchical bone transforms
- Jacobian construction for an articulated chain
- Pseudoinverse-based IK updates
- Iterative end-effector target solving
- Animated posture and skeleton rendering

**Source:** [`kinematics.cpp`](HW2/src/simulation/kinematics.cpp) · [`motion.cpp`](HW2/src/acclaim/motion.cpp) · [`skeleton.cpp`](HW2/src/acclaim/skeleton.cpp)

## HW3 — 2D Fluid Simulation

A real-time particle fluid simulation using a staggered MAC grid and a hybrid PIC/FLIP update.

The simulation loop includes particle integration, collision handling, particle-to-grid transfer, density and divergence correction, and grid-to-particle velocity transfer.

**Source:** [`fluid.cpp`](HW3/src/fluid.cpp)

## Final Project — Interactive Character Animation

A Unity project built around authored character sequences. The repository includes scene logic for story progression, facial expressions, fog, white balance, walking audio, and runtime object parenting.

**Main scene:** [`Main Scene.unity`](Final%20Project/Assets/Scenes/Main%20Scene.unity)

**Project scripts:**

- [`StoryBoardManager.cs`](Final%20Project/Assets/Scripts/StoryBoardManager.cs)
- [`FacialExpressionController.cs`](Final%20Project/Assets/Scripts/FacialExpressionController.cs)
- [`Fog Controller.cs`](Final%20Project/Assets/Scripts/Fog%20Controller.cs)
- [`WhiteBalanceController.cs`](Final%20Project/Assets/Scripts/WhiteBalanceController.cs)
- [`WalkingSound.cs`](Final%20Project/Assets/Scripts/WalkingSound.cs)
- [`AddParent.cs`](Final%20Project/Assets/Scripts/AddParent.cs)

## Repository Structure

```text
.
├── HW1/                  # Soft-body simulation
├── HW2/                  # Forward and inverse kinematics
├── HW3/                  # 2D PIC/FLIP fluid simulation
└── Final Project/        # Unity character-animation project
```

## Build and Run

### C++ assignments

The repository includes Visual Studio solutions:

```text
HW1/SoftSim.sln
HW2/InverseKinematics.sln
HW3/2dFluidSim.sln
```

Open the required solution in Visual Studio, select a build configuration, and run the generated executable. Each project also contains its own CMake configuration.

### Unity final project

1. Install Unity `2022.3.11f1`.
2. Add the `Final Project` directory through Unity Hub.
3. Open `Assets/Scenes/Main Scene.unity`.
4. Enter Play Mode.

## Technologies

`C++` · `OpenGL` · `GLSL` · `Eigen` · `GLFW` · `ImGui` · `CMake` · `Unity 2022.3.11f1` · `C#` · `URP` · `Timeline`
