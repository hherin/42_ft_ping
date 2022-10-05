#include "../inc/ft_ping.h"

t_icmp_echo icmp = {
    .stat = {    .transmitted = 0,
                .received = 0,
                .time.tv_sec = 0,
                .time.tv_usec = 0 },
    .srvname = NULL,
    .sockfd = -1,
    .adinfo = NULL,
    .pid = -1
};

void server_setup()
{
    int ttl = TTL;
    icmp.pid = getpid();
    
    
    int retaddrinfo;
    struct addrinfo hints;
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = 0;
    if ((retaddrinfo = getaddrinfo(icmp.srvname, "", &hints, &(icmp.adinfo))) !=  0){
        fprintf(stderr, "ping: %s: %s\n", icmp.srvname, gai_strerror(retaddrinfo));
        exit(1);
    }
    
    if ((icmp.sockfd = socket(AF_INET, SOCK_RAW, 1)) < 0) {
        fprintf(stderr, "Error socket: %s\n", strerror(errno));
        exit(1);
    }
    setsockopt(icmp.sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
}

int main(int ac, char **av)
{
    if (ac != 2)
        str_exit_error("Destination address required\n");

    signal(SIGINT, ping_end_signal);
    icmp.srvname = av[1];

    server_setup();
    
    icmp_ping_loop(icmp.sockfd, icmp.adinfo, icmp.pid, av[1]);
    
    freeaddrinfo(icmp.adinfo);
    close(icmp.sockfd);

    return 0;
}