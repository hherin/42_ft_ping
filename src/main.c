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
    .error = 0,
    .last_seq = 0,
    .flags=0U
};

static void server_setup(void)
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
    if ((retaddrinfo = getaddrinfo(g_icmp.srvname, "", &hints, &(g_icmp.adinfo))) !=  0)
        str_exit_error(g_icmp.srvname, gai_strerror(retaddrinfo), 2);
    
    if ((g_icmp.sockfd = socket(AF_INET, SOCK_RAW, 1)) < 0)
        str_exit_error("internal error", strerror(errno), 1);
    
    setsockopt(g_icmp.sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
}

static void print_help(void)
{
    printf("\nUsage\n\tping [options] <destination>\n\n");
    printf("Options:\n \
    \t<destination>\tdns name r ip address\n \
    -h\t\tprint help and exit\n \
    -v\t\tverbose output\n");
}

static void parse_cmd(char **av)
{
    for (int i = 0; av[i]; i++) {
        if (av[i][0] != '-'){
            if (g_icmp.srvname)
                str_exit_error("usage error", "one destination allowed, see help [-h]", 1);
            g_icmp.srvname = av[i];
        }
        else {
            for (int j = 1; av[i][j]; j++) {

                switch (av[i][j]) {
                    case 'h': 
                        g_icmp.flags |= HLP_FLG;
                        break;
                    case 'v':
                        g_icmp.flags |= VERB_FLG;
                        break;

                    default:
                        print_help();
                        exit(2);
                }

            }

        }
    }
}


int main(int ac, char **av)
{
    (void)ac;
    signal(SIGINT, ping_end_signal);

    parse_cmd(&av[1]);
    
    if (!g_icmp.srvname)
        str_exit_error("usage error", "Destination address required", 1);
    
    if (g_icmp.flags & HLP_FLG) {
        print_help();
        return 0;
    }
    server_setup();
    
    icmp_ping_loop();
    
    freeaddrinfo(g_icmp.adinfo);
    close(g_icmp.sockfd);

    return 0;
}

