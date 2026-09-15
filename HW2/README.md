# HW2 — Inverse Kinematics

This assignment implements skeletal forward and inverse kinematics with Acclaim ASF/AMC motion data. The application loads an articulated skeleton, evaluates hierarchical transforms, and iteratively moves an end effector toward a target using a Jacobian-based solver.

## Key Source Files

- [`src/simulation/kinematics.cpp`](src/simulation/kinematics.cpp): forward- and inverse-kinematics calculations
- [`src/acclaim/skeleton.cpp`](src/acclaim/skeleton.cpp): skeleton loading and hierarchy handling
- [`src/acclaim/motion.cpp`](src/acclaim/motion.cpp): motion data handling
- [`InverseKinematics/main.cpp`](InverseKinematics/main.cpp): application entry point

## Build on Windows

The tested project configuration uses Visual Studio.

1. Open `InverseKinematics.sln`.
2. Select a Debug or Release configuration.
3. Build the solution.
4. Run the executable generated under `bin/`.

## Build with CMake

The CMake path is provided for other toolchains but may require platform-specific dependency setup.

Requirements:

- CMake 3.14 or later
- A C++ compiler
- A supported OpenGL windowing environment

```bash
cmake -S . -B build
cmake --build build --config Release --parallel
```

On Linux, GLFW may require X11, Wayland, or OSMesa development packages. For example, an X11 setup commonly uses `xorg-dev`.

## Repository Context

See the [repository README](../README.md#hw2--inverse-kinematics) for the project overview and links to the other assignments.
