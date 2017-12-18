// Conan.c: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "gui_splash.h"
#include "config_conan.h"

#include "tui.h"
#include "play.h"
#include "appinfo.h"
#include "playm.h"

/* change this if source at other location */

/***************************** forward declarations ***********************/

void m_play(void);
void m_info(void);


/***************************** menus initialization ***********************/

menu MainMenu[] =
{
	{ "Home", mainScreen, "Main Menu" },
	{ "Play", m_play, "Play Conan" },
	{ "AppInfo", m_info, "Conan Application Information" },
	{ "", (FUNC)0, "" }   /* always add this as the last item! */
};

menu Sub_Play[] =
{
	{ "Play", initPlay, "Play Conan" },
	{ "Exit", DoExit, "Terminate program" },
	{ "", (FUNC)0, "" }
};

menu Sub_Info[] =
{
	{ "About Conan", AppInfo, "Application Information" },
	{ "About Team 0x1", AboutTeam, "Team Information" },
	{ "", (FUNC)0, "" }
};

/***************************** main menu functions ************************/

void m_play(void)
{
	domenu(Sub_Play);
}

void m_info(void)
{
	domenu(Sub_Info);
}

/***************************** start main menu  ***************************/

int main(int argc, char **argv)
{
	//setlocale(LC_ALL, "Korean");
	splash();
	//printf("%s", aa);
	
	startmenu(MainMenu,"Conan - The Music Detective");
//	wchar_t *des = L"개발중인 버전입니다. Under Construction";


	//wchar_t *ws2 = L"동하! 동현이 하이라는뜻";
//	addwstr(ws2);


	return 0;
}