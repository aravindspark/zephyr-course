#include<zephyr/shell/shell.h>
#include<zephyr/drivers/sensor.h>

static int subcmd_fetch_handler(const struct shell * sh, size_t argc, char **argv)
{
    int ret;
    const struct device * sensor_dev = shell_device_get_binding("my_pico2driver");

    if(!sensor_dev) {
        shell_error(sh, "Could not find the device");
        return -ENODEV;
    }

    ret = sensor_sample_fetch(sensor_dev);
    if(ret !=0) {
        shell_error(sh, "Error in channel fetch - %d", ret);
        return -EFAULT;
    }
    shell_info(sh, "Sensor channel fetch Success!");

    return 0;
}

static int subcmd_read_handler(const struct shell * sh, size_t argc, char **argv)
{
    struct sensor_value sval;
    int ret;

    const struct device * sensor_dev = shell_device_get_binding("my_pico2driver");

    if(!sensor_dev) {
        shell_error(sh, "Could not find the device");
        return -ENODEV;
    }

    ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &sval);

    if(ret !=0) {
        shell_error(sh, "Could not get channel, error - %d", ret);
        return -EFAULT;
    }

    shell_info(sh, "Sensor channel get successs! val = %d-%d", sval.val1, sval.val2);

    return 0;
}

static int subcmd_info_handler(const struct shell * sh, size_t argc, char **argv)
{
    const struct device * sensor_dev = shell_device_get_binding("my_pico2driver");


    if(!sensor_dev) {
        shell_error(sh, "Could not find the device");
        return -ENODEV;
    }

    bool is_ready = device_is_ready(sensor_dev);

    shell_info(sh, "Info :\n Device name : my_pico2driver\n Status : %s", is_ready? "READY":"NOT READY");

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(mypico2driver_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "fetch channel of mypico2driver", &subcmd_fetch_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "get channel of mypico2driver",  &subcmd_read_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "print info of mypico2driver", &subcmd_info_handler, 1, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &mypico2driver_subcmd, "mypico2 driver sensor commands", NULL);
