#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>

#define MSG_MAX_SZ ( 64 )

enum Command
{
        JOIN,           //join server, body is nickname
        LEAVE,          //leave server, body is blank
        WRITE,          //send message to chatroom, body is message
        RECEIVE,        //(server only) receive new message, body is message
        ERROR           //error, body is detail about error (if possible)
};

struct Message;

struct Message* msg_init( enum Command, char*, uint8_t );

void msg_free( struct Message* );

enum Command msg_getCommand( struct Message* );

char* msg_getBody( struct Message* );

uint8_t msg_getSize( struct Message* );

char* msg_serialize( struct Message* );

struct Message* msg_deserialize( char* );
#endif
