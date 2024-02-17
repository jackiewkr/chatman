#include "message.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Message
{
        enum Command comm;
        char* body;
        uint8_t size;
};

struct Message* msg_init( enum Command comm, char* message, uint8_t size )
{
        struct Message* msg = malloc( sizeof(struct Message) );

        if (msg != NULL)
        {
                msg->body = malloc( size );
                memcpy( msg->body, message, size );

                msg->size = size;
                msg->comm = comm;
        }

        return msg;
}

void msg_free( struct Message* msg )
{
        free( msg->body );
        free( msg );
}


char* msg_getBody( struct Message* msg )
{
        if ( msg != NULL )
                return msg->body;
        return NULL;
}

enum Command msg_getCommand( struct Message* msg )
{
        if ( msg != NULL )
                return msg->comm;
        return ERROR;
}

uint8_t msg_getSize( struct Message* msg )
{
        if ( msg != NULL )
                return msg->size;
        return 0;
}

char* msg_serialize( struct Message* msg )
{
        char* serialized = malloc( MSG_MAX_SZ );
        snprintf( serialized, MSG_MAX_SZ, "%i%s", msg->comm, msg->body );
        fprintf( stderr, "%s\n", serialized );
        return serialized;
}

struct Message* msg_deserialize( char* serialized )
{
        //TODO: make not assume body starts at [2]
        enum Command comm = (enum Command)(serialized[0] - '0');
        return msg_init( comm, &serialized[1], strlen( &serialized[1] ) );
}
