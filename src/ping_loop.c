#include "../inc/ft_ping.h"

int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid)
{
    struct icmphdr icmp_hdr;
    static int seq = 0;
    int ret;
    
    ft_bzero((UCHAR*)(&icmp_hdr), sizeof(icmp_hdr));
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.code = 0U;
    icmp_hdr.un.echo.id = pid;
    icmp_hdr.un.echo.sequence = seq++;
   
    icmp_hdr.checksum = CheckSum((UCHAR*)(&icmp_hdr), sizeof(icmp_hdr));

    if ((ret = sendto(sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, sa->ai_addr, sizeof(*(sa->ai_addr)))) < 0){
        fprintf(stderr, "error in sendto %s\n", strerror(errno));
        exit(1);
    }

    printf("send => ");
    icmp.stat.send_ms = set_time_from_start();
    icmp.stat.transmitted++;
    
    return ret;
}

void icmp_ping_loop(int sockfd, struct addrinfo *sa, pid_t pid, char *srcname)
{
    gettimeofday(&icmp.stat.ping_start, NULL);
    char dst[INET_ADDRSTRLEN];
    struct sockaddr_in *sin = (struct sockaddr_in*)sa->ai_addr;
    printf("PING %s (%s) 56(84) bytes of data\n", srcname, inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN));
    do {
        icmp_sendto(sockfd, sa, pid);
        my_sleep(1);
        icmp_recvmsg(sockfd);       
    } while (1);
}