#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "include/gpio.h"

#define RGB_RED       21
#define RGB_GREEN     19
#define RGB_BLUE      18
#define BUZZER_GPIO   22
#define BUTTON_GPIO    0
#define FLAME_GPIO     5
#define LED_GPIO       2

int estado = 0;
double intensity = 0.0f;

ledc_info_t ledc_ch[3];

void configureBuzzer() {
    esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
}

void configureRGB() {
    esp_rom_gpio_pad_select_gpio(RGB_RED);
    esp_rom_gpio_pad_select_gpio(RGB_GREEN);
    esp_rom_gpio_pad_select_gpio(RGB_BLUE);

    gpio_set_direction(RGB_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(RGB_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RGB_BLUE, GPIO_MODE_OUTPUT);
}

void configureFlame() {
    esp_rom_gpio_pad_select_gpio(FLAME_GPIO);
    gpio_set_direction(FLAME_GPIO, GPIO_MODE_INPUT);
}

void changeColor(uint8_t red, uint8_t green, uint8_t blue) {

    ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, red);
    ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);

    ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, green);
    ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);

    ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel, blue);
    ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);
        
}

void setUpPwm(double intensity) {

    esp_rom_gpio_pad_select_gpio(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    int rgb_ch;

    ledc_ch[0].channel    = LEDC_CHANNEL_0;
    ledc_ch[0].gpio       = RGB_RED;
    ledc_ch[0].mode       = LEDC_HIGH_SPEED_MODE;
    ledc_ch[0].timerIndex = LEDC_TIMER_0;

    ledc_ch[1].channel    = LEDC_CHANNEL_1;
    ledc_ch[1].gpio       = RGB_GREEN;
    ledc_ch[1].mode       = LEDC_HIGH_SPEED_MODE;
    ledc_ch[1].timerIndex = LEDC_TIMER_0;

    ledc_ch[2].channel    = LEDC_CHANNEL_2;
    ledc_ch[2].gpio       = RGB_BLUE;
    ledc_ch[2].mode       = LEDC_HIGH_SPEED_MODE;
    ledc_ch[2].timerIndex = LEDC_TIMER_0;


    // Configuração do Timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Configuração do Canal
    for(rgb_ch = 0; rgb_ch < 3; rgb_ch++){
    ledc_channel_config_t channel_config = {
        .gpio_num = ledc_ch[rgb_ch].gpio,
        .speed_mode = ledc_ch[rgb_ch].mode,
        .channel = ledc_ch[rgb_ch].channel,
        .timer_sel = ledc_ch[rgb_ch].timerIndex,
        .duty = 0,
        .hpoint = 0
    };
        ledc_channel_config(&channel_config);
    }


    // Configuração led gpio
    int pwm = (int)255 * (intensity / 100);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

}

void playSound() {
    // Configuração do Timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Configuração do Canal
    ledc_channel_config_t channel_config = {
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_config);

    while(true)
    {
        for(int i = 0; i < 65535; i++)
        {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        for(int i = 65535; i > 0; i--)
        {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

void stopSound() {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void playSoundDashboard(double intensity) {
    esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);

    // Configuração do Timer
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    // Configuração do Canal
    ledc_channel_config_t channel_config = {
        .gpio_num = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_config);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, intensity * 100);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}
