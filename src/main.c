
#include "../inc/ft_ping.h"

int seq = 0;

USHORT CheckSum(USHORT *addr, int count)
{
    USHORT sum = 0;

    while( count > 1 )  {
        /*  This is the inner loop */
       sum += */** (unsigned short) */addr++;
       count -= 2;
    }

        /*  Add left-over byte, if any */
    if( count > 0 )
        sum += */*(unsigned char *)*/ addr;

        /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    // checksum = ~sum;
    return ~sum;
}

int main(int ac, char **av)
{
    if (ac != 2)
        str_exit_error("Destination address required\n");

    struct addrinfo *inet_info;

    if (getaddrinfo(av[1], "", NULL, &(inet_info)) ==  EAI_NONAME)
        str_exit_error("Name or service not known\n");

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    signal(SIGINT, ping_end_signal);

    long pid = getpid();
    
    UCHAR head[8];
    ft_bzero((char*)head, sizeof(head));
    // do {
        head[0] = 8U;
        head[1] = 0U;
        head[5] = pid;
        head[7] = seq++;
        USHORT cksum =  CheckSum((USHORT*)head, sizeof(head));
        printf("sum = %u - size %ld\n", cksum, sizeof(head));
        // (USHORT)*(head + 2) = cksum;
        printf("send the message\n");
        if (sendto(sockfd, head, sizeof(head), MSG_CONFIRM, inet_info->ai_addr, sizeof(inet_info->ai_addr)) < 0 )
            print("error in sendto\n");
        else
            printf("Wait for recvmsg...\n");
            
        struct msghdr msg;
        // char recvbuffer[256];
        // struct iovec iov[1];
        // iov[0].iov_base = recvbuffer;
        // iov[0].iov_len = sizeof(recvbuffer);
        // msg.msg_iov = iov;
        // msg.msg_iov_len = sizeof(iov);
        if (recvmsg(sockfd, &msg, 0) < 0)
            printf("error in recvmsg\n");
        else
            printf("Message received!\n");
    // } while (1);
    
    freeaddrinfo(inet_info);

    return 0;
}