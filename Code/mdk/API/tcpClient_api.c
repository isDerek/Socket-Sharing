#include "opt.h"
#include "debug.h"
#include "stats.h"
#include "tcp.h"
#include "tcpecho_raw.h"

#if LWIP_TCP

/**************************************added by derek 2017.11.7********************************/
/********** added by derek 2017.11.7****/
enum clients
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING,
	ES_CONNECTED
};
struct client
{
  u8_t state;
  u8_t retries;
  struct tcp_pcb *pcb;
  /* pbuf (chain) to recycle */
  struct pbuf *p;
};
static void
tcpecho_client_free(struct client *es)
{
  if (es != NULL) {
    if (es->p) {
      /* free the buffer chain if present */
      pbuf_free(es->p);
    }

    mem_free(es);
  }  
}
/*****************************/

	struct tcp_pcb *client_pcb;
	struct client *es;
	err_t err;
	ip4_addr_t destipaddr;
/**********************************************************************/
void tcp_client_send(struct tcp_pcb *tpcb, struct client *es)
{

}
/*************************************************************************/
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client *es)
{
	tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  tcpecho_client_free(es);

  tcp_close(tpcb);
}
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{

  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);
  es = (struct client *)arg;
  if (p == NULL) {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p == NULL) {
      /* we're done sending, close it */
      tcp_client_connection_close(tpcb, es);
			
    } 
    ret_err = ERR_OK;
  } else if(err != ERR_OK) {
    /* cleanup, for unknown reason */
    if (p != NULL) {
      pbuf_free(p);
    }
    ret_err = err;
  }
		else if (es->state == ES_RECEIVED) {
    /* read some more data */
   
   
     tcp_recved(tpcb,p->tot_len);
		memset(p,0x00,TCPREVDATALEN);   // added by derek 2017.11.7  recevrxbuffertcpeth
		if(p->len> TCPREVDATALEN)
		{
			p->len = TCPREVDATALEN;
		}
		memcpy(p,p->payload,p->len);    // added by derek 2017.11.7
		tcpRecevieFlag = 1;
		pbuf_free(p);
		ret_err = ERR_OK;

  } else {
    /* unkown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{

	if(err == NULL)
	{
	es = (struct client *)mem_malloc(sizeof(struct client));
	}
	switch(err)
	{
		case ERR_OK:
			es ->pcb =tpcb;
		  es ->state = ES_CONNECTED;
		  tcp_arg(tpcb,es);
			tcp_recv(tpcb,tcp_client_recv);
		break;
		case ERR_MEM:
			tcp_client_connection_close(tpcb,es);
		break;
		default:
			break;
		  
	}
	return err;
}
err_t tcp_client_connect(const uint8_t *destip, uint16_t port)
{
	client_pcb = tcp_new();
	client_pcb ->so_options |= SOF_KEEPALIVE;
	client_pcb ->keep_idle = 50000;   //ms    tcp connect 5s ,send first alive package       2s send alive    no receive disconnect
	if (client_pcb != NULL)
	{
		IP4_ADDR(&destipaddr,*destip,*(destip+1),*(destip+2),*(destip+3));
		/* connect to destination address/port */
		err = tcp_connect(client_pcb,&destipaddr,port,tcp_client_connected);
		//  tcp_err(client_pcb,tcp_errf);
		
	}
	return err;
}
#endif /* LWIP_TCP */
