#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <pcap.h>


static char gerrbuf[PCAP_ERRBUF_SIZE];

char* w_pcap_lookupdev(char **errbuf) {
    *errbuf = gerrbuf;
    pcap_lookupdev(gerrbuf);
}

void* w_pcap_open(char *dev, char *errbuf) {
    return (void*) pcap_open_live(dev, BUFSIZ, 1,-1, errbuf);
}

int w_pcap_next_ex(void *pcap, unsigned char **pkt_data) {
    struct pcap_pkthdr *header;
    int ret = pcap_next_ex(pcap, &header, pkt_data);
    if (ret < 0) {
        return ret;
    }
    return header->len;
}

void pcap_send(void *pcap, void *src, uint32_t size) {
    pcap_sendpacket(pcap, (uint8_t *) src, size);
}
