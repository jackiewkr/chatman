#include "message.h"
#include "sockwrapper.h"


int main( void )
{
        struct Socket* server_sock = sockw_socket() ;

        if ( sockw_connect( server_sock, "127.0.0.1", 8000 ) == 0 )
        {
                struct Message* msg = msg_init( "Hello, World!", 13 );
                sockw_write( server_sock, msg );
        }

        sockw_shutdown( server_sock );
        return 0;
}
