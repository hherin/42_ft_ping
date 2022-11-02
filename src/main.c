#include "../inc/ft_ping.h"

t_icmp_echo g_icmp = {
    .stat = {    
                .transmitted = 0,
                .received = 0,
                .min = 0.0,
                .max = 0.0,
                .ping_start.tv_sec = 0,
                .ping_start.tv_usec = 0 
    },
    .srvname = NULL,
    .sockfd = -1,
    .adinfo = NULL,
    .pid = -1,
    .lst = {
                .seq = 0,
                .next = NULL
    },
    .error = false,
    .last_seq = 0
};

void server_setup()
{
    int ttl = TTL;
    g_icmp.pid = getpid();
    
    
    int retaddrinfo;
    struct addrinfo hints;
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = 0;
    if ((retaddrinfo = getaddrinfo(g_icmp.srvname, "", &hints, &(g_icmp.adinfo))) !=  0){
        fprintf(stderr, "ping: %s: %s\n", g_icmp.srvname, gai_strerror(retaddrinfo));
        exit(1);
    }
    
    if ((g_icmp.sockfd = socket(AF_INET, SOCK_RAW, 1)) < 0) {
        fprintf(stderr, "Error socket: %s\n", strerror(errno));
        exit(1);
    }
    setsockopt(g_icmp.sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
}

int main(int ac, char **av)
{
    if (ac != 2)
        str_exit_error("Destination address required\n");

    signal(SIGINT, ping_end_signal);
    g_icmp.srvname = av[1];

    server_setup();
    
    icmp_ping_loop(av[1]);
    
    freeaddrinfo(g_icmp.adinfo);
    close(g_icmp.sockfd);

    return 0;
}

