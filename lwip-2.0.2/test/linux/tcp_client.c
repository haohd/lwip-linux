/*
 * tcp_client.c
 *
 *  Created on: Jul 20, 2017
 *      Author: admin
 */
#include "lwip.h"

#define _dbg(x) printf x

static struct tcp_pcb* connect_to_server(const ip_addr_t *ip_addr, u16_t port);
static void tcpc_err_cb(void *arg, err_t err);
static err_t tcpc_connect_cb(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcpc_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static err_t tcpc_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcpc_poll_cb(void *arg, struct tcp_pcb *tpcb);
static int tcpc_send(struct tcp_pcb *pcb, const char *buf, size_t len);

static int connect_status = ERR_OK + 1;
struct tcp_pcb *client_conn = NULL;

err_t tcp_client(const ip_addr_t *ip_addr, u16_t port)
{
  struct tcp_pcb *conn = NULL;
  char data[256];

  connect_status = ERR_OK + 1;
  conn = connect_to_server(ip_addr, port);
  if (conn == NULL)
  {
    return ERR_CONN;
  }

  /* Wait for server accept the connection */
  while(connect_status == ERR_OK + 1);

  if (connect_status != ERR_OK || client_conn == NULL)
  {
    _dbg(("Failed to connect to server!\n"));
    tcp_abort(conn);
    return ERR_CONN;
  }

  _dbg(("Connected!\n"));
  /* Connection accepted */
  while(1)
  {
    if (fgets(data, sizeof(data), stdin))
    {
      if (!strcmp(data, "quit\n"))
      {
        break;
      }
      tcpc_send(client_conn, data, strnlen(data, sizeof(data)));
    }
    else
    {
      break;
    }
  }
  tcp_close(conn);
  return ERR_OK;
}

static struct tcp_pcb* connect_to_server(const ip_addr_t *ip_addr, u16_t port)
{
  struct tcp_pcb *conn = NULL;
  err_t err;

  conn = tcp_new();
  if (conn == NULL) {
    return NULL;
  }

  /* Set arg pointer for callbacks */
  tcp_arg(conn, NULL);

  /* Any local address, pick random local port number */
  err = tcp_bind(conn, IP_ADDR_ANY, 0);
  if (err != ERR_OK)
  {
    _dbg(("Error binding to local ip/port, %d\n", err));
    goto tcp_fail;
  }
  _dbg(("Connecting to host: %s at port:%"U16_F"\n", ipaddr_ntoa(ip_addr), port));

  /* Connect to server */
  err = tcp_connect(conn, ip_addr, port, tcpc_connect_cb);
  if (err != ERR_OK)
  {
    _dbg(("Error connecting to remote ip/port, %d\n", err));
    goto tcp_fail;
  }

  /* Set error callback */
  tcp_err(conn, tcpc_err_cb);
  return conn;

tcp_fail:
  if (conn)
  {
    tcp_abort(conn);
  }
  return NULL;
}

static void tcpc_err_cb(void *arg, err_t err)
{
  LWIP_UNUSED_ARG(err); /* only used for debug output */
  _dbg(("tcpc_err_cb: TCP error callback: error %d, arg: %p\n", err, arg));
  connect_status = err;
}

static err_t tcpc_connect_cb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  if (err != ERR_OK)
  {
    _dbg(("tcpc_connect_cb: TCP connect error %d\n", err));
    return err;
  }

  /* Setup TCP callbacks */
  tcp_recv(tpcb, tcpc_recv_cb);
  tcp_sent(tpcb, tcpc_sent_cb);
  tcp_poll(tpcb, tcpc_poll_cb, 20);

  client_conn = tpcb;
  connect_status = ERR_OK;
  return ERR_OK;
}

static err_t tcpc_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  if (p == NULL)
  {
    _dbg(("tcpc_recv_cb: Recv pbuf=NULL, remote has closed connection\n"));
  }
  else
  {
    if (err != ERR_OK)
    {
      _dbg(("tcpc_recv_cb: Recv err=%d\n", err));
      pbuf_free(p);
      return err;
    }

    /* Tell remote that data has been received */
    tcp_recved(pcb, p->tot_len);

    pbuf_free(p);
  }
  return ERR_OK;
}

static err_t tcpc_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  LWIP_UNUSED_ARG(tpcb);
  LWIP_UNUSED_ARG(len);
  return ERR_OK;
}

static err_t tcpc_poll_cb(void *arg, struct tcp_pcb *tpcb)
{
  return ERR_OK;
}

static int tcpc_send(struct tcp_pcb *pcb, const char *buf, size_t len)
{
  u16_t max_len;
  err_t err = ERR_OK;
  size_t offset = 0;
  size_t snd_len  = len;
  u8_t apiflags = TCP_WRITE_FLAG_COPY; /* Data is copy to sending buffer: Need to optimize */

  if (pcb == NULL || buf == NULL || len == 0)
  {
    _dbg(("Bad input!\n"));
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
