/*
 * if.c
 *
 *  Created on: Jul 19, 2017
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
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

int get_if_address(const char *ifname, uint32_t *ip, uint32_t *mask, uint8_t *mac)
{
  char *buffer = NULL;
  struct ifreq *ifr;
  size_t i;
  int ret = -1;

  if (ifname == NULL || ip == NULL || mask == NULL || mac == NULL)
  {
    return -1;
  }
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0)
  {
    return -1;
  }

  struct ifreq _ifr;
  memset(&_ifr, 0, sizeof(_ifr));
  _ifr.ifr_addr.sa_family = AF_INET;
  strncpy(_ifr.ifr_name, ifname , IFNAMSIZ - 1);
  if (0 == ioctl(fd, SIOCGIFHWADDR, &_ifr))
  {
      memcpy(mac, (unsigned char *)_ifr.ifr_hwaddr.sa_data, 6);
  }

  buffer = (char *) malloc(2048);
  if (buffer == NULL)
  {
    goto _end;
  }

  struct ifconf ifc = {
      .ifc_len = 2048,
      .ifc_buf = (caddr_t) buffer
  };
  if (ioctl(fd, SIOCGIFCONF, &ifc) < 0)
  {
      goto _end;
  }

  for (i = 0; i < ifc.ifc_len; i += sizeof(*ifr))
  {
    ifr = (struct ifreq *)(buffer + i);
    if (ifr->ifr_addr.sa_family != AF_INET)
    {
      // only support IPv4
      continue;
    }

    char _ip[16] = {};
    struct sockaddr_in * addr = (struct sockaddr_in *) &ifr->ifr_addr;
    if (inet_ntop(AF_INET, &addr->sin_addr, _ip, sizeof(_ip)) == NULL)
    {
      continue;
    }

    if(!strcmp(ifname, ifr->ifr_name))
    {
      struct ifreq netmask = {};
      strcpy(netmask.ifr_name, ifr->ifr_name);
      if (ioctl(fd, SIOCGIFNETMASK, &netmask) != 0)
      {
        continue;
      }
      *ip = addr->sin_addr.s_addr;
      addr = (struct sockaddr_in *) &netmask.ifr_addr;
      *mask = addr->sin_addr.s_addr;
      ret = 1;
    }
  }
_end:
  if (buffer)
  {
    free(buffer);
  }
  if (fd >= 0)
  {
    close(fd);
  }
  return ret;
}


