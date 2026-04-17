// Tomasz Żyła 337479
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <errno.h>
#include <assert.h>
#include <poll.h>
#include <sys/time.h>
#include <stdbool.h>

void ERROR(const char* str)
{
    fprintf(stderr, "%s: %s\n", str, strerror(errno));  // NOLINT(*-err33-c)
    exit(EXIT_FAILURE);
}

u_int16_t compute_icmp_checksum(const void *buff, int length)
{
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return ~(sum + (sum >> 16U));
}

void send_packets(int sockfd, struct sockaddr_in* recipient, int ttl, int pid, int seq) {
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) < 0)
        ERROR("setsockopt error");

    struct icmp header = {0};
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_id = pid;
    header.icmp_seq = seq;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((uint16_t*)&header, sizeof(header));

    ssize_t bytes_sent = sendto(
        sockfd,
        &header,
        sizeof(header),
        0,
        (struct sockaddr*)recipient,
        sizeof(*recipient)
    );

    if (bytes_sent < 0)
        ERROR("sendto error");
}

bool receive_packets(int sockfd, int ttl, int pid, int seq) {
    struct pollfd ps;
    ps.fd = sockfd;
    ps.events = POLLIN;

    int packets_received = 0;
    long time_total = 0;

    char routers_ips[3][20];
    int unique_routers = 0;
    bool destination = false;

    struct timeval st_time, cur_time;
    const int time_limit = 1000;
    gettimeofday(&st_time, NULL);


    while (packets_received < 3) {
        gettimeofday(&cur_time, NULL);
        long time_elapsed = (cur_time.tv_sec - st_time.tv_sec) * 1000 + (cur_time.tv_usec - st_time.tv_usec) / 1000;
        long time_left_ms = time_limit - time_elapsed;

        if (time_left_ms < 0)
            break;

        int ready = poll(&ps, 1, time_left_ms);
        if (ready < 0)
            ERROR("poll error");
        if (ready == 0)
            break;

        struct sockaddr_in sender;
        socklen_t sender_len = sizeof(sender);
        uint8_t buffer[IP_MAXPACKET];

        ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
        if (packet_len < 0)
            ERROR("recvfrom error");

        gettimeofday(&cur_time, NULL);
        long travel_time = (cur_time.tv_sec - st_time.tv_sec) * 1000 + (cur_time.tv_usec - st_time.tv_usec) / 1000;

        struct ip* ip_header = (struct ip*) buffer;
        ssize_t ip_header_len = 4 * ip_header->ip_hl;
        struct icmp* icmp_header = (struct icmp*) (buffer + ip_header_len);

        bool valid_packet = false;

        if (icmp_header->icmp_type == ICMP_ECHOREPLY) {
            if (icmp_header->icmp_id == pid && icmp_header->icmp_seq >= seq && icmp_header->icmp_seq < seq + 3) {
                valid_packet = true;
                destination = true;
            }
        } else if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED) {
            struct ip* ip_header_org = (struct ip*) ((uint8_t*)icmp_header + 8);
            ssize_t ip_header_org_len = 4 * ip_header_org->ip_hl;
            struct icmp* icmp_header_org = (struct icmp*) ((uint8_t*)ip_header_org + ip_header_org_len);

            if (icmp_header_org->icmp_id == pid && icmp_header_org->icmp_seq >= seq && icmp_header_org->icmp_seq < seq + 3)
                valid_packet = true;
        }

        if (valid_packet) {
            packets_received ++;
            time_total += travel_time;

            char ip[20];
            inet_ntop(AF_INET, &(sender.sin_addr), ip, sizeof(ip));

            bool new = true;
            for (int i = 0; i < unique_routers; i ++)
                if (strcmp(routers_ips[i], ip) == 0)
                    new = false;

            if (new) {
                strcpy(routers_ips[unique_routers], ip);
                unique_routers ++;
            }
        }
    }

    printf("%2d. ", ttl);
    
    if (packets_received == 0) {
        printf("*\n");
    } else {
        for (int i = 0; i < unique_routers; i++)
            printf("%s ", routers_ips[i]);
        
        if (packets_received == 3)
            printf("%ldms\n", time_total / 3);
        else
            printf("???\n");
    }

    return destination;
}

int main(int argc, char *argv[]) {
    if (argc != 2)
        ERROR("argument error");

    struct sockaddr_in recipient = {0};
    recipient.sin_family = AF_INET;

    if (inet_pton(AF_INET, argv[1], &recipient.sin_addr) != 1)
        ERROR("invalid destination IP address error");

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
        ERROR("socket error");

    int pid = getpid() & 0xFFFF;

    for (int ttl = 1; ttl <= 30; ttl++) {
        int seq = 1 + (ttl - 1) * 3;
        
        for (int i = 0; i < 3; i ++)
            send_packets(sockfd, &recipient, ttl, pid, seq + i);

        bool finished = receive_packets(sockfd, ttl, pid, seq);

        if (finished)
            break;
    }

    return 0;
}