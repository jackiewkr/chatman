#include <stdint.h>

struct Message
{
        char* msg;
        uint8_t size;
};

struct Message* msg_init( char* msg, uint8_t size )
{
        struct Message* msg = malloc( sizeof(struct Message) );

        if (msg != NULL)
        {
                msg->msg = malloc( size );
                memcpy( msg->msg, msg, size );

                msg->size = size;
        }

        return msg;
}

void msg_free( struct Message* msg )
{
        free( msg->msg );
        free( msg );
}


char* msg_get( struct Message* msg )
{
        if ( msg != NULL )
                return msg->msg;
        return msg;
}

uint8_t msg_getSize( struct Message* msg )
{
        if ( msg != NULL )
                return msg->size;
        return 0;
}
