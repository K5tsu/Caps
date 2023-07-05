#include "../include/packetInterpreter.h"

int get_packet_proto(char* buffer, int size){
    struct iphdr *ipheader = (struct iphdr*)buffer;

    int protocol = ipheader->protocol;

    printf("Packet of protocol %i detected\n", protocol);
    return protocol;
}

packet_t parse_packet(char* buffer, int size){
    int proto = get_packet_proto(buffer, size);
    packet_t packet;

    if(proto!=6){
        build_null_packet(packet);
        fprintf(stderr, "Parsed packet of non-supported protocol. This should not have happened %i\n", proto);
        return packet;
    }

    packet.ipheader = (struct iphdr*) buffer;
    int ip_header_length = packet.ipheader->ihl*4;

    packet.tcpheader = (struct tcphdr*) (buffer+ip_header_length);
    int tcp_header_length = packet.tcpheader->doff*4;

    packet.payload = (char*) buffer+ip_header_length+tcp_header_length;

    packet.payload_length = size - ip_header_length - tcp_header_length;

    packet.packet = buffer;

    return packet;
}
