/* SCTP-CMT kernel implementation                                               
 *                                                                               
 * This file is for the Concurrent Multi-path Transfer Feature of SCTP          
 *                                                                              
 * Written by                                                                   
 * Xiangzhong Chang    <changxiangzhong@gmail.com>                         
 */ 
#ifndef __sctp_cmt__
#define __sctp_cmt__

#define CMT_PREFIX "cxz: "

#define TSN_HEEDED 100000

#if defined(CONFIG_DYNAMIC_DEBUG)
/* dynamic_pr_debug() uses pr_fmt() internally so we don't need it here */
#define cmt_debug(fmt, ...) \
    dynamic_pr_debug(CMT_PREFIX fmt, ##__VA_ARGS__)
#elif defined(DEBUG)
#define cmt_debug(fmt, ...) \
    printk(KERN_DEBUG pr_fmt(CMT_PREFIX fmt), ##__VA_ARGS__)
#else
#define cmt_debug(fmt, ...) \
    no_printk(KERN_DEBUG pr_fmt(CMT_PREFIX fmt), ##__VA_ARGS__)
#endif

extern char* cmt_print_trxpt(struct sctp_transport *t);

extern char* cmt_print_assoc(struct sctp_association *asoc);

extern char* cmt_print_sackhdr(struct sctp_sackhdr *sack);

extern char* cmt_print_queued_tsn(struct list_head *queue, struct sctp_transport *transport);

extern char* cmt_print_cwnd(struct list_head *transport_list);
#endif /*__sctp_cmt__*/
