# Chatman

A simple proof-of-concept client-server chatroom program written in C and using TCP sockets.

## Motivation

The idea behind this project is to understand how TCP/IP works when programming
C and to provide a simple program that shows this off. This project has helped me
understand how to setup TCP sockets and how to use basic threading in C.

## Requirements

The chatroom application must have both a client program and a server program.
The client program must support this functionality:
 - Send chat message to server
 - Receive chat messages from server and display to user
 - Show clients connected to server via some form of nickname

The server must support this functionality:
 - Receive chat messages
 - Send chat messages to clients 
 - Store client nicknames
 - Keep a chronological log of the chat messages for future use
 - Notify users joining or leaving

## Issues

The current program sometimes corrupts messages when sending to/from client or server. This
could be fixed by including some form of checksum with the sent message, along with ensuring
any text is sanitised and that no memory is leaked when sending or receiving a message.

## Future Work

It would be nice to extend the functionality of the client to accept any running server,
rather than the one which is hardcoded into the program. The server could do with some
modification, namely that the log of messages is allocated as a continuous segment of memory
with no limit, instead of something more space-efficient such as a linked list with a fixed
size that prunes older messages.

## How to use
Compile the client with `make cli` and the server with `make server`.
By default the server is set to run on port 8000 and the client is set to connect to localhost,
so check if anything is running on that port beforehand. When using the client, you can type 
a message and press enter to send the message to the server. The first message sent will always
be your username.
