#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>


#define LED_DEFAULT_BLINK_TIME 3000

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = false;
    struct sensor_value sval;
    const struct device* sensor_dev = DEVICE_DT_GET(DT_NODELABEL(my_pico2driver0));
    int ret = -1;

#ifndef CONFIG_APP_HEARTBEAT_PERIOD_MS
        LOG_INF("Using defaut blink time: %u ms", LED_DEFAULT_BLINK_TIME);
#endif
    while (1) {

        if (led_state) {
            // Turn off the LED
            ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &sval);
        } else {
            // Turn on the LED
            ret = sensor_sample_fetch(sensor_dev);
        }
        led_state = !led_state;

#if CONFIG_APP_HEARTBEAT_PERIOD_MS
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
#else
        k_msleep(LED_DEFAULT_BLINK_TIME);
#endif

    }
    return 0;
}
