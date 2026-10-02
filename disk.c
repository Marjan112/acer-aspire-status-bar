#include <stdio.h>
#include <stdint.h>
#include <sys/statvfs.h>

int main() 
{
    struct statvfs stat; 

    if (statvfs("/", &stat) != 0) {
        perror("statvfs");
        return 1;
    }

    uint64_t block_size = stat.f_frsize;
    uint64_t total_bytes = stat.f_blocks * block_size;
    uint64_t free_bytes = stat.f_bfree * block_size;

    float total_gib = (float)total_bytes / (1024 * 1024 * 1024);
    float free_gib = (float)free_bytes / (1024 * 1024 * 1024);

    printf("Disk: %.1f/%.1f\n", total_gib - free_gib, total_gib);
    return 0;
}
