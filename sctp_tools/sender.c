#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <sys/socket.h>
#include <sys/types.h>

const int local_port = 7070;
const char lan_addr[] = "192.168.1.106";
const char wifi_addr[] = "192.168.2.110";

const int peer_port = 8080;
const char p_lan_addr[] = "192.168.1.114";
const char p_wifi_addr[] = "192.168.2.107";

const char file_name[] = "/home/xiangzhc/ponponpon.mp3";

struct sockaddr_in local_addr[2];
struct sockaddr_in peer_addr[2];
#define REALLY_BIG (128 * 1024)

char message[REALLY_BIG] = "";


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

int build_endpoint()
{
    int retval, error;
    retval = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    perror("socket create result");

    printf("SO_RCVBUF=%d, SO_SNDBUF=%d\n", SO_RCVBUF, SO_SNDBUF);

  
    int rcv_buf_size = 1024 * 1024;// 1024 * 1024 * 10;                                        
    error = setsockopt(retval, SOL_SOCKET, SO_RCVBUF, &rcv_buf_size, sizeof (rcv_buf_size));
    perror("set [rcv_buf] result");                                             

    int snd_buf_size = 65536;//1024 * 1024 * 10; 
    error = setsockopt(retval, SOL_SOCKET, SO_SNDBUF, &snd_buf_size, sizeof (snd_buf_size));
    perror("set [snd_buf] result");                                             
    
    int max_burst = 1024;                                                       
    setsockopt(retval, SOL_SCTP, SCTP_MAX_BURST, &max_burst, sizeof(max_burst));
    perror("set [max_burst] result");  

    local_addr[0].sin_family = AF_INET;
    local_addr[0].sin_port = htons(local_port);
    inet_pton(AF_INET, lan_addr, &(local_addr[0].sin_addr));
    error = bind(
            retval, 
            (struct sockaddr*) local_addr, 
            sizeof(struct sockaddr_in));
    if(error)
    {
        printf("binding failed: %s\n", strerror(errno));
        exit(-1);
    }

    local_addr[1].sin_family = AF_INET;
    local_addr[1].sin_port = htons(local_port);
    inet_pton(AF_INET, wifi_addr, &(local_addr[1].sin_addr));
    
    error = sctp_bindx(
            retval, 
            (struct sockaddr*) local_addr + 1, 
            1, 
            SCTP_BINDX_ADD_ADDR);

    if(error)
    {
        printf("bindxing failed: %s\n", strerror(errno));
        exit(-1);
    }
    return retval;
}

static int
connectx_func(int sk, struct sockaddr_in *addrs, int count)
{

    int error;
    int i;
    struct sockaddr_in *aptr;


    /* Set the port in every address.  */
    aptr = addrs;
    for (i = 0; i < count; i++) {
        aptr->sin_port = htons(peer_port);
        aptr++;
    }    

    error = sctp_connectx(sk, (struct sockaddr*)addrs, count, NULL);

    if (error != 0) { 
        if (errno == ECONNREFUSED)
            return -2;
        fprintf(stderr, "%s: error connecting to addrs: %s.\n",
                "", strerror(errno));
        return -1;
    }    

    return 0;

} /* connectx_func() */

void do_send(int sock)
{
    struct iovec iov;
    int error;
    int done = 0;

    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("failed to open file\n");
        exit(-1);
    }

    error = connectx_func(sock, peer_addr, 2);

    if (error != 0)
    {
        printf("\n\n\t\tconnect failure! %s\n", strerror(errno));
    }

    printf("Connected\n");


    int finished = 0;
    int read = 0;
    while ((read = fread(message, 1, REALLY_BIG, f)) != 0)
    {
        printf("%d byes has been read\n", read);
        int acc = 0;
        while(acc != read) {
            acc += send(sock, message, read, 0); 
//            printf("\t%d bytes has been sent\n", acc);
        }
        finished += acc;
        printf("====>cycle done! finished=%d\n", finished);
    }

    char command[100];
    memset(command, 0, sizeof(command));
    strcpy(command, "md5sum ");
    strcat(command, file_name);
    system(command);

    close(sock);
    if (f)
        fclose(f);
}

void append_addr()
{
    // bind to a LAN NIC
    peer_addr[0].sin_family = AF_INET;
    peer_addr[0].sin_port = htons(peer_port);
    inet_pton(AF_INET, p_lan_addr, &(peer_addr[0].sin_addr));
    
    peer_addr[1].sin_family = AF_INET;
    peer_addr[1].sin_port = htons(peer_port);
    inet_pton(AF_INET, p_wifi_addr, &(peer_addr[1].sin_addr));
}

int main(int argc, char* argv[])
{
    int socket;
    append_addr();
    socket = build_endpoint();
    do_send(socket);

}
