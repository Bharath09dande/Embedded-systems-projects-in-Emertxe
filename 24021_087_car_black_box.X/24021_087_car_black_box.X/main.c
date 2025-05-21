/*header files*/
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "uart.h"
#include "i2c.h"
#include "ds1307.h"
#include "at24c04.h"

/*variables*/
State_t state; //to change the functions
unsigned char key; //to detect key from the keypad
char gears[10][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C_"}; //gears 2-D array
char menu[4][16] = {"View log", "Clear log", "Download log", "Set time"}; //menu 2-D array
char data[10][16]; //to store the data from the EEPROM
char menu_index = 0, gear_index = 0; //indexs to handle the 2-D array
char flag = 1, flag1 = 0, flagER = 1; //flags to control the program
unsigned short adc_reg_val; //variable to store the ADC value return by the read_adc function
unsigned char address = 0x00; //variable used for to store the data in the EEPROM
unsigned char no_of_events = 0; //to count the events stored in the EEPROM
unsigned char clock_reg[3];//to storing time from RTC (integers)
unsigned char time[9];//to storing time clock_reg (characters)
unsigned long int delay;

void init_config() {
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_uart();
    init_i2c();
	init_ds1307();
    state = e_dashboard;
}

void main(void) {
    init_config();
    while (1) {
        get_time();
        key = read_switches(STATE_CHANGE);
        switch (state) {
            case e_dashboard:
                view_dashboard();
                break;
            case e_main_menu:
                display_main_menu();
                break;
            case e_view_log:
                view_log();
                break;
            case e_download_log:
                download_log();
                break;
            case e_clear_log:
                clear_log();
                break;
            case e_set_time:
                set_time();
                break;
        }
    }
}

void view_dashboard(void) { 
    clcd_print("  TIME   GEAR SP", LINE1(0));/*header*/
    display_time();/*to display the time*/
    clcd_print(gears[gear_index], LINE2(10));/*initial in ON state*/
    /*gear box changing*/
    if (key == MK_SW3) {/*collision*/
        gear_index = 8;
        event_store(time, gears[gear_index], adc_reg_val);
    }
    if (key == MK_SW1) {/*increasing gear+*/
        if (gear_index < 7) {
            gear_index++;
            event_store(time, gears[gear_index], adc_reg_val);
        }   
    } else if (key == MK_SW2) {/*decreasing gear*/
        if (gear_index > 1) {
            gear_index--;
            event_store(time, gears[gear_index], adc_reg_val);
        }
    }
    /*speed changing*/
    adc_reg_val = read_adc(CHANNEL4) / 10.33; //to get value upto 0-99
    clcd_putch((adc_reg_val / 10) + 48, LINE2(14)); //tens
    clcd_putch((adc_reg_val % 10) + 48, LINE2(15)); //ones
    /*to go main_menu function*/
    if (key == MK_SW11) {
        state = e_main_menu;
        key = ALL_RELEASED;
        CLEAR_DISP_SCREEN;
    }
}
//9//
void event_store(unsigned char *time, unsigned char gear[], unsigned int speed) {
    /*to store ten events*/
    no_of_events++;
    if (no_of_events > 10) {
        no_of_events = 10;
        unsigned char address1 = 0x00;
        unsigned char address2 = 10;
        unsigned char data_ew = 0;
        for(int i=0;i < 9;i++)
        {
//            data_ew = 0;
            for(int j = 0;j < 10;j++)
            { 
                data_ew  = read_external_EEPROM(address2++);
                write_external_EEPROM(address1++,data_ew);
            }
        }
        address = 90;
    }
    for (int i = 0; i < 8; i++) {//except the semi-colon
        if (i != 2 && i != 5) {//to avoid ':'
            write_external_EEPROM(address++, time[i]);
        }
    }
    /*to store the gears*/
    write_external_EEPROM(address++, gears[gear_index][0]);
    write_external_EEPROM(address++, gears[gear_index][1]);
    /*to store the speed*/
    write_external_EEPROM(address++, (speed / 10) + '0');
    write_external_EEPROM(address++, (speed / 10) + '0');
}

void display_main_menu() {
    /*to display the star*/
    if (flag1 == 0) {
        clcd_putch('*', LINE1(0));
        clcd_putch(' ', LINE2(0));
    } else if (flag1 == 1) {
        clcd_putch('*', LINE2(0));
        clcd_putch(' ', LINE1(0));
    }
    /*to display the menu*/
    clcd_print(menu[menu_index], LINE1(2));
    clcd_print(menu[menu_index + 1], LINE2(2));
    /*scrolling up*/
    if (key == MK_SW2) {
        if (flag1 == 0)
            flag1 = 0;
        else if (menu_index < 2) {
            CLEAR_DISP_SCREEN;
            menu_index++;
        }
        flag1 = 1;
    }
    /*scrolling down*/
    if (key == MK_SW1) {
        if (flag1 == 1)
            flag1 = 1;
        else if (menu_index > 0) {
            menu_index--;
            CLEAR_DISP_SCREEN;
        }
        flag1 = 0;
    }
    /*to go to view dash board function*/
    if (key == MK_SW12) {
        state = e_dashboard;
        CLEAR_DISP_SCREEN;
    }
    /*to go to respective log functions*/
    if (key == MK_SW11) {
        state = menu_index + 2 + flag1;
        CLEAR_DISP_SCREEN;
    }
}

void view_log() {
    /*NO events present*/
    if (no_of_events == 0) {
        CLEAR_DISP_SCREEN;
        clcd_print("NO LOGS         ", LINE1(0));
        clcd_print("AVAILABLE :-(   ", LINE2(0));
        for (unsigned long int delay = 500000; delay--;);
        /*going to main_menu function*/
        state = e_main_menu;
        flag = 1;
        CLEAR_DISP_SCREEN;
        return;
    }
    static int index = 0;
    /*calling event_reader function only one time for storing data from the EEPROM*/
    if (flagER == 1) {
        event_reader();
        flagER = 0;
    }
    /*header*/
    clcd_print("# TIME     EV SP", LINE1(0));
    /*Upwards*/
    if (key == MK_SW1 && index > 0) {
        index--;
        clcd_print("                ", LINE2(0));
    }
    /*Downwards*/
    if (key == MK_SW2 && index < no_of_events - 1) {
        index++;
        clcd_print("                ", LINE2(0));
    }
    /*to go to view_dashboard function*/
    if (key == MK_SW12) {
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
        index = 0;
        flagER = 1;
        return;
    }
    /*serial number*/
    clcd_putch(index + '0', LINE2(0));
    /*to display the events stored in the EEPROM by using 2-D array*/
    clcd_print(data[index], LINE2(2));
}

void clear_log(void) {
    clcd_print("ALL LOGS        ", LINE1(0));
    clcd_print("CLEARED :-)     ", LINE2(0));
    if (delay++ == 500) {
        delay = 0;
        address = 0x00;
        no_of_events = 0;
        CLEAR_DISP_SCREEN;
        state = e_main_menu;
    }
}

void event_reader(void) {
    char read_address = 0x00;
    unsigned short i, j;
    /*To store the data from EEPROM to 2-D array*/
    for (i = 0; i < no_of_events; i++) {
        for (j = 0; j < 14; j++) {
            if (j == 2 || j == 5) {
                data[i][j] = ':';
            } else if (j == 8 || j == 11) {
                data[i][j] = ' ';
            } else {
                data[i][j] = read_external_EEPROM(read_address++);
            }
        }
        data[i][j] = '\0';
    }
}

void download_log(void)
{
    int index = 0;
    char log[11];
    unsigned char add=0;
    if (no_of_events == 0) /* if no logs available */
    {
        clcd_print("NO LOGS         ",LINE1(0));
        clcd_print("TO DOWNLOAD :-( ",LINE2(0));
        puts("No logs available");
        putch('\n');
        putch('\r');
        for(unsigned long int delay1 = 1000000;delay1--;);
    }
    else /* displaying all logs available */
    {
        puts("Logs :");
        putch('\n');
        putch('\r');
        puts("#     Time        Event       Speed");
        putch('\n');
        putch('\r');

        while (index < no_of_events)
        {
            for (int i = 0; i < 10; i++)
            {
                log[i] = read_external_EEPROM(add++); /* reading the event data */
            }
            /* printing index */
            putch(index + '0');
            puts("   ");
            /*printing hours value */
            putch(log[0]);
            putch(log[1]);
            putch(':');
            /*printing minute  value */
            putch(log[2]);
            putch(log[3]);
            putch(':');
            /*printing second  value */
            putch(log[4]);
            putch(log[5]);
            puts("      ");
            /*printing event  character  */
            putch(log[6]);
            putch(log[7]);
            puts("            ");
            /*printing speed value */
            putch(log[8]);
            putch(log[9]);
            putch('\n');
            putch('\r');
            index++; /*incrementing to next log */
        }
        clcd_print("LOGS DOWNLOADED ",LINE1(0));
        clcd_print("SUCCESSFULLY :-)",LINE2(0));
        for(unsigned long int delay1 = 1000000;delay1--;);
    }
    state = e_main_menu;
    CLEAR_DISP_SCREEN;
}

void set_time(void)
{
    static unsigned int new_time[3];
    static unsigned int blink_pos = 0;
    static unsigned char blink_delay = 0,once = 0;
    char buffer;
    /*storing time from RTC*/
    if (once == 0)
    {
        once = 1;
        get_time();
        /*Storing time*/
        new_time[0] = (time[0] - '0') * 10 + (time[1] - '0');/* hours */
        new_time[1] = (time[3] - '0') * 10 + (time[4] - '0');/* minutes */
        new_time[2] = (time[6] - '0') * 10 + (time[7] - '0');/* second */
    }
    /*HH:MM:SS header on clcd*/
    clcd_print(" HH:MM:SS       ", LINE1(0));
    switch(key)
    {
        case MK_SW1:/*increment time*/
            new_time[blink_pos]++;
            break;
        case MK_SW2:/*changing blink position*/
            blink_pos = (blink_pos + 1) % 3;
            break;
        case MK_SW11:/* storing the new time and exit */
            get_time(); /* get the time */
            /*Hours*/
            buffer = ((new_time[0] / 10) << 4) | new_time[0] % 10; 
            clock_reg[0] = (clock_reg[0] & 0xC0) | buffer; 
            write_ds1307(HOUR_ADDR, clock_reg[0]); 
            /*Minutes*/
            buffer = ((new_time[1] / 10) << 4) | new_time[1] % 10; 
            clock_reg[1] = (clock_reg[1] & 0x80) | buffer; 
            write_ds1307(MIN_ADDR, clock_reg[1]); 
            /*Seconds*/
            buffer = ((new_time[2] / 10) << 4) | new_time[2] % 10; 
            clock_reg[2] = (clock_reg[2] & 0x80) | buffer;
            write_ds1307(SEC_ADDR, clock_reg[2]);
            CLEAR_DISP_SCREEN;
            clcd_print("Time changed    ", LINE1(0));
            clcd_print("Successfully :-)", LINE2(0));
            for(unsigned long int delay1 = 1000000;delay1--;);
            CLEAR_DISP_SCREEN;
            state = e_dashboard;
            break;
        case MK_SW12:
            CLEAR_DISP_SCREEN;
            state = e_dashboard;
            break;
    }
    /* conditions for roll over */
    if (new_time[0] > 23)
        new_time[0] = 0;
    if (new_time[1] > 59)
        new_time[1] = 0;
    if (new_time[2] > 59)
        new_time[2] = 0;
    /*logic to blink at the current pos*/
    if(blink_delay < 50)
    {
        /* Displaying hours field */
        clcd_putch(new_time[0] / 10 + '0', LINE2(1));
        clcd_putch(new_time[0] % 10 + '0', LINE2(2));
        clcd_putch(':', LINE2(3));
        /* Displaying mins field */
        clcd_putch(new_time[1] / 10 + '0', LINE2(4));
        clcd_putch(new_time[1] % 10 + '0', LINE2(5));
        clcd_putch(':', LINE2(6));
        /* Displaying secs field */
        clcd_putch(new_time[2] / 10 + '0', LINE2(7));
        clcd_putch(new_time[2] % 10 + '0', LINE2(8));
        blink_delay++;
    }
    else if(blink_delay < 1000)
    {
        if(blink_pos == 0){
            clcd_print("\xFF\xFF", LINE2(1));
        }else if(blink_pos == 1){
            clcd_print("\xFF\xFF", LINE2(4));
        }else if(blink_pos == 2){
            clcd_print("\xFF\xFF", LINE2(7));
        }
        blink_delay++;
    }
    else
    {
        blink_delay = 0;
    }
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);
    /*Hours*/
	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
    /*Sec*/
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
    /*Min*/
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void display_time(void)
{
    get_time(); /* get the time */
    clcd_print(time, LINE2(1));/* Displaying the time in HH:MM:SS format */
}