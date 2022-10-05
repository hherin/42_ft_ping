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

# define UCHAR unsigned char
# define USHORT unsigned short
# define UINT unsigned int

# define TTL 1

typedef struct s_icmp_stat
{
    USHORT transmitted;
    USHORT received;
    struct timeval time;
}   t_icmp_stat;

typedef struct 
{
    t_icmp_stat stat;
    char *srvname;
    int sockfd;
    struct addrinfo *adinfo;
    pid_t pid;
}       t_icmp_echo;


extern t_icmp_echo icmp;

void str_exit_error(char *str);
void ping_end_signal(int nb);
void my_sleep(int nb);
USHORT CheckSum(UCHAR *msg, int len);

void icmp_recvmsg(int sockfd);

void icmp_ping_loop(int sockfd, struct addrinfo *sa, pid_t pid, char *srcname);
int icmp_sendto(int sockfd, struct addrinfo *sa, pid_t pid);

#endif