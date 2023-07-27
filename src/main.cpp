#include "pico/stdlib.h"
#include "stdio.h"
#include "pico_explorer.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"
#include "button.hpp"
#include "analog.hpp"
#include "encoder.hpp"

using namespace motor;
using namespace encoder;

Motor motor_A(PicoExplorer::MOTOR2_PINS);
Analog pot(PicoExplorer::ADC2_PIN);
Encoder enc(pio0, 0, {6, 7});

bool state = false;

int main() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    stdio_init_all();
    motor_A.init();
    enc.init();
    uint32_t i = 0;
    while(true) {

        auto voltage = pot.read_voltage();
        auto speed = voltage / 3.3f;
        motor_A.speed(0.0f);
//        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(10);
        i++;
        if(i % 20 == 0){
            printf("Count = %ld, ", enc.count());
            printf("Delta = %ld, ", enc.delta());
            printf("Step = %d, ", enc.step());
            printf("Turn = %d\n", enc.turn());
            state = !state;
            gpio_put(PICO_DEFAULT_LED_PIN, state);
        }
    }

}
