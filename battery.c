#include <stdio.h>

#include "sysfs.h"

int main()
{
    int capacity = read_sysfs_int("/sys/class/power_supply/BAT1/capacity");
    const char *status = read_sysfs_str("/sys/class/power_supply/BAT1/status");

    const char *color;
    if (capacity <= 50) color = "#ffff00";
    else if (capacity <= 20) color = "#ff0000";
    else color = "#478061";

    printf("<span foreground='%s'>Battery [%s]: %d%%</span>\n", color, status, capacity);
    return 0;
}
