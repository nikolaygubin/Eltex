#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <net/ethernet.h>
#include <net/if.h>

#define IP_SRC "127.0.0.2"
#define IP_DEST "127.0.0.2"
#define PORT 8080
#define CLIENT_PORT 7777
#define BUF_SIZE 256

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

int main() {
  int sockfd;
  char buf[BUF_SIZE], msg[] = "Hello!\0";
  struct sockaddr_ll server_addr;
  socklen_t server_addr_len = sizeof(server_addr);
  struct udphdr *header_udp;
  struct iphdr *header_ip;
  struct ethhdr *header_eth;
  unsigned index = 0;
  unsigned char mac_src[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, mac_dest[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (sockfd == -1) handle_error("socket");

  index = if_nametoindex("wlp4s0");
  if (index == 0) handle_error("if_nametoindex");
  server_addr.sll_family = AF_PACKET;
  server_addr.sll_ifindex = index;
  server_addr.sll_halen = 6;
  server_addr.sll_pkttype = 0;
  server_addr.sll_protocol = 0;
  memcpy(server_addr.sll_addr, mac_src, sizeof(unsigned char) * 6);

  memset(buf, '\0', BUF_SIZE);
  header_udp =
      (struct udphdr *)(buf + sizeof(struct iphdr) + sizeof(struct ethhdr));
  header_udp->dest = htons(PORT);
  header_udp->source = htons(CLIENT_PORT);
  header_udp->len = htons(sizeof(struct udphdr) + strlen(msg));
  header_udp->check = 0;

  header_ip = (struct iphdr *)(buf + sizeof(struct ethhdr));
  header_ip->version = 4;
  header_ip->ihl = 5;
  header_ip->tos = 0;
  header_ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg);
  header_ip->id = 0;
  header_ip->frag_off = 0;
  header_ip->ttl = 255;
  header_ip->protocol = IPPROTO_UDP;
  header_ip->check = 0;
  header_ip->saddr = inet_addr(IP_SRC);
  header_ip->daddr = inet_addr(IP_DEST);

  int check_sum = 0;
  short *ptr = (short *)header_ip;
  for (int i = 0; i < 10; i++) {
    check_sum += ptr[i];
  }
  check_sum += check_sum >> 16;
  check_sum = ~check_sum;
  header_ip->check = check_sum;


  header_eth = (struct ethhdr *)buf;
  header_eth->h_proto = IPPROTO_UDP;
  memcpy(header_eth->h_source, mac_src, sizeof(unsigned char) * 6);
  memcpy(header_eth->h_dest, mac_dest, sizeof(unsigned char) * 6);

  memmove(buf + sizeof(struct udphdr) + sizeof(struct iphdr) +
              sizeof(struct ethhdr),
          msg, strlen(msg));
  if (sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server_addr,
             server_addr_len) == -1)
    handle_error("sendto");

  memset(buf, '\0', BUF_SIZE);
  while (1) {
    if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server_addr,
                 &server_addr_len) == -1)
      handle_error("recvfrom");
    header_udp = (struct udphdr *)(buf + 20);
    if (header_udp->dest == htons(CLIENT_PORT)) break;
  }

  printf("message from server : %s\n",
         buf + sizeof(struct udphdr) + sizeof(struct iphdr));

  close(sockfd);

  exit(EXIT_SUCCESS);
}