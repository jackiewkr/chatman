#include "message.h"
#include "sockwrapper.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <curses.h>
#include <string.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 8000
#define BUF_LEN 32
#define MAX_MSGS 32

unsigned int should_quit = 0;
unsigned int rows = 0;
unsigned int cols = 0;

static int is_printable( char ch )
{
        if ( ch < 32 || ch > 126 )
                return 0;
        return 1;
}

/**
 * \fn void enter_ncurses()
 * \brief Set up ncurses stuff.
 */
void enter_ncurses( void )
{
        initscr();
        noecho();
        keypad( stdscr, TRUE );
        halfdelay( 10 );    
}

/**
 * \fn void exit_ncurses()
 * \brief Tear down ncurses stuff.
 */
void exit_ncurses( void )
{
        echo();
        nocbreak();
        endwin();
}

void draw_chatbox( WINDOW* chatbox_win, char text_buf[], unsigned int buf_idx )
{
        wclear( chatbox_win );

        wmove( chatbox_win, 0, 0 );
        wprintw( chatbox_win, "> " );
        for ( int i = 0; i < buf_idx; i++ )
        {
                wprintw( chatbox_win, "%c", text_buf[i] );
        }
        wrefresh( chatbox_win );
}

void handle_requests( WINDOW* log_win, struct Socket* server_sock )
{
        struct Message** msgs = malloc( sizeof(struct Message*));
        unsigned int msg_idx = 0;

        struct Message* req;
        while ( (req = sockw_read( server_sock ) ) != NULL )
        {
                msgs = realloc( msgs, sizeof(struct Message*) * msg_idx + 1 );
                msgs[msg_idx++] = req;
        }

        //draw messages onto screen
        for ( int i = 0; i < msg_idx; i++ )
        {
                wprintw( log_win, "%s\n", msg_getBody(msgs[i]) );
                wrefresh( log_win );
        }
}

int main( void )
{
        // try to connect to server
        struct Socket* server_sock = sockw_socket();
        if ( sockw_connect( server_sock, SERVER_IP, SERVER_PORT ) == 0 )
        {
                // start ncurses stuff
                enter_ncurses();
                getmaxyx( stdscr, rows, cols );

                WINDOW* chatbox_win = newwin( 0, 0, rows - 1, 0 );
                WINDOW* log_win = newwin( rows - 1, cols, 0, 0 );
                scrollok( log_win, TRUE );
                
                int ch;
                char text_buf[BUF_LEN] = "";
                unsigned int buf_idx = 0;
                unsigned int has_username = 0;
                while ( !should_quit )
                {
                        // update window dims
                        getmaxyx( stdscr, rows, cols );
                        draw_chatbox( chatbox_win, text_buf, buf_idx );

                        // check key press
                        ch = getch();
                        if ( is_printable( ch ) && buf_idx < BUF_LEN )
                        {
                                text_buf[buf_idx++] = (char)ch;
                        } 
                        else if ( ch == '\n' )
                        {
                                // if no username set, interpret first msg as
                                // username
                                if ( !has_username )
                                {
                                        sockw_write( server_sock,
                                                     msg_init( JOIN, text_buf,
                                                               buf_idx ) );
                                        has_username++;
                                }
                                else
                                {
                                        sockw_write( server_sock,
                                                     msg_init( WRITE, text_buf,
                                                               buf_idx ) );
                                }

                                memset( text_buf, '\0', BUF_LEN );
                                buf_idx = 0;

                        }
                        else if ( ch == KEY_F(10) )
                        {
                                should_quit++;
                        }
                        else if ( ch == KEY_BACKSPACE )
                        {
                                text_buf[buf_idx--] = ' ';
                        }

                        // do socket communication
                        handle_requests( log_win, server_sock );

                }
                exit_ncurses();
        }
        else
        {
                fprintf( stderr, "ERROR: Could not connect to %s:%i\n",
                         SERVER_IP, SERVER_PORT );
        }
        // close socket
        sockw_shutdown( server_sock );
        return 0;
}

#if 0
// send join with randomized username
                srand( time( NULL ) );
                int r = rand();
                char* username = malloc(16);
                snprintf( username, 16, "Doe%i", r );
                sockw_write( server_sock, msg_init( JOIN, username, 16 ) );
                sockw_write( server_sock, msg_init( WRITE, "Hello, World!", 
                                          13 ) );
                while (!should_quit)
                {
                        // print received messages if any exist
                        struct Message* req;
                        while ( ( req = sockw_read( server_sock ) ) != NULL )
                        {
                                fprintf( stderr, "%s\n", msg_getBody( req ) );
                        }
                }

                // send leave msg
                sockw_write( server_sock, msg_init( LEAVE, "", 0 ) );

#endif
