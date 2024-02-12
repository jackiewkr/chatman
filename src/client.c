#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void )
{
        struct sockaddr_in srv_address;
        srv_address.sin_family = AF_INET;
        srv_address.sin_addr.s_addr = inet_addr("127.0.0.1");
        srv_address.sin_port = htons( 8000 );

        int clisock = socket( AF_INET, SOCK_STREAM, 0 );
        if ( connect( clisock, &srv_address, sizeof(srv_address) ) == 0 )
        {
                send( clisock, "Hello, World!", 13, 0 );

                close( clisock );
        }
        return 0;
}
