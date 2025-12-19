# AdvMovSys

A modular advanced movement system for Unreal Engine featuring comprehensive character locomotion mechanics.

## Overview

AdvMovSys is an Unreal Engine game project that implements a robust advanced movement system with multiple character states and animations for enhanced gameplay mobility.

## Features

- **Prone Movement**
  - Prone idle animations
  - Forward movement locomotion
  - Ground-level tactical positioning

- **Crouching Mechanics**
  - Crouch idle animations
  - Crouch forward walk movement
  - Reduced collision profile

- **Sliding System**
  - Left-side sliding mechanics
  - Dynamic traversal ability
  - Quick evasion movements

- **Animation Blending**
  - Smooth blend spaces for edge movements
  - Seamless state transitions
 
## Work In Progress

- **Edge Grab System** (WIP)
  - Currently under development
  - Planned for future release

## Project Structure

```
AdvMovSys/
├── Source/              # C++ source code
├── Content/             # Assets (animations, blueprints, materials)
├── Config/              # Configuration files
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   ├── DefaultInput.ini
│   └── DefaultEditor.ini
└── AdvMovSys.uproject   # Project file
```

## Getting Started

### Prerequisites
- Unreal Engine 5.x (or compatible version)
- Visual Studio 2022 or later (for C++ development)

### Building

1. Open `AdvMovSys.uproject` with Unreal Engine
2. Build the project through the Unreal Editor
3. Launch the game or editor

### Development

To modify the movement system:
- C++ code is located in `Source/AdvMovSys/`
- Animations and blueprints are in `Content/`
- Configure input bindings in `Config/DefaultInput.ini`

## Configuration

Key configuration files:
- **DefaultEngine.ini** - Engine settings and performance
- **DefaultGame.ini** - Game-specific configuration
- **DefaultInput.ini** - Input key bindings and axes
- **DefaultEditor.ini** - Editor preferences

## Author

Pierluigi Pedicone
