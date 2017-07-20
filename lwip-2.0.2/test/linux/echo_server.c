/*
 * server_test.c
 *
 *  Created on: Jul 19, 2017
 *      Author: admin
 */
#include "lwip.h"

#define echo_server_dbg(x) (void) 0

static err_t echo_server_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static err_t echo_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static void echo_server_err(void *arg, err_t err);
static err_t echo_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
static int echo_server_send(struct tcp_pcb *pcb, const char *buf, size_t len);

err_t create_echo_server(void)
{
  struct tcp_pcb *pcb;
  err_t err;

  pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
  if (pcb == NULL)
  {
	  return ERR_MEM;
  }
  tcp_setprio(pcb, TCP_SERVER_PRIO);
  err = tcp_bind(pcb, IP_ANY_TYPE, TCP_LOCAL_SERVER_PORT);
  LWIP_UNUSED_ARG(err); /* in case of LWIP_NOASSERT */
  LWIP_ASSERT("echo_server_init: tcp_bind failed", err == ERR_OK);

  pcb = tcp_listen(pcb);
  LWIP_ASSERT("echo_server_init: tcp_listen failed", pcb != NULL);

  tcp_accept(pcb, echo_server_accept);
  return ERR_OK;
}

static err_t echo_server_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  LWIP_UNUSED_ARG(err);
  LWIP_UNUSED_ARG(arg);
  echo_server_dbg(("echo_server_accept %p / %p\n", (void*)pcb, arg));

  if ((err != ERR_OK) || (pcb == NULL)) {
    return ERR_VAL;
  }

  /* Set priority */
  tcp_setprio(pcb, TCP_SERVER_PRIO);

  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, NULL);

  /* Set up the various callback functions */
  tcp_recv(pcb, echo_server_recv);
  tcp_err(pcb, echo_server_err);
  tcp_sent(pcb, echo_server_sent);
  printf("Echo server accepted new connection.\n");
  return ERR_OK;
}

static err_t echo_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  char *recv_buf;
  echo_server_dbg(("echo_server_recv: pcb=%p pbuf=%p err=%s\n", (void*)pcb,
    (void*)p, lwip_strerr(err)));

  if ((err != ERR_OK) || (p == NULL))
  {
    /* error or closed by other side? */
    if (p != NULL) {
      /* Inform TCP that we have taken the data. */
      tcp_recved(pcb, p->tot_len);
      pbuf_free(p);
    }
    printf("Connection closed.\n");
    return ERR_OK;
  }

  /* Inform TCP that we have taken the data. */
  tcp_recved(pcb, p->tot_len);

  recv_buf = (char *) malloc(p->len + 1);
  if (recv_buf != NULL)
  {
	  memcpy(recv_buf, p->payload, p->len);
	  recv_buf[p->len] = 0;
	  printf("%s", recv_buf);

	  echo_server_send(pcb, recv_buf, p->len + 1);
	  free(recv_buf);
  }

  pbuf_free(p);
  return ERR_OK;
}

static void echo_server_err(void *arg, err_t err)
{
  LWIP_UNUSED_ARG(err);
  echo_server_dbg(("echo_server_err: %s", lwip_strerr(err)));
}

static err_t echo_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  echo_server_dbg(("echo_server_sent %p\n", (void*)pcb));
  LWIP_UNUSED_ARG(len);
  return ERR_OK;
}

static int echo_server_send(struct tcp_pcb *pcb, const char *buf, size_t len)
{
  u16_t max_len;
  err_t err = ERR_OK;
  size_t offset = 0;
  size_t snd_len  = len;
  u8_t apiflags = TCP_WRITE_FLAG_COPY; /* Data is copy to sending buffer: Need to optimize */

  if (pcb == NULL || buf == NULL || len == 0)
  {
	  echo_server_dbg(("Bad input!\n"));
	  return ERR_ARG;
  }

  while(1)
  {
    /* We cannot send more data than space available in the send buffer. */
    max_len = tcp_sndbuf(pcb);
    if (max_len < snd_len)
    {
      snd_len = max_len;
    }
    do {
      /* Write packet to out-queue, but do not send it until tcp_output() is called. */
      err = tcp_write(pcb, buf + offset, snd_len, apiflags);
      if (err == ERR_MEM)
      {
        if ((tcp_sndbuf(pcb) == 0) ||
            (tcp_sndqueuelen(pcb) >= TCP_SND_QUEUELEN)) {
          /* no need to try smaller sizes */
          snd_len = 1;
        } else {
          snd_len /= 2;
        }
      }
      if (err == ERR_OK)
      {
        tcp_output(pcb); /* Send the packet immediately */
      }
    } while ((err == ERR_MEM) && (snd_len > 1));

    if (err == ERR_OK) {
      offset += snd_len;
      snd_len = len - snd_len;
    }
    if (snd_len == 0)
    {
      break;
    }
  }

  /* ensure nagle is normally enabled (only disabled for persistent connections
      when all data has been enqueued but the connection stays open for the next
      request */
  tcp_nagle_enable(pcb);
  return offset;
}
