MSP430
======

sample code for the MSP430 Texas Instruments Microcontroller

A mystery, solved!


SPI Communication

I got it. I never understood how this works, but now i got it. I have not understood yet where the TXBUF and RXBUF are useful in the SPI communication. Maybe it is only needed for I2C. 


My Goal was to send data from one MSP to another via SPI to make Projects possible that are using SPI controlled Devices. The 2.4GhZ Transceivers for example.


In this example i use two MSP430G2452 on launchpads. Both have LEDs connected on the PORT2 at 2.3, 2.4 and 2.5. The Master has three Buttons connected to P2.0, 2.1 and 2.2.


The Launchpads have also the Connections shown in the original TI samples usi2 and usi3 of the ti resource Explorer. These are connect 1.5 with 1.5, connect master 1.6 with slave 1.7. Connect Master 1.7 with slave 1.6. And connect master 1.2 with the slaves rst pin.


There is a Thing that is not made very clean in this code. The spisend method is called from the button Interrupt Routine. That is not good coding.


I send the data by simply putting a value to USISRL, and then push it out with Setting USICNT to eight. I did not understand it completely. So i do not know why the master Need the USI Interrupt Routine even if i only send to the slave.



You can see the Post for this on www.tns-labs.org
