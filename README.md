# ieee754-practice
A tool designed for people who wish to practice manual conversion of IEEE-754 floating point numbers and bit band aliasing.

## Integer Conversions
Shows integer numbers converted from and to any of 
- base 2
- base 10
- base 16.


![int-screenshot](img/int.png)

## Float Conversions
file '29-1.m4a'
Can be used to practice manual conversion to and from [IEEE-754 single precision floats](https://en.wikipedia.org/wiki/IEEE_754)

Offers step-by-step conversions of Decimal Numbers to binary and hexadecimal formats according to the IEEE-754 standard.
Also enables conversions of Hexadecimal Coded floats to decimal numbers.

![float-screenshot](img/float.png)

## Bit Banding
Can be used to practice finding byte addresses from bit-band alias addresses.

The Cortex-M4 is taken as an example, i.e. there are two bit banding areas
- `0x2000 0000` - `0x200F FFFF` -> `0x2200 0000` - `0x23FF FFFF` for data
- `0x4000 0000` - `0x400F FFFF` -> `0x4200 0000` - `0x42FF FFFF` for peripherals

![bb-screenshot](img/bb.png)
