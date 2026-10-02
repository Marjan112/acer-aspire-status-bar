#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <libgen.h>
#include <errno.h>

int main() {
    glob_t label_matches; 

    if(glob("/sys/class/hwmon/hwmon*/temp*_label", 0, NULL, &label_matches) != 0) {
        printf("No temp labels have been found: %s\n", strerror(errno));
        return 1;
    }

    for(size_t i = 0; i < label_matches.gl_pathc; ++i) {
        char *label_path = label_matches.gl_pathv[i];
        char *label_path_copy = strdup(label_path);
        char *label_base = basename(label_path_copy);

        FILE *label_file = fopen(label_path, "r");

        char label[16];
        fread(label, sizeof(label), 1, label_file);

        if(strstr(label, "Package") || strstr(label, "Tctl")) {
            char *parent = dirname(label_path);                
            int n;
            sscanf(label_base, "%*[^0-9]%d", &n); 
            
            char input_path[128];
            snprintf(input_path, sizeof(input_path), "%s/temp%d_input", parent, n);

            FILE *input_file = fopen(input_path, "r");

            int millideg;
            fscanf(input_file, "%d", &millideg);

            float deg = (float)millideg / 1000;

            const char *color;

            if(deg >= 80) color = "#ff0000";
            else if(deg >= 50) color = "#ffd11a";
            else color = "#478061";

            printf("<span foreground='%s'>CPU temp: %.1f°C</span>\n", color, deg);

            free(label_path_copy);
            fclose(input_file);
            fclose(label_file);
            globfree(&label_matches);
            return 0;
        }

        free(label_path_copy);
        fclose(label_file);
    }

    globfree(&label_matches);
    return 0;
}
