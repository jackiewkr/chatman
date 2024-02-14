#include "sockwrapper.h"
#include "message.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define INIT_BUF_SZ ( 4 )       //initial size of buffer when reading msg
#define QUEUE_SIZE  ( 100 )     //size of queue when calling listen()

struct Socket
{
        struct sockaddr_in address;      //address and port
        int socknum;                     //socket number
};

struct Socket* sockw_socket( void )
{
        struct Socket* sockw = malloc( sizeof(struct Socket) );

        sockw->socknum = socket( AF_INET, SOCK_STREAM, 0 );

        return sockw;
}

int sockw_bind( struct Socket* sockw, int port )
{
int active = 1;
        setsockopt( sockw->socknum, SOL_SOCKET, SO_REUSEADDR, &active, 
                    sizeof(active) );
        
        sockw->address.sin_family = AF_INET;
        sockw->address.sin_addr.s_addr = INADDR_ANY;
        sockw->address.sin_port = htons( port );

        return bind( sockw->socknum, &sockw->address, sizeof(sockw->address) );
}

void sockw_listen( struct Socket* sockw )
{
       listen( sockw->socknum, QUEUE_SIZE );
}

int sockw_connect( struct Socket* sockw, char* address, int port )
{
        struct sockaddr_in srv_address;
        srv_address.sin_family = AF_INET;
        srv_address.sin_addr.s_addr = inet_addr( address );
        srv_address.sin_port = htons( port );

        return connect( sockw->socknum, &srv_address, sizeof(srv_address) );
}

struct Socket* sockw_accept( struct Socket* sockw )
{
        struct Socket* client_sockw = malloc( sizeof(struct Socket) );
        int addr_sz;
        client_sockw->socknum = accept( sockw->socknum, &client_sockw->address, 
                                        &addr_sz );

        return client_sockw;
}

struct Message* sockw_read( struct Socket* sockw )
{
        int buf_size = INIT_BUF_SZ;
        char* buf = malloc( buf_size );

        unsigned int complete = 0;
        unsigned int offset = 0;

        while ( !complete )
        {
                int bytes_received = recv( sockw->socknum, buf + offset, 
                                           buf_size - offset, 0 );

                if ( bytes_received < 0 )
                {
                        fprintf( stderr, "An Error Occurred: %s\n", 
                                 strerror( errno ) );
                        return NULL;    //error occurred
                }
                else if ( bytes_received == 0 )
                        complete++;     // full message transmitted
                else
                {
                        offset += bytes_received;

                        //resize buf to double prev size
                        buf_size <<= 1;
                        buf = realloc( buf, buf_size );
                }
        }

        return msg_init( buf, strlen( buf ) ); 
}

void sockw_write( struct Socket* sockw, struct Message* msg )
{
        send( sockw->socknum, msg_get( msg ), msg_getSize( msg ), 0 );
}

void sockw_shutdown( struct Socket* sockw )
{
        shutdown( sockw->socknum, SHUT_RDWR );
        free( sockw );
}
