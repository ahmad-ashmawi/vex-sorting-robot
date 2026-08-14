# Autonomous Object Sorting Robot

An autonomous VEX robot programmed in C++ that sorts objects into designated locations using multi-sensor input and closed-loop motor control. Touch, colour, distance, gyro, and motor encoder inputs drive navigation and object handling.


## Technical Features

| Area | Description |
|---|---|
| **Control** | A **PID controller** drives 90° turns, correcting for gyro drift of 5–7° per rotation. A separate **proportional correction** keeps the robot driving straight by comparing live heading to the target heading. |
| **Navigation** | **Motor encoders** track forward displacement during each leg of the sorting cycle, letting the robot reverse that distance to return to its starting position without needing a mapped path. |
| **Architecture** | A **modular, function-based structure** isolates claw control, lift control, navigation, and input handling into separate functions, so each behaviour can be tested and debugged independently before integration. |
| **Safety** | The robot shuts down automatically after **20 seconds of inactivity**, and returns to its starting position if it fails to detect the target stripe within **15 seconds**, preventing unintended or runaway behaviour. |


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

## Courses
MTE 100 & MTE 121 — University of Waterloo, Fall 2025
