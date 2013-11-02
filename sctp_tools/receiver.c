#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <sys/socket.h>

const int local_port = 8080;
const char lan_addr[] = "192.168.1.116";
const char wifi_addr[] = "192.168.2.104";
const char file_name[] = "/tmp/dest.txt";

#define REALLY_BIG 65536

/* Convenience structure to determine space needed for cmsg. */
typedef union {
    struct sctp_initmsg init;
    struct sctp_sndrcvinfo sndrcvinfo;
} _sctp_cmsg_data_t;

typedef union {
    struct sockaddr_storage ss;
    struct sockaddr_in v4;
    struct sockaddr_in6 v6;
    struct sockaddr sa;
} sockaddr_storage_t;

    static int
test_sk_for_assoc(int sk, sctp_assoc_t assoc_id)
{
    int error = 0;
    struct sctp_status status;
    socklen_t status_len;

    memset(&status, 0, sizeof(status));
    if (assoc_id)
        status.sstat_assoc_id = assoc_id;
    status_len = sizeof(struct sctp_status);
    error = getsockopt(sk, SOL_SCTP, SCTP_STATUS,
            (char *)&status, &status_len);
    return error;
}

int store_file(struct msghdr *msg, size_t msg_len, FILE *fd)
{
    if(msg_len == 0)
        return;
    if(!(MSG_NOTIFICATION & msg->msg_flags)) 
    {
        int index = 0;
        
        printf("DATA(%d):  ", (int)msg_len);
        while ( msg_len > 0 ) {
            printf("writing to file\n");
            char *text;
            int len;

            text = msg->msg_iov[index].iov_base;
            len = msg->msg_iov[index].iov_len;

            if ( len > msg_len ) {
                len = msg_len;
            }

            if ( (msg_len -= len) > 0 ) { index++; }

            int ret = 0;

            while(ret != len) {
                int err = fwrite(text, sizeof(char), len, fd);
                if (err < 0)
                {
                    printf("error happens while writing\n");
                    exit(-1);
                }
                ret += err;
            }

        }
        fflush(fd);
    }
    else 
    {
        printf("NOTIFICAITON: \n");
    }
}

int build_endpoint()
{
    int retval, error;
    retval = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    int rcv_buf_size = 65535;
    setsockopt(retval, SOL_SOCKET, SO_RCVBUF, &rcv_buf_size, sizeof (rcv_buf_size));
    if(retval < 0 )
    {
        fprintf(stderr, "socket creation failed\n");
        exit(-1);
    }

    // bind to a LAN NIC
    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(local_port);
    inet_pton(AF_INET, lan_addr, &(myaddr.sin_addr));
    error = bind(retval, (struct sockaddr*) &myaddr, sizeof myaddr);
    if (error)
    {
        printf("binding failed: %s\n", strerror(errno));
        exit(-1);
    }

    // bind to a WAN NIC
    struct sockaddr_in myaddr2;
    myaddr2.sin_family = AF_INET;
    myaddr2.sin_port = htons(local_port);
    inet_pton(AF_INET, wifi_addr, &(myaddr2.sin_addr));

    error = sctp_bindx(retval, (struct sockaddr*)&myaddr2, 1, SCTP_BINDX_ADD_ADDR);
    if (error)
    {
        printf("bindingx failed: %s\n", strerror(errno));
        exit(-1);
    }
    return retval;
}

void do_listen(int sock)
{
    FILE *f;
    char incmsg[CMSG_SPACE(sizeof(_sctp_cmsg_data_t))];
    struct iovec iov;
    struct msghdr inmessage;
    sockaddr_storage_t msgname;
    int error;
    int done;
    int recvsk = 0;

    // listen to the port
    error = listen(sock, 5);

    if (error != 0)
    {
        printf("\n\n\t\tlisten failure! %s\n", strerror(errno));
    }

    /* Initialize inmessage with enough space for DATA*/
    memset(&inmessage, 0, sizeof(inmessage));

    if ((iov.iov_base = malloc(REALLY_BIG)) == NULL)
    {
        printf("Cannot allocate enough memory.\n");
        exit(-1);
    }
    iov.iov_len = REALLY_BIG;
    inmessage.msg_iov = &iov;
    inmessage.msg_iovlen = 1;

    /* Initialized inmessage for control message */
    inmessage.msg_control = incmsg;
    inmessage.msg_controllen = sizeof(incmsg);
    inmessage.msg_name = &msgname;
    inmessage.msg_namelen = sizeof(msgname);

    printf("\t Listenning...\n");

    done = 0;
    int finished = 0;
    while (!done)
    {
        socklen_t len = 0;
        if (!recvsk)
        {
            if ((recvsk = accept(sock, NULL, &len)) < 0)
            {
                fprintf(stderr, "error in accept:%s\n", strerror(errno));
                exit(-1);
            }
            
            /* Open the file after accepting a new socket */
            f = fopen(file_name, "w");
            finished = 0;
        }

        error = recvmsg(recvsk, &inmessage, MSG_WAITALL);
        if (error < 0)
        {
            if (ENOTCONN != errno)
                break;

            printf("Under what circumstance may we reach here?\n");
            close(recvsk);
            recvsk = 0;
            if (f != NULL)
                fclose(f);
            continue;
        }
        //        printf("call store_file\n");
        if (error)
        {
            finished += error;
            store_file(&inmessage, error, f);
            printf("%d bytes has been stored to %s. last_received=%d\n", 
                finished, file_name, error);
        }

        inmessage.msg_control = incmsg;
        inmessage.msg_controllen = sizeof(incmsg);
        inmessage.msg_name = &msgname;
        inmessage.msg_namelen = sizeof(msgname);
        iov.iov_len = REALLY_BIG;


        /* Verify that the association is no longer present.  */
        if (0 != test_sk_for_assoc(recvsk, 0)) {
            printf("No association is present now!!\n");
            close(recvsk);
            recvsk = 0;
            if (f != NULL)
                fclose(f);
            char command[100];
            memset(command, 0, sizeof(command));
            strcat(command, "md5sum ");
            strcat(command, file_name);
            system(command);
        }

    }

}

int main(int argc, char* argv[])
{
    int socket;
    socket = build_endpoint();
    do_listen(socket);

}
