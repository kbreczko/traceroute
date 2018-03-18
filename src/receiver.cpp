/*
 * Kamil Breczko
 * nr. indeksu: 280990
 */

#include "receiver.h"

Packet Receiver::receive_packet() {
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET + 1];

    /* otrzymany pakiet z gniazda kopiowany jest do bufora */
    ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *) &sender, &sender_len);
    auto receipt_time = std::chrono::high_resolution_clock::now();
    if (packet_len < 0) {
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
        return Packet(0, 0, "", receipt_time);
    }

    char sender_ip_str[20];
    const char *dst = inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    if (dst == NULL) {
        fprintf(stderr, "inet_ntop error: %s\n", strerror(errno));
        return Packet(0, 0, "", receipt_time);
    }

    struct iphdr *ip_header = (struct iphdr *) buffer;
    u_int8_t *icmp_packet = buffer + 4 * ip_header->ihl;
    struct icmphdr *icmp_header = (struct icmphdr *) icmp_packet;

    u_int16_t sequence = 0;
    u_int16_t id = 0;

    /* komunikat osiągnal swoje przeznaczenie */
    if (icmp_header->type == ICMP_ECHOREPLY) {
        sequence = icmp_header->un.echo.sequence;
        id = icmp_header->un.echo.id;
    }

        /* pole TTL osiagnelo wartosc 0 */
    else if (icmp_header->type == ICMP_TIME_EXCEEDED) {
        icmp *sent_icmp_header = get_sent_icmp_header(icmp_packet);

        sequence = sent_icmp_header->icmp_seq;
        id = sent_icmp_header->icmp_id;
    }

    return Packet(id, sequence, sender_ip_str, receipt_time);
}


/* wyciagniecie wyslanego przez nas pakietu z naglowka ICMP */
icmp *Receiver::get_sent_icmp_header(u_int8_t *icmp_packet) const {
    u_int8_t *sent_ip_packet = icmp_packet + 8;
    struct ip *sent_ip_header = (struct ip *) sent_ip_packet;
    u_int8_t *sent_icmp_packet = sent_ip_packet + sent_ip_header->ip_hl * 4;
    struct icmp *sent_icmp_header = (struct icmp *) sent_icmp_packet;
    return sent_icmp_header;
}

Receiver::Receiver(int sockfd) : sockfd(sockfd) {}