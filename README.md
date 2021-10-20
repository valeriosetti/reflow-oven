# Description
This project aims to build a DIY reflow oven based on:
	- STM32F4-discovery board
	- a Solid State Relay (SSR), for turning on/off the heating elements inside the oven
	- a Nokia 3310 LCD, for some basic output to the user regarding the current reflow status
	- a MAX31855 module for the temperature readings
	- a thermocoupe, of course :)

While the STM32 is in charge of controlling the reflow process the GUI is provided through a C++ project for desktop PC based on wxWidgets. 
The communication between the STM32 and the host PC is based on virtual-COM (serial over USB).

Being based on makefile it should be working out-of-the-box for everybody, as long as the user has the "arm-none-eabi-" toolchain installed.

# NOTE
This project was started from ST's CubeMX, which makes it depending on the ST's libraries. However due to their huge memory footprint they were not added to this repo. Therefore, before trying build anything, the user should:
	- go to STM32F4 website and download their STM32F4Cube libraries. 
		- At this moment they are at version "V1.11.0" and I suggest to use this one. Using other versions might give errors for both files' path namings and function implementations...
	- unzip the downloaded file into the project's folder in order to have something like this "[..]/reflow-oven/STM32Cube_FW_F4_V1.11.0"
