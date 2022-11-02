#include "../inc/ft_ping.h"

void sig_send_handler(int signum)
{
    icmp_sendto(g_icmp.sockfd, g_icmp.adinfo, g_icmp.pid);
    alarm(1);
    (void)signum;
}

void add_back_rrtlist(int seq)
{
    t_send_list *tmp = g_icmp.lst.next;

    t_send_list *new;
    if (!(new = malloc(sizeof(t_send_list))))
        str_exit_error("ERROR OF MALLOC\n");
    
    new->seq = seq;
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
    // printf("SEND CKSUM %u\n", icmphdr.icmp_cksum);

    if ((ret = sendto(sockfd, &icmphdr, sizeof(icmphdr), 0, sa->ai_addr, sizeof(*(sa->ai_addr)))) < 0){
        fprintf(stderr, "error in sendto %s\n", strerror(errno));
        exit(1);
    }

    add_back_rrtlist(seq);
    g_icmp.stat.transmitted++;
    g_icmp.last_seq = seq;
    seq++;
    return ret;
}