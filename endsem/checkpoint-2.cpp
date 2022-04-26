#include "includes.h"
using namespace std;

void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void process_ip_packet(const u_char * , int);
void print_ip_packet(const u_char * , int);
void print_tcp_packet(const u_char *  , int );
void print_udp_packet(const u_char * , int);
void print_icmp_packet(const u_char * , int );
void PrintData (const u_char * , int);
void print_packets_count();
void add_to_arp_table(const u_char *buffer, int size);
void print_arp_table(int sizes);
void print_arp_table_from_vector();
void packet_injection_for_arp();
 
FILE *logfile; //to store the output
FILE* obsFile;
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
#define total_cap_count 5
pcap_t *handle; //Handle of the device that shall be sniffed
 
int main(){
    pcap_if_t *alldevsp , *device;
    char errbuf[100] , *devname , devs[100][100];
    int count = 1 , n;
    // get the list of available devices
    printf("Finding available devices ... ");
    if( pcap_findalldevs( &alldevsp , errbuf) ){
        printf("Error finding devices : %s" , errbuf);
        exit(1);
    }
    printf("Done");
    //Print the available devices
    printf("\nAvailable Devices are :\n");
    for(device = alldevsp ; device != NULL ; device = device->next){
        printf("%d. %s - %s\n" , count , device->name , device->description);
        if(device->name != NULL){
            strcpy(devs[count] , device->name);
        }
        count++;
    }
    //Ask user which device to sniff
    // printf("Enter the number of the device you want to sniff : ");
    // printf("sniffing from the device eth0....");
    // scanf("%d" , &n);
    devname = devs[1];
    //Open the device for sniffing
    printf("Opening device %s for sniffing ... " , devname);
    handle = pcap_open_live(devname , 65535 , 1 , 0 , errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s : %s\n" , devname , errbuf);
        exit(1);
    }
    printf("Done\n\n");
    logfile=fopen("log.txt","w");
    obsFile=fopen("obs.txt","w");
    if(logfile==NULL) {
        printf("Unable to create file.");
    }
    //Put the device in sniff loop
    pcap_loop(handle , total_cap_count, process_packet , NULL);
    print_arp_table_from_vector();
    packet_injection_for_arp();
    return 0;
}

bool p8090 = false;
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer){
    int size = header->len;
     
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
        case 1:  //ICMP Protocol
            ++icmp;
            print_icmp_packet( buffer , size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            print_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            print_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    // print_packets_count();
    add_to_arp_table(buffer, size);
}

void print_packets_count(){printf("Packets count: %d\n", total);}

struct ARP_table{
    unsigned char *source_mac;
    char *source_ip;
};

vector<ARP_table>ARPTABLE;

void packet_injection_for_arp(){
    printf("\n--------------Packet Injection using pcap_sendpacket-----------\n");
    // changing the mac address of the arp table
    // make a circular loop around the mac address to the ip address
    printf("Before changing data\n");
    print_arp_table_from_vector();
    unsigned char *temp = ARPTABLE[0].source_mac;
    for(int a=0;a<ARPTABLE.size()-1;a++){
        ARPTABLE[i].source_mac = ARPTABLE[i+1].source_mac;
    }
    ARPTABLE[ARPTABLE.size()-1].source_mac = temp;
    printf("ARP table after changing values....");
    print_arp_table_from_vector();
    const u_char *data;
    for(auto i: ARPTABLE){
        memset(&data, 0, sizeof(data));
        sprintf(data,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X \t IP:\n", i.source_mac[0],i.source_mac[1],i.source_mac[2],i.source_mac[3],i.source_mac[4],i.source_mac[5]);
        // strcpy(data, i.source_ip);
        if(pcap_sendpacket(handle, data, sizeof(data)) != 0){
            printf("pcap packet sent error\n");
            exit(1);
        }
    }
}

void print_arp_table_from_vector(){
    int a=1;
    printf("Printing values from vector\n");
    for(auto i: ARPTABLE){
        printf("MAC%d: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \tIP%d: %s\n",
        a, i.source_mac[0],i.source_mac[1],i.source_mac[2],i.source_mac[3],i.source_mac[4],i.source_mac[5],
        a, i.source_ip);
        a++;
    }
}

void add_to_arp_table(const u_char *buffer, int size){
    struct ethhdr *eth = (struct ethhdr *)buffer;
    struct ARP_table arp_table;
    arp_table.source_mac = eth->h_source;
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    //arp_table.source_ip = inet_ntoa(source.sin_addr);
    strcpy(arp_table.source_ip,inet_ntoa(source.sin_addr));
    printf("MAC%d: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \tIP%d: %s\n", 
            total, arp_table.source_mac[0], arp_table.source_mac[1], arp_table.source_mac[2], arp_table.source_mac[3], arp_table.source_mac[4], arp_table.source_mac[5],
            total, arp_table.source_ip);
    ARPTABLE.push_back(arp_table);
}


 
void print_ethernet_header(const u_char *Buffer, int Size){
    struct ethhdr *eth = (struct ethhdr *)Buffer;
    fprintf(logfile , "\n");
    fprintf(logfile , "Ethernet Header\n");
    fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}
 
void print_ip_header(const u_char * Buffer, int Size){
    print_ethernet_header(Buffer , Size);
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile , "   |-IP Version        : %d\n",(unsigned int)iph->version);
    fprintf(logfile , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile , "   |-Identification    : %d\n",ntohs(iph->id));
    fprintf(logfile , "   |-TTL               : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile , "   |-Protocol          : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile , "   |-Checksum          : %d\n",ntohs(iph->check));
    fprintf(logfile , "   |-Source IP         : %s\n" , inet_ntoa(source.sin_addr) );
    fprintf(logfile , "   |-Destination IP    : %s\n" , inet_ntoa(dest.sin_addr) );
}
 
void print_tcp_packet(const u_char * Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
     
    fprintf(logfile , "\n\n***********************TCP Packet*************************\n");  
         
    print_ip_header(Buffer,Size);
         
    fprintf(logfile , "\n");
    fprintf(logfile , "TCP Header\n");
    fprintf(logfile , "   |-Source Port      : %u\n",ntohs(tcph->source));
    fprintf(logfile , "   |-Destination Port : %u\n",ntohs(tcph->dest));
    fprintf(logfile , "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    fprintf(logfile , "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    fprintf(logfile , "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    fprintf(logfile , "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    fprintf(logfile , "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    fprintf(logfile , "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    fprintf(logfile , "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    fprintf(logfile , "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    fprintf(logfile , "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    fprintf(logfile , "   |-Window         : %d\n",ntohs(tcph->window));
    fprintf(logfile , "   |-Checksum       : %d\n",ntohs(tcph->check));
    fprintf(logfile , "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    fprintf(logfile , "\n");
    fprintf(logfile , "                        DATA Dump                         ");
    fprintf(logfile , "\n");
         
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    fprintf(logfile , "Data Payload\n");    
    PrintData(Buffer + header_size , Size - header_size );
                         
    fprintf(logfile , "\n###########################################################");
    
    uint16_t dstPort = ntohs(tcph->dest);
    if(dstPort==8090)cout<<"Packet captured b/w Client and CS1\n";
    else if(dstPort==8092)cout<<"Packet captured b/w Client and CS2\n";
    //fprintf(obsFile,"A Client sent a packet to CS1\n");
   // else if(dstPort==8092)fprintf(obsFile,"A Client sent a packet to CS2\n");
}
 
void print_udp_packet(const u_char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    fprintf(logfile , "\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer,Size);           
     
    fprintf(logfile , "\nUDP Header\n");
    fprintf(logfile , "   |-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(logfile , "   |-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(logfile , "   |-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile , "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer , iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , Size - header_size);
     
    fprintf(logfile , "\n###########################################################");
}
 
void print_icmp_packet(const u_char * Buffer , int Size){
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
    fprintf(logfile , "\n\n***********************ICMP Packet*************************\n"); 
     
    print_ip_header(Buffer , Size);
             
    fprintf(logfile , "\n");
         
    fprintf(logfile , "ICMP Header\n");
    fprintf(logfile , "   |-Type : %d",(unsigned int)(icmph->type));
             
    if((unsigned int)(icmph->type) == 11)
    {
        fprintf(logfile , "  (TTL Expired)\n");
    }
    else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
    {
        fprintf(logfile , "  (ICMP Echo Reply)\n");
    }
     
    fprintf(logfile , "   |-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(logfile , "   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(logfile , "   |-Sequence : %d\n",ntohs(icmph->sequence));
    fprintf(logfile , "\n");
 
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer + iphdrlen , sizeof icmph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , (Size - header_size) );
     
    fprintf(logfile , "\n###########################################################");
}
 
void PrintData (const u_char * data , int Size){
   //u_char *ptr=(u_char *)data;
 //  const char* S1 = reinterpret_cast<const char*>(data);
  // fprintf(logfile,"%s\n",S1);
    int i , j;
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            fprintf(logfile , "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    fprintf(logfile , "%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else fprintf(logfile , "."); //otherwise print a dot
            }
            fprintf(logfile , "\n");
        } 
         
        if(i%16==0) fprintf(logfile , "   ");
            fprintf(logfile , " %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) 
            {
              fprintf(logfile , "   "); //extra spaces
            }
             
            fprintf(logfile , "         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) 
                {
                  fprintf(logfile , "%c",(unsigned char)data[j]);
                }
                else
                {
                  fprintf(logfile , ".");
                }
            }
             
            fprintf(logfile ,  "\n" );
        }
    }
}
