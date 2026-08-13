# Autonomous Object Sorting Robot

An autonomous VEX robot programmed in C++ that sorts objects into designated locations using multi-sensor input and closed-loop motor control. Touch, colour, distance, gyro, and motor encoder inputs drive navigation, with PID-tuned rotation correcting gyro drift for precise turning. A moving-average filter smooths distance sensor readings to reduce noise. A modular function-based architecture isolates mechanical control, navigation, and input handling for easier testing and debugging.

## How It Works
1. User places an object in the claw and selects a category via taps.
2. The robot drives forward, scanning for the matching coloured stripe.
3. Upon detection, it performs a 90° turn and approaches the box using the distance sensor.
4. The claw releases the object, and the robot returns to its starting position using encoder-based navigation.
5. If idle for 20 seconds, the robot safely shuts down.

## Demo & Report

[![Watch the demo](https://img.youtube.com/vi/CaOrTIv2SYU/hqdefault.jpg)](https://youtu.be/CaOrTIv2SYU)

**Full Project Report:** [View on Google Drive](https://drive.google.com/file/d/1MJbCLCNo2NveWUUdiiL4WoyymR6oygXf/view?usp=sharing)

## Authors
Anshia Yaqoob, Jaime McGale, Martin Montgomery, Ahmad Ashmawi

## Course
MTE 100 & MTE 121 — University of Waterloo, December 2025
