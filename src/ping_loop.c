#include "../inc/ft_ping.h"

static void sig_send_handler(int signum)
{
    icmp_sendto(g_icmp.sockfd, g_icmp.adinfo, g_icmp.pid);
    alarm(1);
    (void)signum;
}


void icmp_ping_loop(char *srcname)
{
    signal(SIGALRM, sig_send_handler);
    
    gettimeofday(&g_icmp.stat.ping_start, NULL);
    
    char dst[INET_ADDRSTRLEN];
    struct sockaddr_in *sin = (struct sockaddr_in*)g_icmp.adinfo->ai_addr;

    printf("PING %s (%s) 56(84) bytes of data.\n", srcname, inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN));
    
    alarm(1);
    do {
        icmp_recvmsg(g_icmp.sockfd);
    } while (1);
}