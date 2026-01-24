#include "ft_ping.h"



uint16_t icmp_checksum(const void *data, size_t len) {
    // reference: https://en.wikipedia.org/wiki/Internet_checksum
    const uint16_t *p = data;
    uint32_t sum = 0;

    while (len > 1) {
        sum += *p;
        p++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(const uint8_t *)p;
    }

    /* fold 32-bit sum to 16 bits */
    sum = (sum >> 16) + (sum & 0xFFFF); 
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

void build_echo_request(uint8_t *buf, size_t payload_len,
                        uint16_t ident, uint16_t seq) {
    struct icmphdr *hdr;
    uint8_t *payload;

    hdr = (struct icmphdr *)buf; // cast the buf to header casting the pointers
    payload = buf + sizeof(struct icmphdr); //find the position of the payload

    hdr->type = ICMP_ECHO;   /* 8 */
    hdr->code = 0; //echo request code is always 0
    hdr->un.echo.id = htons(ident); // set identifier
    hdr->un.echo.sequence = htons(seq);
    hdr->checksum = 0;

    memset(payload, 0xAA, payload_len); // fill payload 

    /* checksum over header + payload */
    hdr->checksum = icmp_checksum(buf, sizeof(struct icmphdr) + payload_len);
}

void build_request(uint8_t *buf){
    unsigned int identifier;
    static unsigned int sequential;

    identifier = getpid();

    build_echo_request(buf,PAYLOAD_LENGTH,identifier,sequential);
    sequential++;
}