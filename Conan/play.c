
#include "play.h"
#include "decision.h"
#include "tui.h"
#include "data.h"

int Point[100];//����Ʈ �ʱ�ȭ ���� �������


struct Tag tags[] = {
	{ L"����", L"������� �ʴ� �±� ��ȣ�Դϴ�." },//0
	{ L"���", L"����� ���õ� �����Դϱ�?" },//1
	{ L"����", L"���� ������ �θ� �κ��� �ִ� �����ΰ���?" },//2
	{ L"����", L"���� ������ �θ� �κ��� �ִ� �����ΰ���?" },//3
	{ L"�߾�", L"�߾��� ����Ű�� �����ΰ���?" },//4
	{ L"ȥ��", L"ȥ�� �׷����� �θ� �����ΰ���?" },//5
	{ L"�̺�", L"�̺��� ���õ� ���丮�� ��� ���ΰ���?" },//6
	{ L"�׸���", L"�׸����ϴ� ������ ��� ���ΰ���?" },//7
	{ L"��", L"�� ��Ұ� ���Ե� ���ΰ���?" },//8
	{ L"����", L"������ ���� �ִ� ���ΰ���?" },//9
	{ L"�࿧", L"�࿧ ���ΰ���?" },//10
	{ L"�׷�", L"�������̼� �θ� ���ΰ���?" },//11
	{ L"�޴���", L"����� �����ϰų� �ϰ��ִ� �޴��� ���ΰ���?" },//12
	{ L"����", L"�����Ǵ� �뷡�ΰ���?" },//13
	{ L"����", L"���� ������, �������� �������� ���ΰ���?" },//14
	{ L"�ܱ���", L"���� �߿� �ѱ�� �ƴ� ���� �� �ܱ�� ���ԵǾ� �־�����?" }//15


};

struct Music music[] = {
	{ L"���� ����⿡��", L"�����������" ,{ 3,4,7,14 } },//0
	{ L"����", L"������" ,{ 1,2,6,7,14 } },//0
	{ L"����", L"������" ,{ 1,2,3,4,6,10,11 } },//0
	{ L"�׸����ϴ�", L"������" ,{ 1,2,7, 8,11,15 } },//0
	{ L"���� �Ǵϱ�", L"��ġ" ,{ 3,4,6,7 } },//0
	{ L"������� ���� ��ó��", L"����" ,{ 1,2,6,9,14 } },//0
	{ L"�� ���� �׷���", L"������" ,{ 1,2,3,5,7,10,11,14 } },//0
	{ L"����", L"��θ���" ,{ 1,2,12 } },//0
	{ L"���� ��ħ", L"������" ,{ 3,4,13 } },//0
	{ L"All Of My Life", L"�ڿ�" ,{ 1,2,14,15 } }//0
};

struct command
{
	const wchar_t *text;
	void(*function)(WINDOW *);
};
typedef struct command COMMAND;


#define MAX_OPTIONS 5

int AN;
int AD;

COMMAND command[MAX_OPTIONS] =
{
	{ L"<�� �׷����ϴ�>", AnswerYes },
	{ L"<�׷��� �����ϴ�>", AnswerMaybeYes },
	{ L"<�𸣰ڽ��ϴ�>", AnswerUnknown },
	{ L"<�ƴѰ� �����ϴ�>", AnswerMaybeNot },
	{ L"<�ƴѵ���>", AnswerNo }

};


void AnswerYes() {
	SELECTED_ANSWER = 5;
}

void AnswerMaybeYes() {
	SELECTED_ANSWER = 4;
}

void AnswerUnknown() {
	SELECTED_ANSWER = 3;
}

void AnswerMaybeNot() {
	SELECTED_ANSWER = 2;
}
void AnswerNo() {
	SELECTED_ANSWER = 1;
}

//�׷����ϴ� �ƴմϴ� �̷��͵�
void display_menu(int old_option, int new_option)
{
	int lmarg = (COLS - 14) / 2,
		tmarg = (LINES - (MAX_OPTIONS + 2)) / 2;

	if (old_option == -1)
	{
		int i;
		attrset(A_REVERSE);
		for (i = 0; i < MAX_OPTIONS; i++)
			mvaddwstr(tmarg + i, lmarg, command[i].text);

	}
	else
		mvaddwstr(tmarg + old_option, lmarg, command[old_option].text);

	attrset(A_BOLD);
	mvaddwstr(tmarg + new_option, lmarg, command[new_option].text);
	attrset(A_REVERSE);
	refresh();
}



int listbox()
{
	WINDOW *win;
	int key, old_option = -1, new_option = 0, i;
	bool quit = FALSE;





	//erase();
	display_menu(old_option, new_option);

	while (1)
	{
		noecho();
		keypad(stdscr, TRUE);
		//	raw();
		mousemask(ALL_MOUSE_EVENTS, NULL);

		key = getch();

		switch (key)
		{
		case KEY_MOUSE:
		{
			const int tmarg = (LINES - (5 + 2)) / 2;
			int selected_opt;
			MEVENT mouse_event;

			getmouse(&mouse_event);

			selected_opt = mouse_event.y - tmarg;
			if (selected_opt >= 0 && selected_opt < 5)
			{
				old_option = new_option;
				new_option = selected_opt;
				display_menu(old_option, new_option);
			}
			if (mouse_event.bstate & BUTTON1_DOUBLE_CLICKED)
				key = 10;
		}
		if (key != 10)
			break;
		case 10:
		case 13:
		case KEY_ENTER:
			old_option = -1;
			//erase();
			//refresh();
		
			if (new_option == 0) SELECTED_ANSWER = 5;
			if (new_option == 1) SELECTED_ANSWER = 4;
			if (new_option == 2) SELECTED_ANSWER = 3;
			if (new_option == 3) SELECTED_ANSWER = 2;
			if (new_option == 4) SELECTED_ANSWER = 1;
			//erase();
			//if(new_option == 1) beep();
			startPlay(SELECTED_ANSWER);
			//display_menu(old_option, new_option);
			break;

		case KEY_PPAGE:
		case KEY_HOME:
			old_option = new_option;
			new_option = 0;
			display_menu(old_option, new_option);
			break;

		case KEY_NPAGE:
		case KEY_END:
			old_option = new_option;
			new_option = 5 - 1;
			display_menu(old_option, new_option);
			break;

		case KEY_UP:
			old_option = new_option;
			new_option = (new_option == 0) ?
				new_option : new_option - 1;
			display_menu(old_option, new_option);
			break;

		case KEY_DOWN:
			old_option = new_option;
			new_option = (new_option == 5 - 1) ?
				new_option : new_option + 1;
			display_menu(old_option, new_option);
			break;
		case KEY_RESIZE:
# ifdef PDCURSES
			resize_term(0, 0);
# endif
			old_option = -1;
			erase();
			display_menu(old_option, new_option);
			break;
		case 'Q':
		case 'q':
			quit = TRUE;
		case KEY_ESC:
			quit = TRUE;
			clsbody();
			bodymsg(L"\n\n\n\n\n\nESC�� ���� �÷��̸� �����߽��ϴ�.");

		}


		if (quit == TRUE) {

			break;

		}
	}


	return 0;
}

//int to chararray
void printBodyInt(int num) {
	char* numarray[3];
	sprintf_s(numarray, sizeof(numarray), "%d", num);
	bodycmsg(numarray);
}
/****************************Play ***************************/

void PlayMode(int questnum) {
	wchar_t *question = tags[questnum].question;

	
//	questtrynum[3] = '\0';
//	wchar_t *question_line= strcat(L"", strcat(question, L"\n"));

	beep();
	clsbody();

	bodymsg(L"                                /:      \n");
	bodymsg(L"                   `.-----.`    oys.    \n");
	bodymsg(L"               ./oyddddddddhy+-`yyo``   \n");
	bodymsg(L"     `.-:::-./ydmmmmmmmmmmmmmmdyhs:-  ====================================================================  \n");
	bodymsg(L"     ` `:+sydmmmmmmmmmmmmmmmmdmmdo    "); printBodyInt(QUESTION_TRY_COUNT); bodymsg(L". ");  bodymsg(question);   bodymsg(L"\n");
	bodymsg(L"     `+hhhhdmmmmdmmmmmmmmmmmmdhddh:   ====================================================================   \n");
	bodymsg(L"    `ys/yddmdmmmhdmmmmmmmmmmmmmmmd+     \n");
	bodymsg(L"    :- ssomsdsymmddymddhyydmhdmmmm:     \n");
	bodymsg(L"       s-.yshoyyhdmdhy+shyyyyosmmy      \n");
	bodymsg(L"       `..+hs//:yoyyds+/:.:y+so+/.      \n");
	bodymsg(L"          .:o-.:s:--//---://-:++:       \n");
	bodymsg(L"           .:+:::/:--::::::::::.        \n");
	bodymsg(L"             `----:so+:-:--++:`         \n");
	bodymsg(L"                `.-::::/+:-/-.`         \n");
	bodymsg(L"               :oosyyyyyos///:os/`      \n");
	bodymsg(L"              +hhhhyyoyhho+/-:::ys-     \n");
	bodymsg(L"             -hhhhh---:yo/+///-:-s/     \n");
	bodymsg(L"            `shhhho.-:ys+++oo++:/yo     \n");
	bodymsg(L"            .yhhhhs+yhhhysssooosyo      \n");
	bodymsg(L"           `ydhhhhhhhhhhhhhdhso++`      \n");
	bodymsg(L"           /hhhhhyohhhhhhhhh-           \n");
	bodymsg(L"           +yhhdhhyhhhhhhhhd-           \n");
	bodymsg(L"           ohhhhhhhhhhhhhhhy`           \n");
	bodymsg(L"          .hhhhyyshhhhhhhhhh+           \n");
	bodymsg(L"          :hhysyyshhhhhhhhhhh.          \n");
	bodymsg(L"          .:oooosyyhhhhhhhyyo`          ");


	listbox();

}

/****************************Play ***************************/

 void PlayResult(int resultmusic) {
	beep();
	clsbody();
	bodymsg(L"                                /:      \n");
	bodymsg(L"                   `.-----.`    oys.    \n");
	bodymsg(L"               ./oyddddddddhy+-`yyo``   \n");
	bodymsg(L"     `.-:::-./ydmmmmmmmmmmmmmmdyhs:-  ====================================================================  \n");
	bodymsg(L"     ` `:+sydmmmmmmmmmmmmmmmmdmmdo     �� ������ ����� ã�� ������...\n");
	bodymsg(L"     `+hhhhdmmmmdmmmmmmmmmmmmdhddh:   ====================================================================   \n");
	bodymsg(L"    `ys/yddmdmmmhdmmmmmmmmmmmmmmmd+     \n");
	bodymsg(L"    :- ssomsdsymmddymddhyydmhdmmmm:     \n");
	bodymsg(L"       s-.yshoyyhdmdhy+shyyyyosmmy      \n");
	bodymsg(L"       `..+hs//:yoyyds+/:.:y+so+/.      "); bodymsg(music[DECESION_MUSIC_SRL].title); bodymsg(L"\n");
	bodymsg(L"          .:o-.:s:--//---://-:++:       \n");
	bodymsg(L"           .:+:::/:--::::::::::.        "); bodymsg(music[DECESION_MUSIC_SRL].artist); bodymsg(L"\n");
	bodymsg(L"             `----:so+:-:--++:`         \n");
	bodymsg(L"                `.-::::/+:-/-.`         \n");
	bodymsg(L"               :oosyyyyyos///:os/`      \n");
	bodymsg(L"              +hhhhyyoyhho+/-:::ys-     \n");
	bodymsg(L"             -hhhhh---:yo/+///-:-s/     \n");
	bodymsg(L"            `shhhho.-:ys+++oo++:/yo     \n");
	bodymsg(L"            .yhhhhs+yhhhysssooosyo      \n");
	bodymsg(L"           `ydhhhhhhhhhhhhhdhso++`      \n");
	bodymsg(L"           /hhhhhyohhhhhhhhh-           \n");
	bodymsg(L"           +yhhdhhyhhhhhhhhd-           <Enter>�� ���� ���� �����ϱ�\n");
	bodymsg(L"           ohhhhhhhhhhhhhhhy`           \n");
	bodymsg(L"          .hhhhyyshhhhhhhhhh+           \n");
	bodymsg(L"          :hhysyyshhhhhhhhhhh.          \n");
	bodymsg(L"          .:oooosyyhhhhhhhyyo`          ");


	//listbox();

}


/****************************Play GiveUp***************************/

 void PlayGiveUp(int resultmusic) {
	beep();
	clsbody();
	bodymsg(L"                                /:      \n");
	bodymsg(L"                   `.-----.`    oys.    \n");
	bodymsg(L"               ./oyddddddddhy+-`yyo``   \n");
	bodymsg(L"     `.-:::-./ydmmmmmmmmmmmmmmdyhs:-  ====================================================================  \n");
	bodymsg(L"     ` `:+sydmmmmmmmmmmmmmmmmdmmdo     �����ƽ��ϴ�.\n");
	bodymsg(L"     `+hhhhdmmmmdmmmmmmmmmmmmdhddh:   ====================================================================   \n");
	bodymsg(L"    `ys/yddmdmmmhdmmmmmmmmmmmmmmmd+     �� ���� �γ��� �������� ������ ����� ��������,\n");
	bodymsg(L"    :- ssomsdsymmddymddhyydmhdmmmm:     ����� �����ϴ� �� ���� �˷��� �˼��� �����ϴ�.\n");
	bodymsg(L"       s-.yshoyyhdmdhy+shyyyyosmmy      �׳��� �߷� ������ ���ǵ��� �̾Ƴ�����, ����� ã�� ������ �ֱ� �ٶ��ϴ�.\n");
	bodymsg(L"       `..+hs//:yoyyds+/:.:y+so+/.      \n");
	bodymsg(L"          .:o-.:s:--//---://-:++:       \n");
	bodymsg(L"           .:+:::/:--::::::::::.        1. ���� ����⿡�� - �����������\n");
	bodymsg(L"             `----:so+:-:--++:`         2. �ֱ��� - ����\n");
	bodymsg(L"                `.-::::/+:-/-.`         3. ���� �Ǵϱ� - ��ġ\n");
	bodymsg(L"               :oosyyyyyos///:os/`      \n");
	bodymsg(L"              +hhhhyyoyhho+/-:::ys-     \n");
	bodymsg(L"             -hhhhh---:yo/+///-:-s/     \n");
	bodymsg(L"            `shhhho.-:ys+++oo++:/yo     \n");
	bodymsg(L"            .yhhhhs+yhhhysssooosyo      \n");
	bodymsg(L"           `ydhhhhhhhhhhhhhdhso++`      \n");
	bodymsg(L"           /hhhhhyohhhhhhhhh-           \n");
	bodymsg(L"           +yhhdhhyhhhhhhhhd-           <Enter>�� ���� ���� �����ϱ�\n");
	bodymsg(L"           ohhhhhhhhhhhhhhhy`           \n");
	bodymsg(L"          .hhhhyyshhhhhhhhhh+           \n");
	bodymsg(L"          :hhysyyshhhhhhhhhhh.          \n");
	bodymsg(L"          .:oooosyyhhhhhhhyyo`          ");


	//listbox();

}

 void initPlay() {
	//ó�� �÷��� �ϴ� ���

	 QUESTION_SRL = 0;
	 QUESTION_TRY_COUNT = 0;
	 DECESION_MUSIC_SRL = -1;
	 TAGS_COUNT = 15;
	 MUSIC_COUNT = 10;
	 TAGS_MAX_COUNT = 15;


	 PLAY_STATUS = 1;
	 startPlay(0);
 }

 void startPlay(int answernum) {
	 //answernum 0���� ��������, �ƴѵ���, �� �𸣰ڽ��ϴ�, �𸣰ڽ��ϴ�. �׷��̴ϴ�. �½��ϴ�.
	
	 

	 

	 //���� ������ �����մϴ�.
	 if (PLAY_STATUS == 1) {
		 //�÷��� ���� ���
		 //�÷��̽ÿ� �ʿ��� ��ȸ�� ���� ������ �����մϴ�.
		   

		 //�亯���� ������ �ݿ��մϴ�. �׸��� ���������� �޾ƿɴϴ�.
		 if (answernum != 0) {
			 PLAY_STATUS = decesion(QUESTION_SRL, answernum);
		 }

		

		
			 //���� Ƚ���� ������ŵ�ϴ�.
			 QUESTION_TRY_COUNT++;
			 //�������� ���� ���� ������ ���մϴ�.
			 QUESTION_SRL = getRandInt(1, TAGS_COUNT);

			 PlayMode(QUESTION_SRL);
		 }

	 if (PLAY_STATUS == 0) {

		 beep();
		 clsbody();
		 bodymsg("\n\n\n\n\n���� ��û�� ���� �÷��̰� ����Ǿ����ϴ�.");
	 }



	 if (PLAY_STATUS == 2) {
		 PlayResult(DECESION_MUSIC_SRL);
	 }

	 if (PLAY_STATUS == 3) {
		 PlayGiveUp(DECESION_MUSIC_SRL);
	 }
		 
	 }




 



 int getRandInt(int start, int end) {
	 return rand() % ((end - start) + 1) + start;
 }


 int decesion( int questnum, int answer) {
	 int decesion_result = 0; // 0: nothing 1:  playing 2: result 3: give up
	 int answer_point = 0;
	 int apexMusic = -1; //1�����ǰ�
	 int apexMusicPoint = -50;
	 int secondMusic = -1;// 2�����ǰ�
	 int secondMusicPoint = -50;
	 int apexDiffer = 0;

	 //Set Answer Point
	 if (answer == 1) answer_point = answer_point - 10;
	 if (answer == 2) answer_point = answer_point - 4;
	 if (answer == 4) answer_point = answer_point + 4;
	 if (answer == 2) answer_point = answer_point + 10;


	 //���� �ݿ��մϴ�. 
	 //��� ���ǿ� �ݿ��ϴ� �ݺ����Դϴ�.
	 for (int i = 0; i < MUSIC_COUNT; i++) {
		 // �±� ������ �ش�Ǵ� �±װ� �ִ��� �˻��մϴ�.
		 bool detected = FALSE;
		 for (int j = 1; j <= TAGS_MAX_COUNT; j++) {
			 //�±� ��ȣ
			 int tagnum = music[i].tags[j];

			 //�±׸� ã�� ���
			 if (questnum == tagnum) {
				 //����Ʈ ����
				 detected = TRUE;
				 Point[i] += answer_point;

				 //������ ����


				 //1� 2� ����
				 //1�� ����
				 if (apexMusicPoint < Point[i]) {
					 apexMusic = i;
					 apexMusicPoint = Point[i];
				 }
				 else if (secondMusicPoint < Point[i]) {
					 secondMusicPoint = i;
					 secondMusicPoint = Point[i];
				 }

				 //1�� 2�� ���� ����
				 apexDiffer = apexMusicPoint - secondMusicPoint;
				
				 break;
			 }

			 //ã�� ���ϰ� 0�� ���� ���
			 if (tagnum == 0) {
				 break;
			 }

			 
		 }

	 }
	 //�״����� �� �ൿ �����մϴ�.
	 int randactnum = getRandInt(1, 15);//������ ���� �����
										//���� ������ �ʹ� ���� ���
	 if (QUESTION_TRY_COUNT < 10) return 1;

	 //Ȯ���� ��� ���
	 if (apexDiffer > 30 + randactnum) {
		 DECESION_MUSIC_SRL = apexMusic;
		 return 2;
	 }
	 //���� ������ �ʹ� ���� ���
	 if (QUESTION_TRY_COUNT > 35 + randactnum) return 3;
	 return 1; //�ش���� ���� �Ϲ����� ���
 }