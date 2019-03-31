The ISR's are in this project because the framework uses msp430Driver modules that require them.
And generally, a client of msp430Drivers might want to modify the ISR.


Note there are duplicates for Timer.
Depends on the msp430Drivers/config.h
Whether Timer uses WDT or internal RTC0.