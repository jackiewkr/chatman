#include "message.h"
#include "sockwrapper.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int should_quit = 0;

void quitHandler( int sig )
{
        should_quit++;
}

int main( void )
{
        //set quit handler
        signal( SIGINT, quitHandler );
        struct Socket* server_sock = sockw_socket() ;

        if ( sockw_connect( server_sock, "127.0.0.1", 8000 ) == 0 )
        {
                // send join with randomized username
                srand( time( NULL ) );
                int r = rand();
                char* username = malloc(16);
                snprintf( username, 16, "Doe%i", r );
                sockw_write( server_sock, msg_init( JOIN, username, 16 ) );
                sockw_write( server_sock, msg_init( WRITE, "Hello, World!", 
                                          13 ) );
                while (!should_quit)
                {
                        // print received messages if any exist
                        struct Message* req;
                        while ( ( req = sockw_read( server_sock ) ) != NULL )
                        {
                                fprintf( stderr, "%s\n", msg_getBody( req ) );
                        }
                }
        }
        // send leave
        sockw_write( server_sock, msg_init( LEAVE, "", 0 ) );
        sockw_shutdown( server_sock );
        return 0;
}
