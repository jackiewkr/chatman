#include "message.h"
#include "sockwrapper.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void )
{
        struct Socket* server_sock = sockw_socket() ;

        if ( sockw_connect( server_sock, "127.0.0.1", 8000 ) == 0 )
        {
                srand( time( NULL ) );
                int r = rand();
                char* username = malloc(16);
                snprintf( username, 16, "Doe%i", r );
                sockw_write( server_sock, msg_init( JOIN, username, 16 ) );
                while (1)
                {
                        sockw_write( server_sock, msg_init( WRITE, 
                                                            "Hello, World!", 
                                                            13 ) );
                        
                        struct Message* req = sockw_read( server_sock );
                        while ( req != NULL )
                        {

                                fprintf( stderr, "%s\n", msg_getBody( req ) );
                                req = sockw_read( server_sock );
                        }
                }
        }
        sockw_write( server_sock, msg_init( LEAVE, "", 0 ) );
        sockw_shutdown( server_sock );
        return 0;
}
