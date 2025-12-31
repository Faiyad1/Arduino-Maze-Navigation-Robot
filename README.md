# Arduino Maze Navigation Robot

**Language:** Arduino (C++)

IR sensor-based autonomous robot that navigates and solves mazes.

## Overview

An autonomous maze-solving robot built on the Arduino BOE Shield-Bot platform. The robot uses three IR sensors to navigate corridors, detect walls, and find the exit of complex mazes.

## Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [How It Works](#how-it-works)
- [Maze Challenges](#maze-challenges)
- [Usage](#usage)
- [Key Parameters](#key-parameters)
- [Tuning Notes](#tuning-notes)
- [Files](#files)
- [References](#references)

## Features

- **Autonomous Navigation**: Follows corridors and makes turn decisions based on sensor input
- **Wall Detection**: Uses IR sensors to detect walls on left, right, and front
- **Dynamic Alignment**: Multi-level alignment system keeps the robot centered in corridors
- **Dead-End Detection**: Distinguishes between short and long dead-ends using timing
- **Turn Handling**: Supports left turns, right turns, and 180-degree flips

## Hardware Requirements

- Arduino BOE Shield-Bot
- 3x IR LED/Sensor pairs (left, right, front)
- 2x Continuous rotation servo motors
- IR Sensor Shield (custom)

### Pin Configuration

| Component   | Output Pin | Input Pin | LED Pin |
|-------------|------------|-----------|---------|
| Left IR     | 10         | 11        | A2      |
| Middle IR   | 6          | 7         | A1      |
| Right IR    | 2          | 3         | A0      |
| Left Servo  | 13         | -         | -       |
| Right Servo | 12         | -         | -       |

## How It Works

### Sensor System
The robot uses IR sensors operating at specific frequencies (~38-44 kHz) to detect walls at various distances. Different frequencies allow for distance-based detection thresholds.

### Navigation Logic
1. **No front wall detected**: Move forward while aligning to stay centered
2. **Front wall + right wall**: Turn right at T-junction
3. **Front wall only**: Continue forward with alignment
4. **No right wall**: Turn right into opening
5. **No left wall**: Turn left into opening
6. **No walls detected**: Check if short dead-end (flip) or long dead-end (stop)

### Alignment Algorithm
A 7-level alignment system adjusts the robot's trajectory based on proximity to walls:
- Lower levels: Minor corrections for slight drifts
- Higher levels: Stronger corrections when close to walls
- Barrier delays prevent over-correction oscillation

## Maze Challenges

The code is designed to handle:

- **Essential**: Basic corridor following with turns
- **Extension**: S-turns, U-turns, T-junctions, and dead-end detection
- **Advanced**: Complex mazes with islands, multiple paths, and open spaces

## Usage

1. Upload `FinalAdvancedMaze.ino` to your Arduino
2. Place the robot at the maze entrance
3. Power on - the robot will automatically begin navigation
4. Robot stops when it detects a long dead-end (finish area)

## Key Parameters

| Parameter            | Value    | Description                              |
|----------------------|----------|------------------------------------------|
| Forward IR frequency | 39750 Hz | Front wall detection                     |
| Side IR frequency    | 38000 Hz | Side wall detection                      |
| Turn delay           | 720 ms   | Time for 90-degree turn                  |
| Dead-end threshold   | 1590 ms  | Time to distinguish short/long dead-ends |

## Tuning Notes

- Servo microsecond values in `forward()`, `turningLeft()`, `turningRight()`, and alignment helpers are calibration points for speed and straightness
- IR modulation frequencies are tuned in `irLeftSensor(...)`, `irMiddleSensor(...)`, `irRightSensor(...)`, and the alignment arrays

## Files

- `FinalAdvancedMaze.ino` - Main robot control code
- `AssignmentSpecification.txt` - Project requirements and maze specifications
- `122-32335-*.pdf` - BOE Shield-Bot documentation
- `TSOP-4038 datasheet.pdf` - IR sensor receiver datasheet

## References

- https://learn.parallax.com/?s=chapter+7+navigating+infrared+10
- https://canvas.sydney.edu.au/courses/64732/pages/exercise-3-using-physical-equipment?module_item_id=2586788

---

*Project completed in May 27, 2025.*