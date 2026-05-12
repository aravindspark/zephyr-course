#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <mypico2.h>

#define DT_DRV_COMPAT my_pico2driver
/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

LOG_MODULE_REGISTER(my_pico2driver, LOG_LEVEL_INF);

struct my_pico2driver_data {
    unsigned int led_state;
};

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static int my_pico2driver_led_on(const struct device *dev,
                                    enum sensor_channel chan) {
    struct my_pico2driver_data *data = dev->data;


    if(data == NULL)
        return 0;

    if (gpio_pin_set_dt(&led, 1 ) < 0) return 0;

    LOG_INF("LED state: ON");

    return 0;
}

static int my_pico2driver_led_off(const struct device *dev,
                                    enum sensor_channel chan,
                                    struct sensor_value *sval) {

    struct my_pico2driver_data *data = dev->data;

    if(data == NULL)
        return 0;

    if (gpio_pin_set_dt(&led, 0 ) < 0) return 0;

    LOG_INF("LED state: OFF");

    if( sval != NULL) {
        sval->val1 = 2026;
        sval->val2 = 5;
    }
    return 0;
}

static int my_pico2driver_set_led_state(const struct device *dev,
                                            unsigned int led_state)
{

    struct my_pico2driver_data *data = dev->data;

    if(data != NULL)
        /* Just set the led state*/
        data->led_state = led_state;

    LOG_INF("new sensor state is: %u", data->led_state);

    return 0;
}

static DEVICE_API(mypico2, led_pico2driver_api) = {
    .parent_api = {
        .sample_fetch = my_pico2driver_led_on,
        .channel_get = my_pico2driver_led_off,
    },
    .set_led_state = my_pico2driver_set_led_state,
};

static struct my_pico2driver_data drv_data;


static int my_pico2driver_init(const struct device *dev) {

    LOG_INF("my_pico2driver_init Initialized!");

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    return 0;
}

DEVICE_DT_INST_DEFINE(0, my_pico2driver_init, NULL, &drv_data, NULL, POST_KERNEL, 80, &led_pico2driver_api);
