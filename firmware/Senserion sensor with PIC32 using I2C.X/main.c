// FDEVOPT
#pragma config SOSCHP = OFF             // Secondary Oscillator High Power Enable bit (SOSC oprerates in normal power mode.)
#pragma config ALTI2C = OFF             // Alternate I2C1 Pins Location Enable bit (Primary I2C1 pins are used)
#pragma config FUSBIDIO = OFF           // USBID pin control (USBID pin is controlled by the USB module)
#pragma config FVBUSIO = OFF            // VBUS Pin Control (VBUS pin is controlled by the USB module)
#pragma config USERID = 0xFFFF          // User ID bits (Enter Hexadecimal value)

// FICD
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config ICS = PGx3               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC3/PGED3)

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = ON             // Downside Voltage Protection Enable bit (Low power BOR is enabled, when main BOR is disabled)

// FWDT
#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (WDT is enabled)

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Selection bits (Fast RC oscillator (FRC) with divide-by-N)
#pragma config PLLSRC = FRC             // System PLL Input Clock Selection bit (FRC oscillator is selected as PLL reference input on device reset)
#pragma config SOSCEN = ON              // Secondary Oscillator Enable bit (Secondary oscillator is enabled)
#pragma config IESO = ON                // Two Speed Startup Enable bit (Two speed startup is enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Selection bit (Primary oscillator is disabled)
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OSCO pin operates as a normal I/O)
#pragma config SOSCSEL = OFF            // Secondary Oscillator External Clock Enable bit (SOSC pins configured for Crystal mode)
#pragma config FCKSM = CSECME           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is enabled)

// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>

void delay_us(unsigned int);
void I2C_init(double);
void I2C_wait_for_idle(void);
void I2C_start();
void I2C_restart();
void I2C_nack(void);
void I2C_stop();
void I2C_write(unsigned char, char, char);
void I2C_read(unsigned char*, char);

void ledBlink1(void);
void ledBlink2(void);


#define SYS_FREQ 8000000UL

void main(void) {

//    int received_bytes = 6;
//    unsigned char data[received_bytes]; // data array contains 4 bytes coming from sensor for temperature conversion
    
    unsigned char data_Global[5];
    float adc_val_temp;
    float adc_val_hum;
    float t_degC;
    float hum;

    I2C_init(400000); // setting frequency of I2C as 100kHz

    while (1) {

        I2C_start(); // sending start bit

        I2C_write(0x44<<1, 1, 0); // Sending address along with wait for ack as 1, and 0 to write to slave

        I2C_write(0xFD, 1, 0); // 0xFD register of sensor gives very precise data of temperature and humidity
        
        I2C_stop();

        delay_us(7000); // delay of 0.01 seconds as suggested in data sheet

        I2C_start();
        
//        I2C_restart(); // Sending repeated Start
        
        I2C_write(0x44 << 1, 1, 1); // setting R/W bit as 1 to read from slave

      
        I2C_read(&data_Global[1], 0); /* Read the Conversion MSB byte and ack it*/
        I2C_read(&data_Global[0], 0); /* Read the Conversion LSB byte and ack it*/
        I2C_read(&data_Global[2], 0); /* Read the Conversion LSB byte and  ack it (Temp. data checksum) */

        I2C_read(&data_Global[4], 0); /* Read the Conversion MSB byte and ack it*/
        I2C_read(&data_Global[3], 0); /* Read the Conversion LSB byte and ack it*/
        I2C_read(&data_Global[5], 1); /* Read the Conversion LSB byte and not ack it (humidity data checksum) */

        I2C_stop();

//        ledBlink1();

        adc_val_temp = data_Global[1] * 256 + data_Global[0]; // conversion function from data sheet
        adc_val_hum = data_Global[4] * 256 + data_Global[3];

        t_degC = -45 + (175 * adc_val_temp) / 65535; // converting into degree Centigrade
        hum = -6 + (125 * adc_val_hum) / 65535;

        if (t_degC >= 25.0 && t_degC <= 40.0) {
            ledBlink1(); // blinks LED for 1 second
        } 
        else {
            ledBlink2(); // blinks LED for 5 seconds
        }
        delay_us(10000);
    }

}

void delay_us(unsigned int us) {
    // Convert microseconds us into how many clock ticks it will take 
    us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks (In case of 8MHz frequency result is 0.4 clock ticks)
    // This line is not fully clear
    _CP0_SET_COUNT(0); // Set Core Timer count to 0

    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

// I2C_init() initialises I2C2 at at frequency of [frequency]Hz  

void I2C_init(double frequency) {
    double BRG;

    I2C2CON = 0; // Turn off I2C2 module
    I2C2CONbits.DISSLW = 0; // Disable slew rate for 100kHz

    BRG = (1 / (2 * frequency)) - 0.000000139;
    BRG *= (SYS_FREQ) - 2;

    I2C2BRG = (int) BRG; // Set baud rate
    I2C2CONbits.A10M = 0; // 7 bit slave address
    I2C2CONbits.ON = 1; // Turn on I2C2 module
}

// I2C_wait_for_idle() waits until the I2C peripheral is no longer doing anything  

void I2C_wait_for_idle(void) {
    uint16_t local_count = 0;
    do {
        local_count++;
        if (local_count > 65530) return;
    } while (I2C2CON & 0x1F);
    //    while(I2C2CON & 0x1F); // Acknowledge sequence not in progress
    // Receive sequence not in progress
    // Stop condition not in progress
    // Repeated Start condition not in progress
    // Start condition not in progress
    //    while(I2C2STATbits.TRSTAT); // Bit = 0 ? Master transmit is not in progress
    local_count = 0;
    do {
        local_count++;
        if (local_count > 65530) return;
    } while (I2C2STATbits.TRSTAT == 1); // Wait until ACK is received   
}

// I2C_start() sends a start condition  

void I2C_start() {
    I2C_wait_for_idle();
    I2C2CONbits.SEN = 1;

    while (I2C2CONbits.SEN == 1);
    I2C_wait_for_idle();
}

// I2C_restart() sends a repeated start/restart condition

void I2C_restart() {
    I2C_wait_for_idle();
    I2C2CONbits.RSEN = 1;
    while (I2C2CONbits.RSEN == 1);
}

// I2C_ack() sends an ACK condition

void I2C_ack(void) {
    I2C_wait_for_idle();
    I2C2CONbits.ACKDT = 0; // Set hardware to send ACK bit
    I2C2CONbits.ACKEN = 1; // Send ACK bit, will be automatically cleared by hardware when sent  
    while (I2C2CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

void I2C_nack(void) // Acknowledge Data bit
{
    I2C_wait_for_idle();
    I2C2CONbits.ACKDT = 1; // Set hardware to send NACK bit
    I2C2CONbits.ACKEN = 1; // Send NACK bit, will be automatically cleared by hardware when sent  
    while (I2C2CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning NACK bit has been sent
}

// I2C_stop() sends a stop condition  

void I2C_stop() {
    I2C_wait_for_idle();
    I2C2CONbits.PEN = 1;
}

// address is I2C slave address, set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking  

void I2C_write(unsigned char address, char wait_ack, char RW) {
    uint16_t local_count = 0;
    I2C2TRN = address | RW; // Send slave address with Read/Write bit cleared
    //    LATBbits.LATB14 = 1;

    I2C_wait_for_idle(); // Wait until I2C bus is idle
    // while (I2C2STATbits.ACKSTAT == 1); // Wait until ACK is received   
    if (wait_ack) {
        do {
            local_count++;
            if (local_count > 65530) return;
        } while (I2C2STATbits.ACKSTAT == 1); // Wait until ACK is received   
    }

}

void I2C_read(unsigned char *value, char ack_nack) {
    uint16_t local_count = 0;
    I2C2CONbits.RCEN = 1; // Receive enable
    while (I2C2CONbits.RCEN); // Wait until RCEN is cleared (automatic)  
    //    while (!I2C2STATbits.RBF);          // Wait until Receive Buffer is Full (RBF flag) 
    do {
        local_count++;
        if (local_count > 65530) return;
    } while (!I2C2STATbits.RBF);

    *value = I2C2RCV; // Retrieve value from I2C2RCV

    if (!ack_nack) // Do we need to send an ACK or a NACK?  
        I2C_ack(); // Send ACK  
    else
        I2C_nack(); // Send NACK  
}

void ledBlink1(void) {
    TRISA = 0; // Setting all the 32 bit register as output to avoid current sinking
    TRISB = 0;
    TRISC = 0;
    LATA = 0; // Setting latch as output to avoid current sinking
    LATB = 0;
    LATC = 0;
    TRISBbits.TRISB2 = 0; // We have accessed only 1 bit from TRISB register as we only need to blink 1 LED            
    LATBbits.LATB11 = 1;
    delay_us(1000000);
    LATBbits.LATB11 = 0;
    delay_us(1000000);
}

void ledBlink2(void) {
    TRISA = 0; // Setting all the 32 bit register as output to avoid current sinking
    TRISB = 0;
    TRISC = 0;
    LATA = 0; // Setting latch as output to avoid current sinking
    LATB = 0;
    LATC = 0;
    TRISBbits.TRISB2 = 0; // We have accessed only 1 bit from TRISB register as we only need to blink 1 LED            
    LATBbits.LATB11 = 1;
    delay_us(5000000);
    LATBbits.LATB11 = 0;
    delay_us(5000000);
}