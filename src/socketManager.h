#include "../include/socketManager.h"

int rawsocket_send(packet_t packet){
    //you need root btw
    int sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1){
        perror("ERROR opening raw socket. Do you have root priviliges?");
        return -1;
    }
    struct sockaddr_in sock_in;
    sock_in.sin_addr.s_addr = packet.ipheader->daddr;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = packet.tcpheader->dest;

    int one = 1;
    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one))<0){
        perror("ERROR setting desired flags in socket options");
        return -1;
    }

    int sent = sendto(sock, packet.packet, packet.ipheader->tot_len, 0, (struct sockaddr*)&sock_in, sizeof(sock_in));
    if(sent<0){
        perror("ERROR sending the packet in the socket");
        return -1;
    }

    printf("Packet of length %d sent to %u\n", packet.ipheader->tot_len, packet.ipheader->daddr);

    close(sock);
    return 0;
}


packet_t rawsocket_sniff(){
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    packet_t packet;

    if(sock == -1){
        perror("ERROR opening raw socket. Do you have root priviliges?");
        packet = build_null_packet(packet);
        return packet;
    }

    int buffer_size = 20000;
    char* buffer = calloc(buffer_size, sizeof(char));
    int received = recvfrom(sock, buffer, buffer_size, 0x0, NULL, NULL);

    

    if(received<0){
        perror("ERROR receiving packet in the socket");
        packet = build_null_packet(packet);
        return packet;
    }

    packet = parse_packet(buffer, buffer_size);

    close(sock);
    return packet;
}

packet_t rawsocket_sniff_pattern(char* payload_pattern){
    int pattern_received = 0;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    packet_t packet;

    while(!pattern_received){
        if(sock == -1){
            perror("ERROR opening raw socket. Do you have root priviliges?");
            packet = build_null_packet(packet);
            return packet;
        }

        int buffer_size = 20000;
        char* buffer = calloc(buffer_size, sizeof(char));
        int received = recvfrom(sock, buffer, buffer_size, 0x0, NULL, NULL);

        if(received<0){
            perror("ERROR receiving packet in the socket");
            packet = build_null_packet(packet);
            return packet;
        }

        packet = parse_packet(buffer, buffer_size);

        if(strncmp(packet.payload, payload_pattern, strlen(payload_pattern)) == 0){
            printf("Found the packet with the pattern %s\n", payload_pattern);
            pattern_received = 1;
        }else{
            printf("Found payload string was %s\n", packet.payload);
        }

    }
    close(sock);
    return packet;
}

