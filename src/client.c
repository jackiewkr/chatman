#include "message.h"
#include "sockwrapper.h"

#include <unistd.h>

int main( void )
{
        struct Socket* server_sock = sockw_socket() ;

        if ( sockw_connect( server_sock, "127.0.0.1", 8000 ) == 0 )
        {
                sockw_write( server_sock, msg_init( JOIN, "JohnDoe1", 8 ) );
                //sleep(3);
                sockw_write( server_sock, msg_init( WRITE, "Hello, World!", 13 ) );
                //sleep(3);
                sockw_write( server_sock, msg_init( LEAVE, "", 0 ) );
        }

        sockw_shutdown( server_sock );
        return 0;
}
