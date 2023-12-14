Code To-Do
- speed problems 
    - MCU? Inefficient code? Slow ADC?
- cal function and control of cal (redefine interupt functions, one define fkor cal and another for ranges???)

- define stm32F3 in stm32_def.h????????




- NOT LIEING, USER ERROR, CODE TRIES TO USE 16, but is limited by the 12 bit SAR adc, SDADC is not being configured!!!!!!!!!!
- lieing about 16 bit res??
- YES 16 bit not defined and defulting to 12 bit resolution (4,012 vs 65,512!!!)

- conclution, reading at 16 bits because no bit shifting in the 12 bit hexadecimal definition. 10 bits is shifted over and so is 8???
ex: 0x1UL = 0x (hexadecimal indicator), 1 (hexadecimal value), UL (unsigned long)
0x00000010 = 16 in hexadecimal
0x10 = 16 in hexadecimal
0x1 = 1 in hexadecimal






DMM circuit design V2

- two diffrent opamp power supplies to prevent adc over voltage
- adc conditioning redesign
- mux / analog switches insted of optocouplers in most spots
- ptc input protection (UL / safty rating ability??)
- seperate adc that is BIPOLAR and diffrential
- use touch screen
- usbc input and programing
- regulator and programer backfeed protection
- better SWD routing and FIXED swd programing header
- proper reset line debounceing circuit
- external crystal problems??? use diffrent MCU (just add new board to platform.ini and maybe boards folder)?????
- SEPERATE DIGITAL AND ANALOG BOARDS maybe power supply seperate 
- ACV and ACI functionality
- To Be Continued.....

- ohms adding measuremnet error, CC sourse needs isolation / control connection to input connection