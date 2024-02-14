#include "message.h"
#include "sockwrapper.h"

#include <stdio.h>

#define DEFAULT_PORT ( 8000 )

int main( void )
{
        struct Socket* server_sock = sockw_socket();
        if ( sockw_bind( server_sock, DEFAULT_PORT ) == 0 )
        {
                sockw_listen( server_sock );
                struct Socket* client_sock = sockw_accept( server_sock );

                struct Message* request = sockw_read( client_sock );
                printf( "%s\n", msg_get( request ) );

                sockw_shutdown( client_sock );
        }
        sockw_shutdown( server_sock );   

        return 0;
}
