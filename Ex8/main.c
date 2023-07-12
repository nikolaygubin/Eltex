#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

#include <assert.h>

#define CNT_BYTE_READ 20

extern WINDOW *stdscr;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void reverse(char* str)
{
    // получаем длину строки
    int n = strlen(str);
 
    // начинаем менять местами символы с обоих концов строки
    for (int i = 0, j = n - 1; i < j; i++, j--)
    {
        char ch = str[i];
        str[i] = str[j];
        str[j] = ch;
    }
}

void read_file(int file_descriptor, char *buf) {

    char buffer_for_read[256];

    int res = read(file_descriptor, buffer_for_read, CNT_BYTE_READ);

    strncpy(buf, buffer_for_read, res);
    buf[res] = '\0';
}

void draw_file(int file_descriptor) {
    while(1) {
        char buf[256];
        read_file(file_descriptor, buf);
        
        printw("%s", buf);
        if (buf[0] == '\0') break;
    }

}

void save_file(FILE *fp, int file_descriptor) {

    lseek(file_descriptor, 0, SEEK_SET);
    int max_y = 0, max_x = 0;
    getmaxyx(stdscr, max_y, max_x);
    // fprintf(fp, "max_y: %d\nmax_x: %d\n",  max_y, max_x);
    
    for (int i = 0; i <= max_y; i++) {
        char buf[1024];
        buf[0] = '\0'; // строка будет идти в обратном порядке
        int counter_LF = 0, index = 0; // счетчик, означает, что нужно поставить \n

        for (int j = max_x; j >= 0; j--) {
            char tmp_ch = mvinch(i, j);

            if (tmp_ch == ' ' && 0 == counter_LF) {
                continue;
            } else if (tmp_ch >= 32 && tmp_ch < 127) {
                counter_LF = 1;
                buf[index++] = tmp_ch;
            }


        }
        if (counter_LF == 0) {
            // buf[index++] = '\n'; 
            fprintf(fp, "\n");
            write(file_descriptor, "\n", 1);

        } else {
            buf[index] = '\0';
            reverse(buf);
            buf[index++] = '\n';
            buf[index] = '\0'; 

            if (strlen(buf) > 0) {
                // fprintf(fp, "|");
                fprintf(fp, "%s", buf);
                write(file_descriptor, buf, strlen(buf));

            }
        }
    }
}






void print_menu(WINDOW *MENU_WIND, WINDOW *MENU_BORDER) {

    box(MENU_BORDER, '|', '-');
    wprintw(MENU_WIND, "F(1): save file, F(2): open/create file, F(3): exit\n");
    wrefresh(MENU_BORDER);

}

void input_filename_on_window(WINDOW *INPUT_FILE) {
    wprintw(INPUT_FILE, "Enter(F4) filename for open/create file: ");

    wrefresh(INPUT_FILE);
}



int print_file(char *filename) {

    attron(A_BOLD);
    int file_descriptor = open(filename, O_CREAT|O_RDWR, S_IRWXU);
    draw_file(file_descriptor);


    attroff(A_BOLD);
    refresh();

    return file_descriptor;
}


int write_scr(FILE *fp, int file_descriptor, WINDOW *MENU_WIND, WINDOW *MENU_BORDER) {
    // int status_new_file = 1, status_exit = 0;
    int status = 0;
    
    int ch;
    int y = 0, x = 0;
    int flag_break = 0;
    wmove(stdscr, y, x);
    while( (ch = wgetch(stdscr)) != KEY_F(3)) {
        getyx(stdscr, y, x);        

        switch (ch) {
        case KEY_BACKSPACE:
            wdelch(stdscr);
            // обработать \n, а точнее удаление строки
            break;

        case KEY_LEFT:
            wmove(stdscr, y, x - 1);
            break;

        case KEY_RIGHT:
            wmove(stdscr, y, x + 1);
            break;

        case KEY_UP:
            wmove(stdscr, y - 1, x);
            break;

        case KEY_DOWN:
            wmove(stdscr, y + 1, x);
            break;

        case KEY_F(1):
            save_file(fp, file_descriptor);
            break;

        case KEY_F(2):
            flag_break = 1;
            status = 1;
            break;

        case KEY_F(3):
            flag_break = 1;
            break;
        // case ENTER - обработать бы перенос строки
        default:
            wechochar(stdscr, ch);
            break;
        }

        if (flag_break) break;
    }

    close(file_descriptor);

    return status;
}

int write_scr_input_file(WINDOW *INPUT_FILE, char *filename) {
    
    int status = 0;    
    int ch;
    int y = 0, x = 41;
    int flag_break = 0;
    unsigned int index = 0;
    wmove(INPUT_FILE, y, x);
    while(1) {

        ch = wgetch(INPUT_FILE);
        getyx(INPUT_FILE, y, x);        

        switch (ch) {
        case KEY_BACKSPACE:
            wdelch(INPUT_FILE);
            // обработать \n, а точнее удаление строки
            // !!! for filename need to do !!!
            break;

        case KEY_LEFT:
            wmove(INPUT_FILE, y, x - 1);
            if (index > 0) index--;
            break;

        case KEY_RIGHT:
            wmove(INPUT_FILE, y, x + 1);
            index++;
            break;

        case KEY_F(3):
            flag_break = 1;
            break;
        
        case KEY_F(4): // KEY_ENTER :(
            flag_break = 1;
            status = 1;
            break;
        default:
            wechochar(INPUT_FILE, ch);
            filename[index++] = ch;
            break;
        }

        if (flag_break) break;
    }

    return status;
}

int main(int argc, char ** argv) {

    WINDOW *MENU_WIND, *MENU_BORDER, *INPUT_FILE;
    FILE *fp = fopen("OUTPUT.txt", "w"); // для отладки
    initscr();
    
    signal(SIGWINCH, sig_winch);
    cbreak();
    noecho();
    curs_set(TRUE);
    keypad(stdscr, TRUE); // Для обработки esc спец символов
    // idlok(stdscr, TRUE);
    // scrollok(stdscr, TRUE);
    // scroll(stdscr);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    MENU_BORDER = newwin(4, max_x, max_y - 4, 0);
    MENU_WIND = derwin(MENU_BORDER, 1, max_x - 2, 1, 1);
    INPUT_FILE = derwin(MENU_BORDER, 1, max_x - 2, 2, 1);

    keypad(INPUT_FILE, TRUE);

    char filename[256] = "temp.out";
    // можно сразу начинать с открытия файла, н-р, через аргументы

    while(1) {
        int status_input = -1;
        int file_descriptor = print_file(filename);
        print_menu(MENU_WIND, MENU_BORDER);

        int status = write_scr(fp, file_descriptor, MENU_WIND, MENU_BORDER);
        
        if (0 == status) {
            break;
        } else if (1 == status) {
            input_filename_on_window(INPUT_FILE);
            status_input = write_scr_input_file(INPUT_FILE, filename);
        }
        wclear(MENU_WIND);
        wclear(INPUT_FILE);
        werase(stdscr); 
        //  werase не удаляет стр, а заполняет пробелами
        // Проблема, 

        if (0 == status_input) break;

    }


    delwin(MENU_WIND);
    delwin(MENU_BORDER);
    delwin(INPUT_FILE);


    fclose(fp);
    
    endwin();
    exit(EXIT_SUCCESS);
    
    return 0;
}
