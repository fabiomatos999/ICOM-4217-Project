#include <msp430.h>

#define SENSOR_PIN BIT0 // Defines the temperature sensor pin as P1.0

int main(void)
{
    int voltage;
    float temperature;

    WDTCTL = WDTPW + WDTHOLD;

    ADC10CTL1 = INCH_0 + ADC10DIV_3; // Setting the input channel for the ADC to P1.0 and configuring the ADC clock divider to obtain the desired clock frequency

    // ADC10CTL1 = register in the MSP430 microcontroller that controls the behavior of the Analog-to-Digital Converter (ADC)
    // INCH_0 = constant defined in the header file msp430.h that represents the input channel for the ADC.
    // ADC10DIV_3 = constant that sets the clock divider for the ADC. It is set to divide the clock source by 4, which gives a clock frequency of approximately 250 kHz

    ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // Setting the sample-and-hold time to 64 clock cycles, enabling the ADC, and enabling interrupts for the ADC

    // ADC10CTL0 = control register for ADC10
    // ADC10SHT_3 = sets the sample-and-hold time for the ADC to 64 clock cycles
    // ADC10ON = enables the ADC
    // ADC10IE = enables interrupts for the ADC

    ADC10AE0 |= SENSOR_PIN; // Enable analog input on the sensor pin

    // ADC10AE0 = register that enables the analog input

    while (1) // Reads the voltage value from the temperature sensor, converts it to a temperature value
    {
        ADC10CTL0 |= ENC + ADC10SC; // Is used to start a new analog-to-digital conversion 
        
        // ENC = (enable conversion) is set to enable the ADC to start the conversion
        // ADC10SC = (start conversion) is set to initiate the conversion process

        __bis_SR_register(CPUOFF + GIE); // It is a built-in function in the MSP430, puts the CPU in low-power mode and enables interrupts to wake it up when the ADC conversion is complete.


        voltage = ADC10MEM; // Save the voltage reading to the integer variable

        temperature = ((float)voltage * 3.3 / 1024 - 0.5) * 100; // Convert the voltage reading to a temperature value using the TMP36 transfer function

    }
}
