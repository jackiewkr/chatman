#ifndef SOCKWRAPPER_H_
#define SOCKWRAPPER_H_

/**
 * \file sockwrapper.h
 * \brief Wrapper over TCP sockets to provide client-server communication.
 *
 * Uses "sys/socket.h" and related libraries to provide communication of 
 * messages using a message struct. Hides implementation details of TCP sockets
 * behind simple function calls.
 */

struct Socket;

/**
 * \fn struct Socket* sockw_socket()
 * \brief Creates a TCP socket.
 *
 * Creates a TCP socket with AF_INET and SOCK_STREAM set. Wrapper for socket()
 * and setsockopt().
 * Returns a pointer to a Socket struct.
 */
struct Socket* sockw_socket( void );

/**
 * \fn int sockw_bind()
 * \brief Binds a IPv4 address and port to the socket
 * Wrapper for bind().
 *
 * \param sockw The socket to bind with
 * \param port The port to use
 *
* Returns 0 on success, -1 on failure.
 */
int sockw_bind( struct Socket* sockw, int port );

/**
 * \fn void sockw_listen()
 * \brief Listens for any requests to establish a connection.
 * Wrapper for listen()
 *
 * \param sockw Socket to listen on
 */
void sockw_listen( struct Socket* sockw );

/**
 * \fn void sockw_connect()
 * \brief Connect to a socket with goven address and port.
 * Wrapper for connect().
 *
 * \param sockw The socket to use
 * \param address The IPv4 address to use as a char buffer (no \0)
 * \param port The port to use
 *
 * Returns 0 on successful connection
 */
int sockw_connect( struct Socket* sockw, char* address, int port );

/**
 * \fn struct Socket* sockw_accept()
 * \brief Establishes a connection between client and server.
 * Wrapper for accept().
 *
 * \param sockw Socket to use when accepting
 *
 * Returns a pointer to a Socket struct, representing the client.
 */
struct Socket* sockw_accept( struct Socket* sockw );

/**
 * \fn struct Message* sockw_read()
 * \brief Read a message from a socket.
 * Wrapper for recv(). 
 *
 * \param sockw Socket to read from
 *
 * Returns a pointer to a Message struct.
 */
struct Message* sockw_read( struct Socket* sockw );

/**
 * \fn void sockw_write()
 * \brief Write a message to a socket.
 * Wrapper for send().
 *
 * \param sockw Socket to write to
 * \param msg Message to send
 */
void sockw_write( struct Socket* sockw, struct Message* msg );

/**
 * \fn sockw_shutdown()
 * \brief Closes a socket.
 * Wrapper for close().
 *
 * \param sockw Socket to close.
 */
void sockw_shutdown( struct Socket* sockw );


#endif
