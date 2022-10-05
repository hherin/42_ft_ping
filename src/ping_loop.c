#include "../inc/ft_ping.h"


int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid)
{
    UCHAR icmp_hdr[8];
    static int seq = 0;
    int ret;
    
    ft_bzero((char*)icmp_hdr, sizeof(icmp_hdr));
    icmp_hdr[0] = 8U;
    icmp_hdr[1] = 0U;
    icmp_hdr[5] = pid;
    icmp_hdr[7] = seq++;
    ((USHORT*)icmp_hdr)[1] = CheckSum(icmp_hdr, sizeof(icmp_hdr));

    if ((ret = sendto(sockfd, icmp_hdr, sizeof(icmp_hdr), 0, sa->ai_addr, sizeof(*(sa->ai_addr)))) < 0){
        fprintf(stderr, "error in sendto %s\n", strerror(errno));
        exit(1);
    } 
    icmp.stat.transmitted++;
    
    return ret;
}

void icmp_ping_loop(int sockfd, struct addrinfo *sa, pid_t pid, char *srcname)
{
    int n = 5;
    
    gettimeofday(&icmp.stat.time, NULL);
    if (icmp_sendto(sockfd, sa, pid) >= 0) {
        char dst[INET_ADDRSTRLEN];
        struct sockaddr_in *sin = (struct sockaddr_in*)sa->ai_addr;
        printf("PING %s (%s) 56(84) bytes of data\n", srcname, inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN));
    }
    do {

        icmp_recvmsg(sockfd);
        
        my_sleep(1);
        icmp_sendto(sockfd, sa, pid);
        n--;

    } while (1);
}