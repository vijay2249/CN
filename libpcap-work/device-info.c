#include<stdio.h>
#include<pcap.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]){
  char *device, ip[13], subnetMask[13];
  bpf_u_int32 ip_raw; // ip address as integer
  bpf_u_int32 subnet_mask_raw; //subnet mask as integer
  int lookup_return_code;
  char error_buffer[PCAP_ERRBUF_SIZE];
  struct in_addr address; //used for both ip and subnet..

  //find a device
  device = pcap_lookupdev(error_buffer);
  if(device == NULL){
    printf("%s\n", error_buffer);
    return 1;
  }

  printf("network device found: %s\n", device);

  //get device info
  lookup_return_code = pcap_lookupnet(device, &ip_raw, &subnet_mask_raw, error_buffer);
  if(lookup_return_code == -1){
    printf("Error: %s\n", error_buffer);
    return 1;
  }
  
  address.s_addr = ip_raw;
  strcpy(ip, inet_ntoa(address));
  if(ip == NULL){
    perror("inet_ntoa"); //error
    return 1;
  }

  address.s_addr = subnet_mask_raw;
  strcpy(subnetMask, inet_ntoa(address));
  if(subnetMask == NULL){
    perror("inet_ntoa"); //error
    return 1;
  }

  printf("Device: %s\n", device);
  printf("IP Address: -%s-\n", ip);
  printf("Subnet mask: %s\n", subnetMask);
  
  return 0;
}
