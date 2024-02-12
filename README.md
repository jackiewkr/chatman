# Chatman

A simple client-server chatroom program written in C and using TCP sockets.

## Motivation

The idea behind this project is to understand how TCP/IP works when programming
C and to provide a simple program that shows this off. The chatroom part
was chosen to be a easy enough program to write in a few days, while still being 
impressive to show off.

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
