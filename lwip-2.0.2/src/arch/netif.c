/*
 * netif.c
 *
 *  Created on: Jul 18, 2017
 *      Author: haohd
 *
 *  Copyright (C) 2017 miniHome
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <pcap.h>
#include "lwip.h"

#define NET_DEBUG_PRINTF              printf
#define NETIF_SET_NAME(netif,c1,c2)   do { (netif)->name[0] = c1; (netif)->name[1] = c2; } while (0)
#define NET_IF_IP_LEN                 16
#define LOCALHOST_IP_ADDR             ((1 << 24) | (0 << 16) | (0 << 8) | (127)) /* "127.0.0.1" */

static err_t linux_lwip_init(struct netif *netif);
static err_t linux_link_output(struct netif *netif, struct pbuf *pfirst);
static u32_t get_default_getway_ip(void);
static void* netif_packet_capture(void *arg);

#if LWIP_NETIF_STATUS_CALLBACK
static void linux_net_status_cb(struct netif *netif);
#endif
#if LWIP_IGMP
err_t linux_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group,  u8_t action);
#endif


static struct netif my_netif;
static pcap_t *gppcap = NULL;
static u16_t available_ports[LWIP_LINUX_PORT_NUM*2];

struct netif* get_netif(void)
{
	return &my_netif;
}

int lwip_linux_check_port(u16_t port)
{
  int i;
	for (i = 0; i < LWIP_LINUX_PORT_NUM*2; i++)
	{
		if (port == available_ports[i])
		{
			return 1;
		}
	}
	/* port is not configured for LWIP */
	return 0;
}

/** Returns the current time in milliseconds,
 * may be the same as sys_jiffies or at least based on it. */
u32_t sys_now(void)
{
	static long long prev_time_in_mill = 0;
	long long time_in_mill;
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	time_in_mill = (long long) (tv.tv_sec) * 1000 + (long long) ((tv.tv_usec) / 1000) ;
  if (prev_time_in_mill == 0)
  {
    prev_time_in_mill = time_in_mill;
  }

  u32_t mill = (u32_t) (time_in_mill - prev_time_in_mill);
  return mill;
}

err_t net_init(char *ifname)
{
  char cmd[256];
  char errbuf[PCAP_ERRBUF_SIZE];
  u8_t mac_addr[6];
  char *dev;
  ip_addr_t ip = {0};
  ip_addr_t gw = {0};
  ip_addr_t mask = {0};
  u32_t ip_addr = 0, mask_addr = 0;
  int i;

  if (ifname == NULL)
  {
_netdev_try:
    dev = pcap_lookupdev(errbuf);
    if(dev == NULL)
    {
      printf("PCAP locckup device error: %s\n", errbuf);
      return ERR_IF;
    }
    printf("Found ethif: %s\n", dev);

    /* Get the network address and mask */
    if (get_if_address(dev, &ip_addr, &mask_addr, mac_addr) < 0)
    {
      return ERR_IF;
    }
    if (ip_addr == LOCALHOST_IP_ADDR)
    {
      goto _netdev_try;
    }
  }
  else
  {
    /* Get the network address and mask */
    dev = ifname;
    if (get_if_address(dev, &ip_addr, &mask_addr, mac_addr) < 0)
    {
      return ERR_IF;
    }
    if (ip_addr == LOCALHOST_IP_ADDR)
    {
      return ERR_IF;
    }
  }

	ip.addr = ip_addr;
	gw.addr = get_default_getway_ip();
	mask.addr = mask_addr;

	if (gw.addr == 0)
	{
	  return ERR_IF;
	}

	/* Disable firewall */
	system("sudo ufw disable");
	for (i = 0; i < LWIP_LINUX_PORT_NUM*2; i++)
	{
		/* Drop packets on ports */
	  if (i < LWIP_LINUX_PORT_NUM)
	  {
	    available_ports[i] = LWIP_LINUX_SERVER_START_PORT_NUM + i;
	  }
	  else
	  {
	    available_ports[i] = LWIP_LINUX_CLIENT_START_PORT_NUM + (i - LWIP_LINUX_PORT_NUM);
	  }
		sprintf(cmd, "sudo iptables -A INPUT -p tcp --destination-port %u -j DROP", available_ports[i]);
		system(cmd);
	}

	// Read MAC address from pre-programmed area of EEPROM.
	my_netif.hwaddr_len = 6;
	memcpy(&my_netif.hwaddr[0], mac_addr, 6);

	NET_DEBUG_PRINTF("MAC : %02x:%02x:%02x:%02x:%02x:%02x\n",
			my_netif.hwaddr[0], my_netif.hwaddr[1], my_netif.hwaddr[2],
			my_netif.hwaddr[3], my_netif.hwaddr[4], my_netif.hwaddr[5]);

	NET_DEBUG_PRINTF("IP  : %u.%u.%u.%u\n",
			(ip.addr) & 0xFF, (ip.addr >> 8) & 0xFF,
			(ip.addr >> 16) & 0xFF, (ip.addr >> 24) & 0xFF);

	NET_DEBUG_PRINTF("GW  : %u.%u.%u.%u\n",
				(gw.addr) & 0xFF, (gw.addr >> 8) & 0xFF,
				(gw.addr >> 16) & 0xFF, (gw.addr >> 24) & 0xFF);

	NET_DEBUG_PRINTF("MASK: %u.%u.%u.%u\n",
				(mask.addr) & 0xFF, (mask.addr >> 8) & 0xFF,
				(mask.addr >> 16) & 0xFF, (mask.addr >> 24) & 0xFF);

	NETIF_SET_NAME(&(my_netif), ' ', '1');

	my_netif.next = NULL;

	// Initialize LWIP
	lwip_init();

	// Add our netif to LWIP (netif_add calls our driver initialization function)
	if (netif_add(&my_netif,
			&ip,
			&mask,
			&gw,
			NULL,
			linux_lwip_init,
			ethernet_input) == NULL)
	{
		NET_DEBUG_PRINTF("netif_add failed\n");
		return ERR_IF;
	}

	netif_set_default(&my_netif);
	netif_set_link_down(&my_netif);
	netif_set_down(&my_netif);
#if LWIP_NETIF_STATUS_CALLBACK
	netif_set_status_callback(&my_netif, linux_net_status_cb);
#endif

	netif_set_up(&my_netif);

#if 0
	/* Use static IP */
	if (dhcp)
	{
		net_set_dhcp(1);
	}
#endif

	NET_DEBUG_PRINTF("Initialized the lwip.\n");

#if LWIP_IGMP
  igmp_start(&my_netif);
#endif

	/* open device for reading in promiscuous mode */
	gppcap = pcap_open_live(dev, BUFSIZ, 1,-1, errbuf);
	if(gppcap == NULL)
	{
		printf("pcap_open_live(): %s\n", errbuf);
		return ERR_IF;
	}

	netif_set_link_up(&my_netif);
	printf("Lwip IF is up now!\n");

	return ERR_OK;
}

void net_quit(void)
{
	char cmd[256];
	int i;

	/* Enable firewall */
	//system("sudo ufw enable");
	for (i = 0; i < LWIP_LINUX_PORT_NUM*2; i++)
	{
		/* Accept packets on ports */
		sprintf(cmd, "sudo iptables -A INPUT -p tcp --destination-port %u -j ACCEPT", available_ports[i]);
		system(cmd);
	}
}

pthread_t start_netif(void)
{
  pthread_t thread;
  int ret = pthread_create( &thread, NULL, netif_packet_capture, NULL);
  if(ret)
  {
    return -1;
  }
  return thread;
}

static void* netif_packet_capture(void *arg)
{
	int ret;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	struct pbuf *pnew;

	struct netif *mynetif = &my_netif;
	if (mynetif == NULL)
	{
	  return NULL;
	}

  /* Read the packets */
  while((ret = pcap_next_ex(gppcap, &header, &pkt_data)) >= 0)
  {
#if NO_SYS
    // Check lwIP timeouts.
    sys_check_timeouts();
#endif

    if(ret == 0)
    {
      /* Timeout elapsed */
      continue;
    }
    pnew = pbuf_alloc(PBUF_RAW, header->len, PBUF_RAM);
    if (pnew != NULL)
    {
      memcpy(pnew->payload, pkt_data, header->len);
      mynetif->input(pnew, mynetif);
    }

  }
	return NULL;
}

static err_t linux_lwip_init(struct netif *netif)
{
	// Setup lwIP arch interface.
	netif->output = etharp_output;
	netif->linkoutput = linux_link_output;
	netif->mtu = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP;
	netif->input = ethernet_input;
#if LWIP_IGMP
	netif->igmp_mac_filter = linux_igmp_mac_filter;
#endif // LWIP_IGMP
	return ERR_OK;
}

#if LWIP_NETIF_STATUS_CALLBACK
static void linux_net_status_cb(struct netif *netif)
{
}
#endif

#define netif_out_dbg(x) (void) 0
static err_t linux_link_output(struct netif *netif, struct pbuf *pfirst)
{
	/* Source location in pbuf payload for streamout.l/memcpy.b */
    register u32_t *src;
	/* Destination location in pbuf payload for memcpy.b */
	register u8_t *dest;
	/* Register used to pass the size of the payload to streamout.l/memcpy.b */
	register u32_t size;

    /* Current pbuf when combining pbufs. */
	struct pbuf *pcur;
	/* New pbuf as destination of combined pbufs. */
	struct pbuf *pnew;
	/* Total length of new pbuf payload. */
	u32_t tot_len;

	/* To support pbuf chains when sending the loop is used to add
	 * multiple pbuf payloads into the transmit buffer. When the packet has
	 * been formed then it can be transmitted. The tot_len and len members
	 * of the pbuf structure determine whether a pbuf is a partial packet.
	 */
	do
	{
		/* Each packet has 2 free bytes at the start of the first
		 * pbuf in the chain. This is primarily to align the payload
		 * to a 32 bit boundary. It works well for us as we can have
		 * an aligned buffer to stream into the ethernet transmit
		 * buffer directly.
		 * The payload of the first pbuf has a word free (ARCH_HW_HLEN)
		 * where the packet length word is written.
		 */

#if 0
		/* Calculate Ethernet frame length. This is the total size of the
		 * packet minus the length of the ethernet headers and the
		 * packet length word. */
		*((u16_t *)pfirst->payload) = pfirst->tot_len - ETHERNET_WRITE_HEADER - ARCH_HW_HLEN;
#endif

		/* Stream out the payloads from each pbuf in the chain.
		 * Start at the first.
		 */
		pcur = pfirst;

		/* Holder for an aligned pbuf. If the length of the payload of a
		 * pbuf is not a multiple of 4 bytes then it cannot be streamed into
		 * the transmit buffer as the buffer requires 32-bit accesses.
		 * Therefore anything other than aligned writes will result in gaps
		 * in the data between pbuf payloads! */
		pnew = NULL;

		/* Iterate trough the pbuf chain. */
		while (1)
		{
			/* Setup registers for stream instruction.
			 * Source data is the start of the pbuf payload. */
			src = (u32_t*)pcur->payload;
			/* Size is the length of this pbuf'f payload. */
			size = pcur->len;

			netif_out_dbg(("arch_ft900_link_output: pbuf chain size %"U32_F" %p\n", size, src));

			/* If the length of pbuf is not a multiple of 4 it cannot be streamed.
			 * The ETH_DATA register will only receive 32-bit writes correctly.
			 */
			if ((size & 3) != 0)
			{
				/* If, however, this is the last pbuf in a packet then we can
				 * send pad bytes after to ensure a 32-bit write at the end. The
				 * packet size in the frame header ensures the hardware knows
				 * when to stop transmitting.
				 */
				if (pcur->next != NULL)
				{
					netif_out_dbg(("arch_ft900_link_output: Alignment... copying remaining chain\n"));

					/* Combine this and remaining pbufs into one buffer.
					 * Do not use the pbuf_coalesce function as this will
					 * free the original pbufs and cause issues.
					 * The total_len for the packet is used to create a new
					 * pbuf large enough for all these other pbufs.
					 */
					pnew = pbuf_alloc(PBUF_RAW, pcur->tot_len, PBUF_RAM);
					if (pnew == NULL)
					{
						/* Out of memory for creating a new pbuf. */
						/* The original packet will still be sent.
						 * It will be picked up by a network analyser and
						 * reported as malformed.
						 */
						netif_out_dbg(("arch_ft900_link_output: copy failed\n"));
						LINK_STATS_INC(link.memerr);
					}
					else
					{
						/* Copy this packet and all subsequent packets into
						 * the new pbuf. Technically we could stop if any of
						 * these pbufs ends on a 32-bit boundary but this
						 * would require additional complexity without a
						 * massive increase in speed.
						 */
						dest = pnew->payload;
						tot_len = 0;

						do
						{
							src = (u32_t*)pcur->payload;
							size = pcur->len;
							memcpy((u8_t *) dest, (u8_t *) src, size);

							/* Adjust the new pbuf with the sizes of the original
							 * pbufs. The original pbufs are not modified or freed
							 * as the caller will take care of that.
							 */
							tot_len += size;
							dest += size;
							/* Move to the next pbuf in the chain of original pbufs. */
							pcur = pcur->next;
						} while (pcur);

						/* Finalise the important bits of the new pbuf. */
						src = (u32_t*) pnew->payload;
						size = tot_len;
						size = ((size + 3) & (~3));

						netif_out_dbg(("arch_ft900_link_output: new pbuf size now %"U32_F"\n", size));
					}
				}
			}

			pcap_sendpacket(gppcap, (u8_t *) src, size);

			/* If we have created a new aligned buffer then free it now.
			 * Remember we are leaving the original pbuf chain intact.
			 */
			if (pnew != NULL)
			{
				pbuf_free(pnew);
				break;
			}

			/* Are we at the end of the pbuf chain? Test for sending the
			 * total_len number of bytes.
			 */
			if (pcur->len == pcur->tot_len)
			{
				break;
			}

			/* Hard end of chain detected - catch this case. */
			if (pcur->next == NULL)
			{
				break;
			}

			/* Move to next pbuf in chain. */
			pcur = pcur->next;
		}

		// Move to next packet in chain. There probably isn't one.
		pfirst = pcur->next;
	} while (pfirst != NULL);
    return 0;
}
#if LWIP_IGMP
err_t linux_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group,  u8_t action)
{
	return ERR_OK;
}
#endif

static u32_t get_default_getway_ip(void)
{
  FILE *f;
  char line[100] , *p , *c, *g, *saveptr;
  u32_t gw = 0;

  f = fopen("/proc/net/route" , "r");
  if (f == NULL)
  {
    return 0;
  }
  while(fgets(line , 100 , f))
  {
    p = strtok_r(line , " \t", &saveptr);
    c = strtok_r(NULL , " \t", &saveptr);
    g = strtok_r(NULL , " \t", &saveptr);

    if(p != NULL && c != NULL)
    {
      if(strcmp(c , "00000000") == 0)
      {
        if (g)
        {
          char *pEnd;
          gw = (u32_t) strtol(g, &pEnd, 16);
        }
        break;
      }
    }
  }
  fclose(f);
  return gw;
}
