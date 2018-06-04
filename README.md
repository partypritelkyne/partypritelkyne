# Párty přítelkyně*
An official repository for [www.party-pritelkyne.cz](http://www.party-pritelkyne.cz)

## Code
There are currently two variants of PP01 Arduino projects:
- PP01_RELEASE – main version of the code
- PP01_BASIC – a template for creating your own projects

## How to flash an ATtiny85?
You can follow [these instructions](https://create.arduino.cc/projecthub/arjun/programming-attiny85-with-arduino-uno-afb829) on Arduino Project Hub. You will need an Arduino programmed as an ISP, a breadboad, a capacitor and some wires. Also mind that the production code is optimized for running at 8 MHz and the 8 MHz bootloader is preinstalled on all *Párty přítelkyně*.

Currently, the "Digispark" or equivalent development boards aren't officially supported, since they use a different bootloader. They might work, but it's not guaranteed.

## Schematics and PCB files
[The default schematic](schematics/pp01-default-schematic.pdf) on which are all *Párty přítelkyně* based is available as a PDF.

PCB source files and gerber files for manufacturing aren't publicly available, yet – they might be released in the future, until then you must figure it out by yourself :-)

## License
All provided source codes and schematics are distributed under MIT license unless specified otherwise.

---

***Párty přítelkyně = Party Friend**
