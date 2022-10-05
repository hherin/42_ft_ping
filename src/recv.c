#include "../inc/ft_ping.h"



void icmp_recvmsg(int sockfd)
{
    struct cmsghdr control;
    struct msghdr msg;
    struct iovec iov[1];
    char msg_buffer[64];
    struct sockaddr_in sin;

    ft_bzero(&msg_buffer, sizeof(msg_buffer));
    ft_bzero(&control, sizeof(control));
    ft_bzero(iov, sizeof(iov));
    ft_bzero(&msg, sizeof(msg));

    iov[0].iov_base = msg_buffer;
    iov[0].iov_len = sizeof(msg_buffer);

    msg.msg_name = &sin;
    msg.msg_namelen = sizeof(sin);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_control = &control;
    msg.msg_controllen = sizeof(control);
    int recv;
    if ((recv = recvmsg(sockfd, &msg, MSG_DONTWAIT)) < 0){
        if (errno == EAGAIN)
            return;
        fprintf(stderr, "Error in recvmsg: %s\n", strerror(errno));
        exit(1);
    }
    else {
        char dst[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &sin.sin_addr, dst, INET_ADDRSTRLEN);
        if (sin.sin_addr.s_addr != ((struct sockaddr_in*)(icmp.adinfo->ai_addr))->sin_addr.s_addr)
            printf("gateway ttl\n");
        
        printf("%ld bytes from (%s)\n", (msg.msg_iov)->iov_len, dst);
        icmp.stat.received++;
    }
    
}
