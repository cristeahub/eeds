#ifndef __PROTO__H
#define __PROTO__H

void setup_song();
void stop_song();
void play_tone();
void play_song();
void stopTimer();
void startTimer();
void setupDAC();
void disableDAC();
void generate_blip();
void generate_laser();
void generate_blurp();
void generate_alert();
void button_handler();
void setupTimer();
void disableTimer();
void startTimer();
void setSleep(int);
void setupNVIC();
void setupGPIO();

#endif
