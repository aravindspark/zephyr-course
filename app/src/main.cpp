#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)
#define LED_DEFAULT_BLINK_TIME 3000

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

#ifndef CONFIG_BLINK_SLEEP_TIME_MS
        LOG_INF("Using defaut blink time: %u ms", LED_DEFAULT_BLINK_TIME);
#endif
    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
#if CONFIG_BLINK_SLEEP_TIME_MS
        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
#else
        k_msleep(LED_DEFAULT_BLINK_TIME);
#endif

    }
    return 0;
}
