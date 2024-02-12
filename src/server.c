#include "message.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_PORT ( 8000 )

struct Server
{
        int port;
        int socket;
        struct sockaddr_in srv_address;
};

int main( void )
{
        int port = DEFAULT_PORT;
        struct sockaddr_in srv_address;

        srv_address.sin_family = AF_INET;
        srv_address.sin_addr.s_addr = INADDR_ANY;
        srv_address.sin_port = htons( port );

        int sock = socket( AF_INET, SOCK_STREAM, 0 );

        int activate = 1;
        setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(activate) );

        if ( bind( sock, &srv_address, sizeof(srv_address) ) == 0 )
        {
                listen( sock, 100 );

                struct sockaddr_in cli_address;
                unsigned int cli_size;

                int cli_socket = accept( sock, &cli_address, &cli_size );


                //receive rest of packet
                char* buf = malloc( 13 );
                recv( cli_socket, buf, 13, MSG_WAITALL );

                printf( "%s\n", buf );
                close( cli_socket );
                close( sock );

        }

        return 0;
}
