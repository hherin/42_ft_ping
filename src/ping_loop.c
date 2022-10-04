#include "../inc/ft_ping.h"

void icmp_recvmsg(int sockfd )
{
    struct cmsghdr control_buffer;
    struct msghdr msghdr;
    struct iovec iov[1];
    char msg_buffer[64];
    struct sockaddr_in sin;

    ft_bzero(&msg_buffer, sizeof(msg_buffer));
    ft_bzero(&control_buffer, sizeof(control_buffer));
    ft_bzero(iov, sizeof(iov));
    ft_bzero(&msghdr, sizeof(msghdr));

    iov[0].iov_base = msg_buffer;
    iov[0].iov_len = sizeof(msg_buffer);

    msghdr.msg_name = &sin;
    msghdr.msg_namelen = sizeof(sin);
    msghdr.msg_iov = iov;
    msghdr.msg_iovlen = 1;
    if (recvmsg(sockfd, &msghdr, 0) < 0){
        fprintf(stderr, "Error in recvmsg: %s\n", strerror(errno));
        exit(1);
    }
    else{
        char dst[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &sin.sin_addr, dst, INET_ADDRSTRLEN);
        printf("%ld bytes from (%s)\n", (msghdr.msg_iov)->iov_len, dst);
    }
}

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
    
    return ret;
}

void icmp_ping_loop(int sockfd, struct addrinfo *sa, pid_t pid, char *srcname)
{
    int n = 5;

    if (icmp_sendto(sockfd, sa, pid) >= 0) {
        char dst[INET_ADDRSTRLEN];
        struct sockaddr_in *sin = (struct sockaddr_in*)sa->ai_addr;
        printf("PING %s (%s) 56(84) bytes of data\n", srcname, inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN));
    }
    do {

        icmp_recvmsg(sockfd);
        
        icmp_sendto(sockfd, sa, pid);
        n--;

    } while (1 && n);
}