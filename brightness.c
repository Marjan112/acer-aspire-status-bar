#include <stdio.h>
#include <stdlib.h>

int read_sysfs(const char *path)
{
    FILE *f = fopen(path, "r");
    int v;
    fscanf(f, "%d", &v);
    fclose(f);
    return v; 
}

int main()
{
    int max_brightness = read_sysfs("/sys/class/backlight/intel_backlight/max_brightness");
    int brightness = read_sysfs("/sys/class/backlight/intel_backlight/brightness");

    int percent = brightness * 100 / max_brightness;

    printf("Brightness: %d%%\n", percent);
    
    return 0;
}
