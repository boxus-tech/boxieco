# Boxieco [![Build Status](https://travis-ci.org/boxus-tech/boxieco.svg?branch=master)](https://travis-ci.org/boxus-tech/boxieco)

DIY indoor environment monitor with temperature, humidity, pressure, eCO2 and TVOC readings.

## Overview

<img src="https://github.com/boxus-tech/boxieco/blob/master/pictures/final-photo.jpeg" width="450">

## Hardware

### Components

* Arduino Nano (ATmega328P) board
* BME280 temperature, pressure and humidity sensor board
* CCS811 eCO2 and TVOC sensor board
* LCD5110 screen

### Wirings

CCS811:
```typescript
WAK -> GND
VCC -> 5V / 3V
SDA -> A4
SCL -> A5
GND -> GND
```

BME280:
```typescript
VCC -> 3V
SDA -> A4
SCL -> A5
GND -> GND
```

LCD5110:
```typescript
VCC -> 3V
CLK -> D13
DIN -> D11
DC  -> D10
CE  -> D9
RST -> D8
BL  -> 3V
GND -> GND
```

### Firmware

Use [Arduino IDE](https://www.arduino.cc/en/Main/Software) to flash [Boxieco sketch](https://github.com/boxus-tech/boxieco/blob/master/arduino/boxieco/boxieco.ino).

### 3D-printed housing

You can use a filament of your choice to print Boxieco housing using provided [STL 3D-models](https://github.com/boxus-tech/boxieco/tree/master/3d_parts) or easily customize with `*.scad` files.

<img src="https://github.com/boxus-tech/boxieco/blob/master/pictures/box-render.png" width="450">

## Contribution

### TODO


