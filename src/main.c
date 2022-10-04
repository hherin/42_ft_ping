#include "../inc/ft_ping.h"


int main(int ac, char **av)
{
    if (ac != 2)
        str_exit_error("Destination address required\n");

    struct addrinfo *inet_info;
    int sockfd;
    int ttl = 64;
    long pid = getpid();
    signal(SIGINT, ping_end_signal);

    int retaddrinfo;
    struct addrinfo hints;
    ft_bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = 0;
    if ((retaddrinfo = getaddrinfo(av[1], "", &hints, &(inet_info))) !=  0){
        fprintf(stderr, "ping: %s: %s\n", av[1], gai_strerror(retaddrinfo));
        exit(1);
    }
    
    
    if ((sockfd = socket(AF_INET, SOCK_RAW, 1)) < 0) {
        fprintf(stderr, "Error socket: %s\n", gai_strerror(sockfd));
        exit(1);
    }
    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    
    icmp_ping_loop(sockfd, inet_info, pid, av[1]);
    
    freeaddrinfo(inet_info);
    close(sockfd);

    return 0;
}