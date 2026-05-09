#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#include <mypico2.h>

#define LED_DEFAULT_BLINK_TIME 3000

/* Use the USB CDC ACM - Lack for serial cable */
BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart), "Console device is not ACM CDC UART device");

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = false;
    struct sensor_value sval;
    const struct device* sensor_dev = DEVICE_DT_GET(DT_NODELABEL(my_pico2driver0));
    int ret = -1;

    const struct device *usb_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));    
    uint32_t dtr = 0;

    /* Poll if the DTR flag was set */
    while (!dtr) {
        uart_line_ctrl_get(usb_dev, UART_LINE_CTRL_DTR, &dtr);
        /* Give CPU resources to low priority threads. */
        k_sleep(K_MSEC(100));
    }   

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
        /*Custom API to set led state*/
        mypico2_set_led_state(sensor_dev, led_state);
#if CONFIG_APP_HEARTBEAT_PERIOD_MS
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
#else
        k_msleep(LED_DEFAULT_BLINK_TIME);
#endif

    }
    return 0;
}
