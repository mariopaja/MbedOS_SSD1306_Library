# SSD1306 Library

SSD1306 Library is a MbedOS library used for the SSD1306 OLED display (128x64).<br/>
Library contains functions only to print characters and lines.<br/>
Library is stripped out of any graphics or images.

## Installation

```bash
Import Library... -> Git or os.mbed.com URL
```

## Implementation Example

```C++
#include "mbed.h"
#include "SSD1306.h"


SSD1306 display(D14, D15);

int main(){
    display.setSpeed(SSD1306::Medium);  
    display.init();  

    display.clearScreen(); 

    display.setCursor(0,2);             
    display.printf("Hello World!"); 
    display.drawLine(5, 8, 15, 25);
        
    display.refreshDisplay();

    unsigned char level=255;  
    display.setBrightness(level);

    while(true){
        wait_us (20000);
    }

}
```

## Disclaimer
This code was tested ony on STM32 Nucleo-64 F446RE board

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

