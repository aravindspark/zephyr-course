
#ifndef MYPICO2_H
#define MYPICO2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <zephyr/device.h>
#include <zephyr/toolchain.h>
#include <zephyr/drivers/sensor.h>

__subsystem struct mypico2_driver_api {
    struct sensor_driver_api parent_api;
    /**
    *   Custom API to set led state from main.c
    */
    int (*set_led_state) (const struct device *dev, unsigned int led_state);
};

DEVICE_API_EXTENDS(mypico2, sensor, parent_api);

__syscall int mypico2_set_led_state(const struct device *dev,
                unsigned int led_state);

static inline int z_impl_mypico2_set_led_state(const struct device *dev,
                        unsigned int led_state) {

    __ASSERT_NO_MSG(DEVICE_API_IS(sensor, dev));
    __ASSERT_NO_MSG(DEVICE_API_IS(my_pico2driver, dev));

    return DEVICE_API_GET(mypico2, dev)->set_led_state(dev, led_state);
}

#include <zephyr/syscalls/mypico2.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MYPICO2_H */
