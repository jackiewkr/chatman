# Chatman

A simple client-server chatroom program written in C and using TCP sockets.

## Motivation

The idea behind this project is to understand how TCP/IP works when programming
C and to provide a simple program that shows this off. This project has helped me
understand POSIX threads, thread synchronization and ore.

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

## How to use
Compile the client with `make cli` and the server with `make server`.
By default the server is set to run on port 8000, so check if anything is running
on that port beforehand. When using the client, you can type a message and press
enter to send the message to the server. The first message sent will always be
your username.
