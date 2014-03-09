#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void play_tone();
void play_song();
void stopTimer();
void startTimer();
void setupDAC();
void disableDAC();
void generate_blip();
void generate_laser();
void generate_blurp();
void button_handler();

int mode = -1;

void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
    *TIMER1_IFC = 1;
    if (mode == 0)
        play_tone();
    if (mode == 1)
        play_song();
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    *GPIO_IFC = 0xff;

    button_handler();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    *GPIO_IFC = 0xff;

    button_handler();
}

void button_handler() {
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);

    int current_button = -1;
    int input = ~(*GPIO_PC_DIN);
    for (int i=0; i < 8; i++) {
        if ((1 << i) == (input & (1 << i))) {
            current_button = i;
            break;
        }
    }
    switch (current_button) {
        case 0:
            mode = 0;
            generate_blip();
            startTimer();
            setupDAC();
            break;
        case 1:
            mode = 0;
            generate_laser();
            startTimer();
            setupDAC();
            break;
        case 2:
            mode = 0;
            generate_blurp();
            startTimer();
            setupDAC();
            break;
        case 3:
            mode = 1;
            startTimer();
            setupDAC();
            break;
        default:
            if (current_button != -1) {
                stopTimer();
                disableDAC();
            }
            break;
    }
}
