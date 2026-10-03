#include <stdio.h>
#include <stdlib.h>

#include "sysfs.h"

int main()
{
    int max_brightness = read_sysfs_int("/sys/class/backlight/intel_backlight/max_brightness");
    int brightness = read_sysfs_int("/sys/class/backlight/intel_backlight/brightness");

    int percent = brightness * 100 / max_brightness;

    printf("Brightness: %d%%\n", percent);
    
    return 0;
}
