# triforce-control

[![Documentation](https://codedocs.xyz/TeamTriforceUK/triforce-control.svg)](https://codedocs.xyz/TeamTriforceUK/triforce-control/)

## Introduction
This software is used on the Triforce robot to translate trasnsmitter PPM signals to ESC PWM signals.
The translation involves determining the speeds of the three drive motors so that the robot moves in the direction of the transmitter joystick.

## Compile
```
git clone https://github.com/TeamTriforceUK/triforce-control.git
cd triforce-control
mbed deploy
mbed compile -t GCC_ARM -m lpc1768
```
The produced binary can then be copied to the mbed via USB.

## Hardware
The key piece of hardware is the LPC1768 (see below), pin assignment modifications
would be required to use another mbed compatible platform.
-  [LPC1768](https://developer.mbed.org/handbook/Order)
- [Turnigy 9X 9Ch Transmitter](https://hobbyking.com/en_us/turnigy-9x-9ch-transmitter-w-module-8ch-receiver-mode-2-v2-firmware.html)
- [Turnigy 9X 2.4GHz 8Ch Receiver (V2)](https://hobbyking.com/en_us/turnigy-9x-2-4ghz-8ch-receiver-v2.html)

## License
This software is licensed under the MIT license, unless stated otherwise.

## Contributing
All contributions are welcome.
