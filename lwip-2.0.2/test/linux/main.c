/*
 * main.c
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
#include "lwip.h"

int main(void)
{
  pthread_t thread;

	if(net_init(NULL) != ERR_OK)
	{
		printf("Failed to initialize netif!\n");
		goto _EXIT;
	}

	thread = start_netif();
	if (thread < 0)
	{
    printf("Failed to start netif!\n");
    goto _EXIT;
	}

#if TEST_ID == ECHO_SERVER
  if (create_echo_server() != ERR_OK)
  {
    printf("Failed to create echo server!\n");
    goto _EXIT;
  }
#elif TEST_ID == TCP_CLIENT
  ip_addr_t ip_addr = { TCP_REMOTE_SERVER_ADDR };
  if (tcp_client(&ip_addr, TCP_REMOTE_SERVER_PORT) != ERR_OK)
  {
    printf("Failed to connect to server!\n");
    goto _EXIT;
  }
#endif


	pthread_join(thread, NULL);
_EXIT:
  net_quit();
	return 1;
}




