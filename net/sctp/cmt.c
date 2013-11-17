/**
 * SCTP-CMT kernel implementation
 *
 * This file is for the Concurrent Multi-path Transfer Feature of SCTP
 *
 * Written by 
 *      Xiangzhong Chang    <changxiangzhong@gmail.com>
 */

#include <net/sctp/sctp.h>
#include <net/sctp/cmt.h>
#include <linux/list.h>   /* For struct list_head */


char* cmt_print_trxpt(struct sctp_transport *t)
{
    static char ret[256];
    memset(ret, 0, sizeof(ret));
    if (!t)
	return ret;
    snprintf(ret, sizeof(ret), "trxpt@%p[addr=%pISc,state=%d,cwnd=%d,ssthresh=%d,sfr.saw_newack=%d,sfr.highest=0x%x,cuc.pcumack=0x%x,cuc.find_pc=%d,cuc.new_pc=%d],", 
            t,
            &t->ipaddr.sa,
            t->state,
            t->cwnd,
            t->ssthresh,
	    t->cmt_sfr.saw_newack,
	    t->cmt_sfr.hisfd,
	    t->cmt_cuc.pseudo_cumack,
	    t->cmt_cuc.find_pseudo_cumack,
	    t->cmt_cuc.new_pseudo_cumack);
    return ret;
}

char* cmt_print_cwnd(struct list_head *transport_list) {
	int c = 0;
	struct sctp_transport *transport;
	static char buf[256];
	memset(buf, 0, sizeof(buf));
	list_for_each_entry(transport, transport_list,
			transports)
		c += snprintf(
				buf + c,
				sizeof(buf) - c,
				"==>trx|%p|c=%d|ssh=%d|fly=%d|,",
				transport,
				transport->cwnd,
				transport->ssthresh,
				transport->flight_size
				);
	return buf;
}

char* cmt_print_assoc(struct sctp_association *asoc)
{
    struct list_head *pos, *temp;
    struct sctp_transport *transport;
    static char ret[1024];
    memset(ret, 0, sizeof(ret));
    if (!asoc)
	    return ret;
    snprintf(
            ret, 
            sizeof(ret), 
            "asoc[state=%d,active=%p,retran=%p,pri=%p\t",
            asoc->state,
            asoc->peer.active_path,
            asoc->peer.retran_path,
            asoc->peer.primary_path
            );
    
    strlcat(ret, "[l=(", sizeof(ret));
    list_for_each_safe(pos, temp, &asoc->peer.transport_addr_list) {
        transport = list_entry(pos, struct sctp_transport, transports);
        strlcat(ret, cmt_print_trxpt(transport), sizeof(ret));
    }

    strlcat(ret, ")]", sizeof(ret));
    return ret;
}

char* cmt_print_sackhdr(struct sctp_sackhdr *sack)
{
	int i;
	sctp_sack_variable_t *frags = sack->variable;
	static char buf[256];
	__u32 ctsn = ntohl(sack->cum_tsn_ack);
	int gap_count = ntohs(sack->num_gap_ack_blocks);

	memset(buf, 0, sizeof(buf));
	if (gap_count)
		snprintf(buf, sizeof(buf), "gack={0x%x|", ntohl(sack->cum_tsn_ack));
	else
		snprintf(buf, sizeof(buf), "sack={0x%x|", ntohl(sack->cum_tsn_ack));
	for (i = 0; i < gap_count; i++) {
		snprintf(buf + strlen(buf), 
				sizeof(buf) - strlen(buf),
				"[0x%x~0x%x]", 
				ctsn + ntohs(frags[i].gab.start),
				ctsn + ntohs(frags[i].gab.end));
	}
	strlcat(buf, "}", sizeof(buf));

	return buf;
}

char* cmt_print_queued_tsn(struct list_head *queue, struct sctp_transport *transport)
{
	static char buf[1024 * 2];
	struct sctp_chunk *tchunk;
	int ret;
	__u32 tsn;
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "%p.queue={", transport);
	list_for_each_entry(tchunk, queue, transmitted_list) {
		tsn = ntohl(tchunk->subh.data_hdr->tsn);
		ret = snprintf(buf + strlen(buf),
				sizeof(buf) - strlen(buf),
				tchunk->tsn_gap_acked? "0x%x(A), ": "0x%x, ",
				tsn);
		// success when return value is non-negative and less than n
		if(!(ret > 0 && ret < sizeof(buf) - strlen(buf)))
			break;
	}
	strlcat(buf, "}", sizeof(buf));
	return buf;
}
