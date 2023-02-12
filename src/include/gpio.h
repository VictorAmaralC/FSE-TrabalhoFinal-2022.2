#ifndef GPIO_H
#define GPIO_H

typedef struct{
    int channel;
    int gpio;
    int mode;
    int timerIndex;
} ledc_info_t;

void configureBuzzer();
void configureRGB();
void configureFlame();
void setUpPwm(double intensity);
void playSound();
void stopSound();
void changeColor(uint8_t red, uint8_t green, uint8_t blue);

#endif