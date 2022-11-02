#include "../inc/ft_ping.h"

void init_recv(struct packet *pck, struct msghdr *msg, struct iovec *iov, struct sockaddr_in *sin)
{
    ft_bzero(pck, sizeof(*pck));
    ft_bzero(iov, sizeof(iov));
    ft_bzero(msg, sizeof(*msg));

    iov[0].iov_base = pck;
    iov[0].iov_len = sizeof(*pck);

    msg->msg_name = sin;
    msg->msg_namelen = sizeof(*sin);
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
}


int icmp_recvmsg(int sockfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    struct packet pck;
    struct sockaddr_in sin;
    
    init_recv(&pck, &msg, iov, &sin);
    if (recvmsg(sockfd, &msg, MSG_DONTWAIT) < 0){
        if (errno == EAGAIN)
            return 0;
        fprintf(stderr, "Error in recvmsg: %s\n", strerror(errno));
        exit(1);
    }
    else {
        // long recv_time = get_time_from_start();
        struct timeval recv_ms;
        gettmeofday(recv_ms, NULL);
        printf("send sec=%d usec=%d\nrecv sec=%d usec=%d\n", \
        g_icmp.stat.send_ms.tv_sec, g_icmp.stat.send_ms.tv_usec, recv_ms.tv_sec, recv_ms.tv_usec);
        g_icmp.stat.received++;
        set_rtt_stat();
        char dst[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sin.sin_addr, dst, INET_ADDRSTRLEN);
        printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d\n", g_icmp.srvname, dst, pck.icmp_hdr.un.echo.sequence, pck.ip_hdr.ttl);
    }
    return 1;
}
