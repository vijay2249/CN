/* Compile with: gcc find_device.c -lpcap */
#include <stdio.h>
#include <pcap.h>

int main(int argc, char **argv) {
  char *device; /* Name of device (e.g. eth0, wlan0) */
  char error_buffer[PCAP_ERRBUF_SIZE]; /* Size defined in pcap.h */

  /* Find a device */
  device = pcap_lookupdev(error_buffer);
  if (device == NULL) {
    printf("Error finding device: %s\n", error_buffer);
    return 1;
  }

  printf("Network device found: %s\n", device);
  return 0;
}


// ------- new used pcap_findalldevs() function -> read abput it...
// /* Compile with: gcc find_device.c -lpcap */
// #include <stdio.h>
// #include <pcap.h>
// int main(int argc, char **argv) {
//   int device; /* Name of device (e.g. eth0, wlan0) */
//   char error_buffer[PCAP_ERRBUF_SIZE]; /* Size defined in pcap.h */
//   pcap_if_t *ift = NULL;
//   // pcap_if_t *it = ift;
//   /* Find a device */
//   device = pcap_findalldevs(&ift, error_buffer);
//   if (device != 0) {
//     printf("Error finding device: %s\n", error_buffer);
//     return 1;
//   }
//   printf("Network device found: %s\n", device);
//   return 0;
// }
