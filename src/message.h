#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>

struct Message;

struct Message* msg_init( char*, uint8_t );

void msg_free( struct Message* );

char* msg_get( struct Message* );

uint8_t msg_getSize( struct Message* );
#endif
