#
# Makefile for SCTP support code.
#

obj-$(CONFIG_IP_SCTP) += sctp.o
obj-$(CONFIG_NET_SCTPPROBE) += sctp_probe.o

sctp-y := sm_statetable.o sm_statefuns.o sm_sideeffect.o \
	  protocol.o endpointola.o associola.o \
	  transport.o chunk.o sm_make_chunk.o ulpevent.o \
	  inqueue.o outqueue.o ulpqueue.o command.o \
	  tsnmap.o bind_addr.o socket.o primitive.o \
	  output.o input.o debug.o ssnmap.o auth.o cmt.o

sctp_probe-y := probe.o

sctp-$(CONFIG_SCTP_DBG_OBJCNT) += objcnt.o
sctp-$(CONFIG_PROC_FS) += proc.o
sctp-$(CONFIG_SYSCTL) += sysctl.o

sctp-$(subst m,y,$(CONFIG_IPV6))	+= ipv6.o

#CFLAGS_sm_statetable.o := -DDEBUG
#CFLAGS_sm_statefuns.o := -DDEBUG
CFLAGS_sm_sideeffect.o := -DDEBUG
#CFLAGS_protocol.o := -DDEBUG
#CFLAGS_endpointola.o := -DDEBUG
#CFLAGS_associola.o := -DDEBUG
CFLAGS_transport.o := -DDEBUG
#CFLAGS_chunk.o := -DDEBUG
#CFLAGS_sm_make_chunk.o := -DDEBUG
#CFLAGS_ulpevent.o := -DDEBUG
#CFLAGS_inqueue.o := -DDEBUG
#CFLAGS_ulpqueue.o := -DDEBUG
#CFLAGS_command.o := -DDEBUG
#CFLAGS_tsnmap.o := -DDEBUG
#CFLAGS_bind_addr.o := -DDEBUG
#CFLAGS_socket.o := -DDEBUG
#CFLAGS_primitive.o := -DDEBUG
#CFLAGS_output.o := -DDEBUG
#CFLAGS_input.o := -DDEBUG
#CFLAGS_debug.o := -DDEBUG
#CFLAGS_ssnmap.o := -DDEBUG
#CFLAGS_auth.o:= -DDEBUG
#CFLAGS_cmt.o:= -DDEBUG
CFLAGS_outqueue.o := -DDEBUG
