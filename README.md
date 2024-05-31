# Smart Fan Controller

## Motivation
This project has been made for the subject `Processadors Digitals` at the **Universitat Politecnica de Catalunya**. 

## Overview

The Smart Fan Controller is an advanced IoT project that integrates WiFi connectivity, a web server, sensor data collection, and multiple control modes to provide an intelligent and user-friendly fan control system. This project allows users to control the fan speed, operation modes, and other functionalities remotely via a web interface.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [System Architecture](#system-architecture)
4. [Installation](#installation)
5. [Usage](#usage)
    - [WiFi Connectivity](#wifi-connectivity)
    - [Web Server](#web-server)
    - [Control Modes](#control-modes)
6. [Control Modes](#control-modes)
    - [Manual Mode](#manual-mode)
    - [Auto Mode](#auto-mode)
    - [Night Mode](#night-mode)
    - [Timer Mode](#timer-mode)
7. [Hardware Setup](#hardware-setup)
8. [Software Details](#software-details)
    - [WiFi Connection](#wifi-connection)
    - [File Handling](#file-handling)
    - [Web Server Handlers](#web-server-handlers)
    - [Button Interrupts](#button-interrupts)
    - [Task Management](#task-management)
9. [Contributing](#contributing)


## Introduction

The Smart Fan Controller project aims to modernize the traditional fan by adding features like remote control, automated speed adjustments based on temperature, and various operational modes. It is designed to enhance user convenience and energy efficiency.

## Features

- **WiFi Connectivity**: Allows the fan to be controlled via a web interface.
- **Web Server**: Provides a user interface for remote control.
- **Multiple Control Modes**: Includes Manual, Auto, Night, and Timer modes.
- **Sensor Integration**: Monitors temperature and humidity to adjust fan speed.
- **OLED Display**: Shows current status and sensor readings.


## System Architecture

The system consists of an ESP32 microcontroller connected to a fan, an OLED display, and sensors. The ESP32 hosts a web server for remote control and handles the logic for various fan control modes.

![System Architecture](./images/system_architecture.png)

## Installation

To install and set up the Smart Fan Controller:

1. **Hardware Setup**: Connect the ESP32, fan, OLED display, and sensors according to the wiring diagram. Change the pins in the `pinout.h` file.

2. **Software Setup**: Upload the provided code to the ESP32 using the Arduino IDE or Platformio.

## Usage

### WiFi Connectivity

The system connects to a WiFi network with the following settings:
- **Static IP**: 192.168.1.45

### Web Server

Access the web interface via the IP address assigned to the ESP32. The web server allows users to:
- Control the fan (On/Off)
- Adjust fan speed
- Switch between control modes
- View current temperature and humidity

### Control Modes

#### Manual Mode

Allows the user to manually control the fan speed and turn the fan on or off.

- #### Auto Mode

Automatically adjusts the fan speed based on the current temperature.

- #### Night Mode

Reduces fan speed to a minimum level for quiet operation during the night.

- #### Timer Mode

Turns the fan off after a specified duration. Default to 30 minutes.

## Hardware Setup

Follow the wiring diagram to connect the ESP32 to the fan, OLED display, buttons, and sensors. Ensure proper power supply and grounding for all components.

## Software Details

### WiFi Connection

The `connectWifi` function establishes a connection to the specified WiFi network with a static IP configuration.

### File Handling

The `loadFile` function reads the content of a file from the ESP32's SPIFFS filesystem.

### Web Server Handlers

The web server has handlers for various endpoints to control the fan and modes:
- `handle_root`: Serves the main web page.
- `handle_SetUnlocked`: Unlocks the device.
- `handle_SetLocked`: Locks the device.
- `handle_ActivateFan`: Turns the fan on.
- `handle_DeactivateFan`: Turns the fan off.
- `handle_IncreaseSpeed`: Increases fan speed.
- `handle_DecreaseSpeed`: Decreases fan speed.
- `handle_ActivateNightMode`: Activates night mode.
- `handle_DeactivateNightMode`: Deactivates night mode.
- `handle_ActivateAutoMode`: Activates auto mode.
- `handle_DeactivateAutoMode`: Deactivates auto mode.
- `handle_ActivateTimer`: Activates the fan timer.

### Button Interrupts

The system uses interrupts to handle button presses for mode changes and fan control.

### Task Management

The system uses FreeRTOS tasks to manage different functionalities such as sensor data collection, web server operations, and fan control.

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests for any improvements or bug fixes.


