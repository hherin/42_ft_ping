#include "../inc/ft_ping.h"

static struct msghdr init_recv(void)
{
    struct msghdr msg={};
    static struct ip iphdr;
    static struct icmp icmphdr;
    static struct sockaddr_in sin;
    static struct iovec iov[2];
    
    ft_bzero(iov, sizeof(iov));
    ft_bzero(&iphdr, sizeof(iphdr));
    ft_bzero(&icmphdr, sizeof(icmphdr));
    ft_bzero(&sin, sizeof(sin));

    iov[0].iov_base = &iphdr;
    iov[0].iov_len = sizeof(iphdr);
    iov[1].iov_base = &icmphdr;
    iov[1].iov_len = sizeof(icmphdr);

    msg.msg_name = &sin;
    msg.msg_namelen = sizeof(sin);
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;

    return msg;
}
    

static int process_received_packet(const struct iovec iov[2], const struct msghdr msg)
{
    struct ip *iphdr = iov[0].iov_base;
    struct icmp *icmphdr = iov[1].iov_base;
    
    char dst[INET_ADDRSTRLEN];
    struct sockaddr_in *sin = (struct sockaddr_in*)(msg.msg_name);
    inet_ntop(AF_INET, &sin->sin_addr, dst, INET_ADDRSTRLEN);
    
    switch (icmphdr->icmp_type) {

        case ICMP_ECHOREPLY:
            t_send_list *elem;
            
            if (icmphdr->icmp_id != g_icmp.pid || !(elem = set_rtt_time(icmphdr->icmp_seq)))
                return 0;
            
            int compare = icmphdr->icmp_cksum;
            icmphdr->icmp_cksum=0;
            if (compare != CheckSum((UCHAR*)icmphdr, sizeof(icmphdr)))
                return 0;
            
            if (sin->sin_addr.s_addr == LOCALHOST_INT_ADDR)
                printf("64 bytes from 127.0.0.1: icmp_seq=%d", icmphdr->icmp_seq);
            else
                printf("64 bytes from %s (%s): icmp_seq=%d", g_icmp.srvname, dst, icmphdr->icmp_seq);
            printf(" ttl=%d time=%.2f ms\n", iphdr->ip_ttl, elem->rtt_time);
            return 1;

        case ICMP_DEST_UNREACH:

            if (g_icmp.flags & VERB_FLG)
                printf("64 bytes from %s (%s): icmp_seq=%d Packet filtered : type %u code %u\n", g_icmp.srvname, dst, g_icmp.last_seq, icmphdr->icmp_type, icmphdr->icmp_code);
            else
                printf("64 bytes from %s (%s): icmp_seq=%d Packet filtered\n", g_icmp.srvname, dst, g_icmp.last_seq);
            g_icmp.error++;
            return 0;

        case ICMP_TIME_EXCEEDED:

            if (g_icmp.flags & VERB_FLG)
                printf("64 bytes from %s (%s): icmp_seq=%d Time to live exceeded : type %u code %u\n", g_icmp.srvname, dst, g_icmp.last_seq, icmphdr->icmp_type, icmphdr->icmp_code);
            else
                printf("64 bytes from %s (%s): icmp_seq=%d Time to live exceeded\n", g_icmp.srvname, dst, g_icmp.last_seq);
            g_icmp.error++;
            return 0;
        
        case ICMP_ECHO:
            return 0;
            
    }
    return 1;

}

int icmp_recvmsg(int sockfd)
{
    struct msghdr msg;

    msg = init_recv();

    if (recvmsg(sockfd, &msg, MSG_DONTWAIT) < 0){
        if (errno == EAGAIN)
            return 0;
        str_exit_error("internal error", strerror(errno), 1);
    }
    else {
        if (((struct ip*)(msg.msg_iov[0].iov_base))->ip_p == IPPROTO_ICMP)
            if (!process_received_packet(msg.msg_iov, msg))
                return 0;
        g_icmp.stat.received++;
    }
    return 1;
}
