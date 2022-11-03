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

static float compute_average(void)
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

static float my_sqrt(const float x)
{
    union 
    {
        int i;
        float x;
    } u;
    u.x = x;
    u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
    u.x = u.x + x / u.x;
    u.x = 0.25f * u.x + x / u.x;

    return u.x;
}

float compute_mdev(float avg)
{
    t_send_list *tmp = g_icmp.lst.next;
    float sum = 0.0;
    int total = 0;
    while (tmp)
    {
        sum += (avg - tmp->rtt_time) * (avg - tmp->rtt_time);
        total++;
        tmp = tmp->next;
    }
    return my_sqrt(sum / total);
}

void print_stats(void)
{
    struct timeval endtime;

    gettimeofday(&endtime, NULL);
    printf("\n--- %s ping statistics ---\n", g_icmp.srvname);
    
    long time_elapsed = (endtime.tv_sec * 1000 + endtime.tv_usec / 1000)- (g_icmp.stat.ping_start.tv_sec * 1000 + g_icmp.stat.ping_start.tv_usec / 1000);
    int loss = (int)((1 - ((float)g_icmp.stat.received / (float)g_icmp.stat.transmitted)) * 100);
    printf("%d packets transmitted, %d received, ", g_icmp.stat.transmitted, g_icmp.stat.received);
    if (!g_icmp.error) {
        printf("%d%% packet loss, time %ldms\n", loss, time_elapsed);
        float avg = compute_average();
        float mdev = compute_mdev(avg);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f\n", g_icmp.stat.min, avg, g_icmp.stat.max, mdev);
    }
    else {
        printf("+%d errors, %d%% packet loss, time %ldms\n\n", g_icmp.error, loss, time_elapsed);
    }
}