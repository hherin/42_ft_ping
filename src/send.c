#include "../inc/ft_ping.h"

void sig_send_handler(int signum)
{
    icmp_sendto(g_icmp.sockfd, g_icmp.adinfo, g_icmp.pid);
    alarm(1);
    (void)signum;
}

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

    // printf("send => ");
    gettimeofday(g_icmp.stat.send_ms, NULL);
    g_icmp.stat.transmitted++;
    
    return ret;
}