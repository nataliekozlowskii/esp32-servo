#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/mcpwm.h"
#include "esp_log.h"
#include "esp_mac.h"

#define SERVO_GPIO     25
#define FLEX_ADC_CH    ADC1_CHANNEL_7 // GPIO 35
#define SERVO_MIN_uS   1000 // 1 ms high pulse --> servo rotates to min 0° position
#define SERVO_MAX_uS   2000 // 2 ms high pulse --> servo rotates to max 180° position

static const char *TAG = "FLEX_SERVO";

void rotate_servo(int* adc_val, int* pulse_us) {
    /*
     * @brief Rotates servo based on ADC value read from the flex sensor.
     * @param adc_val Pointer to store ADC reading (0 – 4095).
     * @param pulse_us Pointer to store calculated servo pulse width (in us).
    */
    *adc_val = adc1_get_raw(FLEX_ADC_CH);
    *pulse_us = SERVO_MIN_uS + (*adc_val * (SERVO_MAX_uS - SERVO_MIN_uS)) / 4095;
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, *pulse_us);
}

extern "C" void app_main() {
    adc1_config_width(ADC_WIDTH_BIT_12); // ADC will read values 0 - 4095
    adc1_config_channel_atten(FLEX_ADC_CH, ADC_ATTEN_DB_12); // increase max measurable V to ~3.3V

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_GPIO); // init MCPWM so signal is sent to SERVO_GPIO

    mcpwm_config_t pwm_config = {
        .frequency = 50,
        .cmpr_a = 0, // start w/ no pulse
        .duty_mode = MCPWM_DUTY_MODE_0, // signal is high for duty cycle
        .counter_mode = MCPWM_UP_COUNTER,
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    int adc_val, pulse_us;
    
    while (true) {
        adc_val = adc1_get_raw(FLEX_ADC_CH);
        rotate_servo(&adc_val, &pulse_us);
        ESP_LOGI(TAG, "Flex ADC: %d", adc_val);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}