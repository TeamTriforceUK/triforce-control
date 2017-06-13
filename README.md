# triforce-control

[![Documentation](https://codedocs.xyz/TeamTriforceUK/triforce-control.svg)](https://codedocs.xyz/TeamTriforceUK/triforce-control/)

## Introduction
This software is used on the Triforce robot to translate trasnsmitter PPM signals to ESC PWM signals.
The translation involves determining the speeds of the three drive motors so that the robot moves in the direction of the transmitter joystick.


# triforce-control

## Introduction


### Repository Navigation
This repository is one of two repositories hosting the codebase for the Triforce robot. The repositories are versioned and released seperately. Links to the repositories are below.

#### [Triforce Control](https://github.com/TeamTriforceUK/triforce-control)

The Trforce Control firmware (mbed OS) runs on an LPC1768 (ARM Cortex M3). The firmware is reposnsible for controlling the robot.

#### [Triforce Telemetry](https://github.com/TeamTriforceUK/triforce-telemetry)

The Triforce Telemetry repositiry hosts firmware (FreeRTOS) for the ESP8266 WiFi chip. This microcontroller works alongside the mbed to provide telemetry functionality via an HTTP server.

The ESP8266 allows us to communicate telemetry from Triforce to connected mobile devices (tablets, laptops, mobile phones). We also control non-safety critical settings such as LED lights from the mobile device.

## System Overview

### Microprocessors

| Device  | Firmware  | Language |
|---------|-----------|----------|
| [LPC1768](https://developer.mbed.org/platforms/mbed-LPC1768/) | [mbed OS](https://www.mbed.com/en/development/mbed-os/)   | C++      |
| [ESP8266](https://www.adafruit.com/product/2471) | [FreeRTOS](http://www.freertos.org/)  | C        |

### Sensors

| Sensor | Purpose                              | Communication |
|--------|--------------------------------------|---------------|
| [BNO055](https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview) | Orientation, direction, acceleration |  I2C          |

## Development Environment

A development environment can be configured on Windows or Linux. Instructions can be found [here](https://github.com/ARMmbed/mbed-cli).

## Compilation
```
git clone https://github.com/TeamTriforceUK/triforce-control.git
cd triforce-control
mbed deploy
mbed compile -t GCC_ARM -m lpc1768
```

## Contributing

We are open to any contributions in terms of ideas, suggestions, bug reports, development. Feel free to open GitHub issues regarding any contributions.

## Licensing

This software is licensed under the [MIT License](https://tldrlegal.com/license/mit-license), unless stated otherwise.
