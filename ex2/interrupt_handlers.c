#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "proto.h"

int mode = -1;
extern int i;

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
    button_handler();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    button_handler();
}

void button_handler() {
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
    *GPIO_IFC = 0xff;

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
            setup_song();
            break;
        case 1:
            mode = 0;
            generate_laser();
            setup_song();
            break;
        case 2:
            mode = 0;
            generate_blurp();
            setup_song();
            break;
        case 3:
            mode = 1;
            setup_song();
            break;
        default:
            if(current_button != -1) {
                mode = 0;
                generate_alert();
                setup_song();
            }
            break;
    }
}
