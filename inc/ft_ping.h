#ifndef FT_PING__H
# define FT_PING__H

# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <string.h>
# include <netdb.h>
# include <arpa/inet.h>
# include "../lib/inc/libft.h"
# include <signal.h>
# include <errno.h>
# include <sys/time.h>
# include <netinet/ip_icmp.h>
# include <netinet/ip.h>
# include <stdbool.h>

# define UCHAR unsigned char
# define USHORT unsigned short
# define UINT unsigned int

# define TTL 64


struct packet 
{
    struct ip iphdr;
    struct icmp icmphdr;
};

typedef struct list_rtt
{
    UINT seq;
    struct timeval sendtime;
    float rtt_time;
    struct list_rtt *next;
}       t_send_list;

typedef struct 
{
    char *srvname;
    int sockfd;
    struct addrinfo *adinfo;
    pid_t pid;
    t_send_list lst;
    bool error;
    struct ip iphdr;
    struct icmp icmphdr;
    UINT last_seq;
    struct s_icmp_stat
    {
        USHORT transmitted;
        USHORT received;
        float min;
        float max;
        struct timeval ping_start;
        struct timeval send_ms;
    }   stat;

}       t_icmp_echo;


extern t_icmp_echo g_icmp;

void icmp_ping_loop(char *srcname);

void str_exit_error(char *str);
void ping_end_signal(int nb);
void sig_send_handler(int signum);
USHORT CheckSum(UCHAR *msg, int len);

t_send_list* set_rtt_time(UINT seq);
void print_stats(void);

int icmp_recvmsg(int sockfd);

int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid);


#endif