#include <ncurses.h>
#include <time.h>
#include <string.h>
int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    time_t timeinfo;
    // Create the window
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW* win = newwin(3, width-4, height-4, 2);

    // Print the variable sized "time" text
    time_t rawtime;
    struct tmtimeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char buffer[80];
    strftime(buffer, 80, "%T", timeinfo);
    mvwprintw(win, 1, (width-strlen(buffer))/2, "%s", buffer);
    // Show the window
    wrefresh(win);
    getch();

    // End ncurses
    endwin();
    return 0;
}