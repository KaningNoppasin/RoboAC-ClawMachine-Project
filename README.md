# RoboAC-ClawMachine-Project




## Project Structure
```
/RoboAC-Wheelchair-Project
├── include
├── lib
├── src
│   └── main.cpp
├── platformio.ini
└── README.md
```
- ### Main Machine
  - **main.cpp**
    ```cpp
    #include <Arduino.h>
    #include "./main/main.ino"
    ```

## Configuration
- ### Arduino Mega
  - **platformio.ini**
    ```ini
    [env:megaatmega2560]
    platform = atmelavr
    board = megaatmega2560
    framework = arduino
    monitor_speed = 115200
    monitor_filters = time, log2file
    lib_deps = 
    	waspinator/AccelStepper@^1.64
    	arduino-libraries/Servo@^1.2.1
    	bblanchon/ArduinoJson@^7.0.4
    	featherfly/SoftwareSerial@^1.0
    ```
## Workspace
  - ```X-Axis``` 690 mm
  - ```Y-Axis``` 690 mm


## Isometric
![image](https://github.com/KaningNoppasin/labubu_dollProject/assets/95127494/2b797583-906b-4e08-a9a6-3c365a557ab9)


## Top view
![image](https://github.com/KaningNoppasin/labubu_dollProject/assets/95127494/acd96ec4-96f8-4583-b83b-ae31e48f2a74)

