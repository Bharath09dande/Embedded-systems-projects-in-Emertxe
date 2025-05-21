#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>

/* Enum for maintaining the app state */
typedef enum {
    e_dashboard, e_main_menu, e_view_log, e_clear_log, e_download_log, e_set_time
} State_t;


extern State_t state; // App state

/*Function declarations*/

//Dashboard function declaration
void view_dashboard(void);

//Storing events function declaration
void event_store(unsigned char *time,unsigned char gear[],unsigned int speed);

//main menu function declaration
void display_main_menu(void);

//View log function declaration
void view_log(void);

//Reading events function declaration
void event_reader(void);

//Set time function declaration
void set_time(void);

//Download log function _decleration
void download_log(void);

//Clear log function declaration
void clear_log(void);

static void get_time(void);

void display_time(void);
#endif	/* MAIN_H */

