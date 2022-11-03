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

# define UCHAR unsigned char
# define USHORT unsigned short
# define UINT unsigned int
# define LOCALHOST_INT_ADDR 16777343
# define TTL 64

# define HLP_FLG    1U << 0 
# define VERB_FLG   1U << 1
# define TTL_FLG    1U << 2



typedef struct list_rtt
{
    UINT seq;
    struct timeval sendtime;
    float rtt_time;
    int cksum;
    struct list_rtt *next;
}       t_send_list;

typedef struct 
{
    char *srvname;
    int sockfd;
    struct addrinfo *adinfo;
    pid_t pid;
    t_send_list lst;
    int error;
    struct ip iphdr;
    struct icmp icmphdr;
    UINT last_seq;
    unsigned int flags;
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

void icmp_ping_loop();

void str_exit_error(const char *s1, const char *s2, int exitcode);
void ping_end_signal(int nb);
void free_everything(void);
USHORT CheckSum(UCHAR *msg, int len);

t_send_list* set_rtt_time(UINT seq);
void print_stats(void);

int icmp_recvmsg(int sockfd);

int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid);


#endif