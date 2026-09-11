\# PIC32 Temperature \& Humidity Monitor (Sensirion SHT4x)



A bare-metal firmware implementation for reading environmental data from a Sensirion SHT4x digital sensor using a PIC32 microcontroller over the I2C bus.



\## Project Overview



* This project implements reliable I2C communication between a PIC32 microcontroller and a high-precision Sensirion SHT4x relative humidity and temperature sensor.



\## Component Details

* Microcontroller: PIC32MX675F512L
* Sensor: Sensirion SHT45-AD1B



\## Reference Documentation \& Datasheets

* PIC32 PIC32MX675F512L documents: [PIC32MX doc](https://www.microchip.com/en-us/product/pic32mx675f512l)
* Sensirion SHT4x Datasheet: [SHT4x doc](https://sensirion.com/media/documents/33FD6951/6A7C10A0/HT_DS_Datasheet_SHT4x_V7.3.pdf)



\## Software Tools

* IDE: Microchip MPLAB X IDE
* Compiler: XC32 Compiler (xc32-gcc)



\## Hardware Tool

* PICkit 3 (programmer and debugger)



\## How to Build \& Flash



1. Clone this repository: git clone \[https://github.com/MuhammadHarisMasoom/pic32-temp-humidity-i2c.git (https://github.com/MuhammadHarisMasoom/pic32-temp-humidity-i2c.git)
2. Open MPLAB X IDE.
3. Go to File > Open Project and navigate to the firmware/ folder.
4. Select your target PIC32 device variant in the project properties.
5. Click Clean and Build Project (Shift + F11).
6. Connect your programmer/debugger and click Make and Program Device.



👨‍💻 Author

* Muhammad Haris Masoom
* Embedded Systems Engineer
* [LinkedIn](www.linkedin.com/in/muhammadharismasoom)
* [GitHub](https://github.com/MuhammadHarisMasoom)

