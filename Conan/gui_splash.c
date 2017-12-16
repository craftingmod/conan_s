#include <stdio.h>
#include "tui.h"
#include "playm.h"
#include "fmod.h"
#include "config_conan.h"

void splash();
int WaitForUser(void);

void splash() {
	char Logo[][30] =
	{
		"   ###            #   ",
		"  #   #  #    #  ##   ",
		" #     #  #  #  # #   ",
		" #     #   ##     #   ",
		" #     #   ##     #   ",
		"  #   #   #  #    #   ",
		"   ###   #    # ##### ",
		""
	};
	char* titlero = "       Conan - The Music Defective         ";

	FMOD_SYSTEM *system;
    FMOD_SOUND *sound;

    playm_createSystem(&system);
    playm_playSound(system,&sound,music_intro,0);


	WINDOW *win;
	chtype  ch;
	int width, height, x, y, i;

	initscr();

	start_color();

	cbreak();
	noecho();

	curs_set(0);

	noecho();

	/* refresh stdscr so that reading from it will not cause it to
	overwrite the other windows that are being created */

	refresh();

	/* Create a drawing window */

	width = 48;
	height = 15;

	win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);

	if (win == NULL)
	{
		endwin();

		return;
	}

	while (1) {
		werase(win);
		wrefresh(win);


		/* Draw RED bounding box */

		wattrset(win, COLOR_PAIR(7));
		box(win, ' ', ' ');
		wrefresh(win);

		/* Display Australia map */

		wattrset(win, A_BOLD);
		i = 0;

		napms(250);
		while (*Logo[i])
		{
			mvwaddstr(win, i + 3, 13, Logo[i]);
			wrefresh(win);
			napms(100);
			++i;
		}

		napms(1100);
		init_pair(5, COLOR_BLACK, COLOR_WHITE);
		wattrset(win, COLOR_PAIR(5) | A_BLINK);
		mvwaddstr(win, height - 2, 3,
			titlero);
		//mvaddstr(win, height -3, 4, appversion);
		wrefresh(win);
		while(1){
			if (WaitForUser() >= 0){
				break;
			}
			break;
		}
		break;
	}
	playm_stopSound(sound);
    playm_destorySystem(system);

	endwin();
}
int WaitForUser(void)
{
	chtype ch;

	nodelay(stdscr, TRUE);
	halfdelay(50);

	ch = getch();

	nodelay(stdscr, FALSE);
	nocbreak();     /* Reset the halfdelay() value */
	cbreak();

	return ch;
}
