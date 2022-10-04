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

# define UCHAR unsigned char
# define USHORT unsigned short
# define UINT unsigned int


typedef struct s_icmp_header
{
    UCHAR  type;
    UCHAR  code;
    USHORT somme;
    USHORT id;
    USHORT seq_nu;
}   icmpping_header;

void str_exit_error(char *str);
void ping_end_signal(int nb);
USHORT CheckSum(UCHAR *msg, int len);

void icmp_ping_loop(int sockfd, struct addrinfo *sa, pid_t pid, char *srcname);

#endif