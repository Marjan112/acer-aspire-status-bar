#include <stdio.h>
#include <stdlib.h>

int main()
{
    double loadavg;
    if (getloadavg(&loadavg, 1) < 0) {
        perror("getloadavg");
        return 1;
    }

    printf("Hot Loads: %.2f\n", loadavg);
    return 0;
}
