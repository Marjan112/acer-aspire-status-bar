#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <linux/wireless.h>

int main()
{
    struct ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) < 0) {
        perror("getifaddrs");
        return 1;
    }

    const char *ifprefix = "wl";
    const size_t ifprefix_len = strlen(ifprefix);

    char ifname[IFNAMSIZ] = {0};

    for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        if (strncmp(ifa->ifa_name, ifprefix, ifprefix_len) == 0) {
            strncpy(ifname, ifa->ifa_name, strlen(ifa->ifa_name));
            break;
        }
    }

    if (*ifname == '\0') {
        puts("<span foreground='#ff0000'>W: No interface</span>");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct iwreq iw = {0};
    strncpy(iw.ifr_name, ifname, strlen(ifname));

    char ssid[IW_ESSID_MAX_SIZE] = {0};
    iw.u.essid.pointer = ssid;
    iw.u.essid.length = IW_ESSID_MAX_SIZE;

    if (ioctl(sock, SIOCGIWESSID, &iw) < 0) {
        puts("<span foreground='#ff0000'>W: down</span>");
        close(sock);
        freeifaddrs(ifaddr);
        return 0;
    }

    struct iw_statistics stats;
    iw.u.data.pointer = &stats;
    iw.u.data.length = sizeof(stats);

    if (ioctl(sock, SIOCGIWSTATS, &iw) < 0) {
        printf("<span foreground='#ff0000'>W: %s</span>\n", strerror(errno));
        close(sock);
        freeifaddrs(ifaddr);
        return 1;
    }

    int percent = ((float)stats.qual.qual) * 100 / 70;

    const char *color;
    if (percent < 70) color = "#748047";
    else if (percent < 50) color = "#ffea00";
    else if (percent < 30) color = "#ff7b00";
    else if (percent < 20) color = "#ff0000";
    else color = "#478061";

    printf("<span foreground='%s'>%s: %d</span>\n", color, ssid, percent);

    close(sock);
    freeifaddrs(ifaddr);
    return 0;
}
