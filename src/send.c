#include "../inc/ft_ping.h"

static void add_back_rrtlist(int seq, int cksum)
{
    t_send_list *tmp = g_icmp.lst.next;

    t_send_list *new;
    if (!(new = malloc(sizeof(t_send_list))))
        str_exit_error("internal error", "malloc failed\n", 1);
    
    new->seq = seq;
    new->cksum = cksum;
    gettimeofday(&new->sendtime, NULL);
    new->next = NULL;

    if (!tmp) {
        g_icmp.lst.next = new;
    }
    else {
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid)
{
    struct icmp icmphdr;
    static int seq = 0;
    int ret;
    
    ft_bzero((UCHAR*)(&icmphdr), sizeof(icmphdr));
    icmphdr.icmp_type = ICMP_ECHO;
    icmphdr.icmp_code = 0U;
    icmphdr.icmp_id = pid;
    icmphdr.icmp_seq = seq;
   
    icmphdr.icmp_cksum = CheckSum((UCHAR*)(&icmphdr), sizeof(icmphdr));

    if ((ret = sendto(sockfd, &icmphdr, sizeof(icmphdr), 0, sa->ai_addr, sizeof(*(sa->ai_addr)))) < 0)
        str_exit_error("internal error", "sendto failed", 1);

    add_back_rrtlist(seq, icmphdr.icmp_cksum);
    g_icmp.stat.transmitted++;
    g_icmp.last_seq = seq;
    seq++;
    return ret;
}