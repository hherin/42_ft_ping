#include "../inc/ft_ping.h"

static t_send_list *get_seq_packet(UINT seq)
{
    t_send_list *tmp = g_icmp.lst.next;

    while (tmp)
    {
        if (tmp->seq == seq)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

t_send_list* set_rtt_time(UINT seq)
{
    struct timeval recv_ms;
    gettimeofday(&recv_ms, NULL);

    t_send_list *elem = get_seq_packet(seq);
    if (elem){
        elem->rtt_time = (float)(recv_ms.tv_usec - elem->sendtime.tv_usec) / 1000.0;
        
        if (elem->rtt_time < g_icmp.stat.min || g_icmp.stat.min == 0.0)
            g_icmp.stat.min = elem->rtt_time;
        if (elem->rtt_time > g_icmp.stat.max || g_icmp.stat.max == 0.0)
            g_icmp.stat.max = elem->rtt_time;
    }
    return elem;
}

float compute_average(void)
{
    t_send_list *tmp = g_icmp.lst.next;
    float sum = 0.0;
    int total = 0;
    while (tmp)
    {
        sum += tmp->rtt_time;
        total++;
        tmp = tmp->next;
    }
    return sum / total;
}

float compute_mdev(float avg)
{
    t_send_list *tmp = g_icmp.lst.next;
    float sum = 0.0;
    int total = 0;
    while (tmp)
    {
        printf("sum + avg + rtt = %.3f + %.3f - %.3f", sum, avg, tmp->rtt_time);
        sum += (avg - tmp->rtt_time) * (avg - tmp->rtt_time);
        printf(" = %.3f\n", sum);
        total++;
        tmp = tmp->next;
    }
    return (float)(sum / total);
}

void print_stats(void)
{
    struct timeval endtime;

    gettimeofday(&endtime, NULL);
    printf("\n--- %s ping statistics ---\n", g_icmp.srvname);
    
    long time_elapsed = (endtime.tv_sec * 1000 + endtime.tv_usec / 1000)- (g_icmp.stat.ping_start.tv_sec * 1000 + g_icmp.stat.ping_start.tv_usec / 1000);
    int loss = (int)((1 - ((float)g_icmp.stat.received / (float)g_icmp.stat.transmitted)) * 100);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", g_icmp.stat.transmitted, g_icmp.stat.received, loss, time_elapsed);
    if (!g_icmp.error) {
        float avg = compute_average();
        float mdev = compute_mdev(avg);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f\n", g_icmp.stat.min, avg, g_icmp.stat.max, mdev);
    }
}