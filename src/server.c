#include "message.h"
#include "sockwrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define DEFAULT_PORT ( 8000 )

struct ClientThread
{
        struct Server* srv;
        struct Socket* sockw;
};

struct Server
{
        struct Socket* srv_sock;

        struct Message** log;           //chat log
        unsigned int log_sz;
        unsigned int log_lock;          //whether log is currently being edited
};

/* mangle message to become RECEIVE message with username */
struct Message* mangleMessage( char* username, char* body )
{
        char* buf = malloc( MSG_MAX_SZ );
        snprintf( buf, MSG_MAX_SZ, "%s: %s", username, body );
        return msg_init( RECEIVE, buf, MSG_MAX_SZ );
}

void addMessageToLog( struct Server* srv, struct Message** msg )
{
        while ( srv->log_lock == 1 )
        {
                sleep( 1 );     
        }

        srv->log_lock = 1;
        srv->log = realloc( srv->log, ++srv->log_sz * sizeof(struct Message*) );
        srv->log[srv->log_sz - 1] = *msg;
        srv->log_lock = 0;
}

/**
 * \fn void thread_loop()
 * \brief Loop for handling messages from a given client
 * Handles a given client socket's messages. Run in a separate thread from the 
 * main thread. Once LEAVE message is received, close thread.
 *
 * \param srv Server struct to use
 * \param client_sock Which client to handle
 */
static void* thread_loop( void* params )
{
        struct ClientThread* ct = (struct ClientThread*)params;
        char* username = NULL;
        unsigned int last_log_sz = 0;

        int has_closed_conn = 0;
        while ( !has_closed_conn )
        {
                // read message
                struct Message* req = sockw_read( ct->sockw );
                
                struct Message* msg;
                // execute message
                switch ( msg_getCommand( req ) )
                {
                        case JOIN:
                                username = msg_getBody( req );
                                msg = mangleMessage( username, "has joined." );
                                addMessageToLog( ct->srv, &msg );
                                break;
                        case LEAVE:
                                has_closed_conn++;
                                msg = mangleMessage( username, "has left." );
                                addMessageToLog( ct->srv, &msg );
                                break;
                        case WRITE:
                                msg = mangleMessage( username, msg_getBody( req ) );
                                addMessageToLog( ct->srv, &msg );
                                break;
                        default:
                                break;
                }

                // check for new messages to send
                while ( last_log_sz < ct->srv->log_sz )
                {
                        sockw_write( ct->sockw, ct->srv->log[last_log_sz] );
                        fprintf( stderr, "%i: Wrote message to client at index %i\n", 
                                 sockw_getsocknum( ct->sockw ), last_log_sz );
                        last_log_sz++;
                }
        }
        pthread_t tid = pthread_self();
        sockw_shutdown( ct->sockw );
        free( ct );
        pthread_cancel( tid );

        return NULL;
}

int main( void )
{
        struct Server srv;
        srv.log = NULL;
        srv.log_sz = 0;

        srv.srv_sock = sockw_socket();
        if ( sockw_bind( srv.srv_sock, DEFAULT_PORT ) == 0 )
        {
                sockw_listen( srv.srv_sock );

                while (1)
                {
                        struct Socket* client_sock = sockw_accept( srv.srv_sock );
                        if ( client_sock != NULL )
                        {
                                //package arguments into ClientThread struct
                                struct ClientThread* ct = malloc( sizeof(struct ClientThread) );
                                ct->srv = &srv;
                                ct->sockw = client_sock;

                                pthread_t thread_id;
                                pthread_create( &thread_id, NULL, &thread_loop, (void*)ct);
                        }
                }

                
        }
        sockw_shutdown( srv.srv_sock );   

        return 0;
}
