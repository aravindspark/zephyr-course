#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT my_pico2driver
/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

LOG_MODULE_REGISTER(my_pico2driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static int my_pico2driver_led_on(const struct device *dev,
                                    enum sensor_channel chan) {
    
    if (gpio_pin_toggle_dt(&led) < 0) return 0;
    LOG_INF("LED state: ON");

    return 0;
}

static int my_pico2driver_led_off(const struct device *dev,
                                    enum sensor_channel chan,
                                    struct sensor_value *val) {

    if (gpio_pin_toggle_dt(&led) < 0) return 0;
    LOG_INF("LED state: OFF");

    return 0;
}

static DEVICE_API(sensor, api_led_pico2driver) = {
    .sample_fetch = my_pico2driver_led_on,
    .channel_get = my_pico2driver_led_off,
};

static int my_pico2driver_init(const struct device *dev) {

    LOG_INF("my_pico2driver_init Initialized!");

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    
    return 0;
} 

DEVICE_DT_INST_DEFINE(0, my_pico2driver_init, NULL, NULL, NULL, POST_KERNEL, 80, &api_led_pico2driver);
