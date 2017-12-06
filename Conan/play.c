
#include "play.h"
#include "decision.h"
#include "tui.h"
#include "data.h"
#include "fmod.h"
#include "playm.h"
#include "config_conan.h"

int Point[1000];//포인트 초기화 음악 갯수대로

struct command
{
	const wchar_t *text;
	void(*function)(WINDOW *);
};
typedef struct command COMMAND;


#define MAX_OPTIONS 5

int AN;
int AD;

SndSystem *mSystem = NULL;
SndSound *mSound = NULL;
SndChannel *mChannel = NULL;

COMMAND command[MAX_OPTIONS] =
{
	{ L"<예 그렇습니다>", AnswerYes },
	{ L"<그런거 같습니다>", AnswerMaybeYes },
	{ L"<모르겠습니다>", AnswerUnknown },
	{ L"<아닌거 같습니다>", AnswerMaybeNot },
	{ L"<아닌데요>", AnswerNo }

};

void closeQuest();

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

//그렇습니다 아닙니다 이런것들
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

			nc_getmouse(&mouse_event);

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
			bodymsg(L"\n\n\n\n\n\nESC를 눌러 플레이를 종료했습니다.");

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
	snprintf(numarray, sizeof(numarray), "%d", num);
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
	 // pause music
	 if(mChannel != NULL){
		 playm_pauseChannel(mChannel,1);
	 }
	beep();
	clsbody();
	bodymsg(L"                                /:      \n");
	bodymsg(L"                   `.-----.`    oys.    \n");
	bodymsg(L"               ./oyddddddddhy+-`yyo``  추리 결과 \n");
	bodymsg(L"     `.-:::-./ydmmmmmmmmmmmmmmdyhs:-  ====================================================================  \n");
	bodymsg(L"     ` `:+sydmmmmmmmmmmmmmmmmdmmdo     바보같은 녀석, 추리에는 이기고 지는 것도 위도 아래도 없어.\n");
	bodymsg(L"     `+hhhhdmmmmdmmmmmmmmmmmmdhddh:    진실은 언제나 단 하나뿐이니까. \n");
	bodymsg(L"    `ys/yddmdmmmhdmmmmmmmmmmmmmmmd+   ====================================================================     \n");
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
	bodymsg(L"           `ydhhhhhhhhhhhhhdhso++`      이노래 아닌데;; - N을 눌러 계속 진행하기\n");
	bodymsg(L"           /hhhhhyohhhhhhhhh-           \n");
	bodymsg(L"           +yhhdhhyhhhhhhhhd-           이거 맞아요 - <ESC>를 눌러 게임 종료하기\n");
	bodymsg(L"           ohhhhhhhhhhhhhhhy`           \n");
	bodymsg(L"          .hhhhyyshhhhhhhhhh+           \n");
	bodymsg(L"          :hhysyyshhhhhhhhhhh.          \n");
	bodymsg(L"          .:oooosyyhhhhhhhyyo`          ");

	int key;
	bool quit = FALSE;

	while (1)
	{
		noecho();
		keypad(stdscr, TRUE);
		

		key = getch();

		switch (key)
		{
		
		

		
		case 'n':
			quit = TRUE;
			//이어서 하기
			//게임 모드 재설정
			PLAY_STATUS = 1;
			//기존에 선택된 곡은 후보에 올리지 않음
			Point[DECESION_MUSIC_SRL] = -600;
			startPlay(0);

			break;
		case KEY_ESC:
			quit = TRUE;
			clsbody();
			bodymsg(L"\n\n\n\n\n\nESC를 눌러 플레ddd이를 종료했습니다.");

		}


		if (quit == TRUE) {

			break;

		}

 }

	//listbox();

}


/****************************Play GiveUp***************************/

 void PlayGiveUp(int resultmusic) {
	beep();
	closeQuest();
	clsbody();
	bodymsg(L"                                /:      \n");
	bodymsg(L"                   `.-----.`    oys.    \n");
	bodymsg(L"               ./oyddddddddhy+-`yyo``   \n");
	bodymsg(L"     `.-:::-./ydmmmmmmmmmmmmmmdyhs:-  ====================================================================  \n");
	bodymsg(L"     ` `:+sydmmmmmmmmmmmmmmmmdmmdo     때려쳤습니다.\n");
	bodymsg(L"     `+hhhhdmmmmdmmmmmmmmmmmmdhddh:   ====================================================================   \n");
	bodymsg(L"    `ys/yddmdmmmhdmmmmmmmmmmmmmmmd+     제 명석한 두뇌를 바탕으로 곰곰히 고민해 보았지만,\n");
	bodymsg(L"    :- ssomsdsymmddymddhyydmhdmmmm:     당신이 생각하는 게 뭔지 알래야 알수가 없습니다.\n");
	bodymsg(L"       s-.yshoyyhdmdhy+shyyyyosmmy      그나마 추론 가능한 음악들을 뽑아놨으니, 당신이 찾는 음악이 있길 바랍니다.\n");
	bodymsg(L"       `..+hs//:yoyyds+/:.:y+so+/.      \n");
	bodymsg(L"          .:o-.:s:--//---://-:++:       \n");
	bodymsg(L"           .:+:::/:--::::::::::.        1. "); bodymsg(music[DECESION_MUSIC_SRL].title); bodymsg(L" - "); bodymsg(music[DECESION_MUSIC_SRL].artist); bodymsg(L"\n");
		bodymsg(L"             `----:so+:-:--++:`         2. "); bodymsg(music[SECOND_MUSIC_SRL].title); bodymsg(L" - "); bodymsg(music[SECOND_MUSIC_SRL].artist); bodymsg(L"\n");
	bodymsg(L"                `.-::::/+:-/-.`         \n");
	bodymsg(L"               :oosyyyyyos///:os/`      \n");
	bodymsg(L"              +hhhhyyoyhho+/-:::ys-     \n");
	bodymsg(L"             -hhhhh---:yo/+///-:-s/     \n");
	bodymsg(L"            `shhhho.-:ys+++oo++:/yo     \n");
	bodymsg(L"            .yhhhhs+yhhhysssooosyo      \n");
	bodymsg(L"           `ydhhhhhhhhhhhhhdhso++`      \n");
	bodymsg(L"           /hhhhhyohhhhhhhhh-           \n");
	bodymsg(L"           +yhhdhhyhhhhhhhhd-           <ESC>를 눌러 게임 종료하기\n");
	bodymsg(L"           ohhhhhhhhhhhhhhhy`           \n");
	bodymsg(L"          .hhhhyyshhhhhhhhhh+           \n");
	bodymsg(L"          :hhysyyshhhhhhhhhhh.          \n");
	bodymsg(L"          .:oooosyyhhhhhhhyyo`          ");


	//listbox();

}

 void initPlay() {
	//처음 플레이 하는 경우
/*
	 SndSystem *mSystem = NULL;
	 SndSound *mSound = NULL;
	 SndChannel *mChannel = NULL;
	 */
	 if(mSystem == NULL){
		 playm_createSystem(&mSystem);
	 }
	 playm_playSoundWC(mSystem,&mSound,&mChannel,music_playing,1);

	 QUESTION_SRL = 0;
	 QUESTION_TRY_COUNT = 0;
	 DECESION_MUSIC_SRL = 0;

	 /*
	  * Init Tags count and music count
	  */
	 TAGS_COUNT = 0;
	 while((tags+TAGS_COUNT++)->name != NULL);
	 TAGS_COUNT -= 3; // throw start and end

	 MUSIC_COUNT = 0;
	 while((music+MUSIC_COUNT++)->title != NULL);
	 MUSIC_COUNT -= 1;

	 /* Reset point array */
	 int i;
	 for(i=0;i<MUSIC_COUNT;i+=1){
		 Point[i] = 0;
	 }

	 TAGS_MAX_COUNT = 15;
	 readyForShowResult = FALSE;
	 APEX_POINT = 0;
	 readyForShowResultCount = 0;

	 PLAY_STATUS = 1;
	 startPlay(0);
 }

 void startPlay(int answernum) {
	 //answernum 0부터 정보없음, 아닌데요, 잘 모르겠습니다, 모르겠습니다. 그럴겁니다. 맞습니다.
	
	 

	 

	 //현재 상항을 점검합니다.
	 if (PLAY_STATUS == 1) {
		 //플레이 중인 경우
		 //플레이시에 필요한 일회용 지역 변수를 정의합니다.
		 if(mChannel != NULL && playm_getPaused(mChannel)){
			 playm_pauseChannel(mChannel,0);
		 }
		   

		 //답변받은 정보를 반영합니다. 그리고 결정사항을 받아옵니다.
		 if (answernum != 0) {
			 PLAY_STATUS = decesion(QUESTION_SRL, answernum);
		 }

		

		
			 //질문 횟수를 증가시킵니다.
			 QUESTION_TRY_COUNT++;
			 //랜덤값을 픽해 질문 정보를 정합니다.
			 QUESTION_SRL = getQuestionNumber();

			 PlayMode(QUESTION_SRL);
		 }

	 if (PLAY_STATUS == 0) {

		 beep();
		 clsbody();
		 closeQuest();
		 bodymsg("\n\n\n\n\n종료 요청이 들어와 플레이가 종료되었습니다.");
	 }



	 if (PLAY_STATUS == 2) {
		 PlayResult(DECESION_MUSIC_SRL);
		 //결과 낸거 초기화
		 readyForShowResult = 0;
		 DIFFER_D_COUNT = 0;
	 }

	 if (PLAY_STATUS == 3) {
		 PlayGiveUp(DECESION_MUSIC_SRL);
	 }
		 
}
void closeQuest(){
	mChannel = NULL;
	if(mSound != NULL){
		playm_stopSound(mSound);
	}
	if(mSystem != NULL){
		playm_destorySystem(mSystem);
	}
	mSound = NULL;
	mSystem = NULL;
}



 



 int getRandInt(int start, int end) {
	 return rand() % ((end - start) + 1) + start;
 }


 int decesion( int questnum, int answer) {
	 int decesion_result = 0; // 0: nothing 1:  playing 2: result 3: give up
	 int answer_point = 0;
	 int apexMusic = -1; //1등음악값
	 int apexMusicPoint = -50;
	 int secondMusic = -1;// 2등음악값
	 int secondMusicPoint = -50;
	 int apexDiffer = 0;


	 //Set Answer Point
	 if (answer == 1) answer_point = answer_point - 10;
	 if (answer == 2) answer_point = answer_point - 4;
	 if (answer == 4) answer_point = answer_point + 4;
	 if (answer == 5) answer_point = answer_point + 10;


	 //점수 반영합니다. 
	 //모든 음악에 반영하는 반복문입니다.
	 for (int i = 0; i < MUSIC_COUNT; i++) {
		 // 태그 내에서 해당되는 태그가 있는지 검사합니다.
		 bool detected = FALSE;
		 for (int j = 1; j <= TAGS_MAX_COUNT; j++) {
			 //태그 번호
			 int tagnum = music[i].tags[j];

			 //태그를 찾은 경우
			 if (questnum == tagnum) {
				 //포인트 변경
				 detected = TRUE;
				 Point[i] += answer_point;

				
				
			 }
			 //동류값 갯수



			 //찾지 못하고 0을 만난 경우
			 if (tagnum == 0) {
				 break;
			 }

			 
		 }


		 //1등값 2등값 지정
		 //1등 지정
		 if (apexMusicPoint < Point[i]) {
			 apexMusic = i;
			 apexMusicPoint = Point[i];
		 }
		 else if (secondMusicPoint < Point[i]) {
			 secondMusicPoint = i;
			 SECOND_MUSIC_SRL = i;
			 secondMusicPoint = Point[i];
		 }

		


	 }

	 //1등 2등 차이 지정
	 apexDiffer = apexMusicPoint - secondMusicPoint;
	 //70이상 차이면 비교모드로 전환
	 if (apexDiffer > 70 && readyForShowResult == 0) readyForShowResult = 1;

	 //그다음에 할 행동 결정합니다.
	 int randactnum = getRandInt(1, 15);//임의의 변수 만들기
										//질문 갯수가 너무 적은 경우
	 if (QUESTION_TRY_COUNT < 7) return 1;

	 //확신이 드는 경우
	 if (apexDiffer > 15 + randactnum) {
	
		 if (readyForShowResult == 2) {
			 //확실함
			if(apexMusic >= 0) DECESION_MUSIC_SRL = apexMusic;
			 return 2;
		 }
		 //아직 생각중 모드가 아니라면 생각중 모드 설정
		 else if (readyForShowResult < 1){
			 //생각중 모드 설정 및 최초 differ서정
			 readyForShowResult = 1;
			 OLD_DIFFER = apexDiffer;
		 }
		
	 } 

	 //너무달라붕트면 때려쳐
	 if (readyForShowResultCount > 3) {
		 readyForShowResultCount = 0;
			 readyForShowResult = 0;
	 }

	 if (readyForShowResult == 1) {
		 //확실한지 확인하기
	
		 //그전 음악이 현재 음악과 같다면
		 if (DECESION_MUSIC_SRL == apexMusic && APEX_POINT <= apexMusicPoint) {
			 DIFFER_D_COUNT++;
		 }
		 else {
			 DIFFER_D_COUNT = 0;
			// Point[DECESION_MUSIC_SRL] -= 30;
			 readyForShowResult = 1;
		 }


		 if (apexMusic >= 0) DECESION_MUSIC_SRL = apexMusic; //일단 결과 후보 올림, -1값이 아닌 경우만 유호값
		 APEX_POINT = apexMusicPoint;					 // SECOND_MUSIC_SRL = secondMusic;
		
		// NEW_DIFFER = apexDiffer;
	

		 if (DIFFER_D_COUNT > 1) readyForShowResult = 2;
		 if ( QUESTION_TRY_COUNT > 49) readyForShowResult = 2;
	



		 return 1;
	 }
	 
	 //질문 갯수가 너무 많은 경우
	 if (QUESTION_TRY_COUNT > 35 + randactnum) return 3;
	 return 1; //해당사항 없는 일반적인 경우
 }

 int getQuestionNumber() {
	 if (readyForShowResult == 1) {//원래는 1
		 //결과를 고민중인 경우

		 int index = 0;
		 int resultnum = 0;
		 int dup = FALSE;//중복값

		



		 while (resultnum == 0 || dup == TRUE) {
			 int indexmax = TAGS_MAX_COUNT - 1;
			index = getRandInt(0, indexmax);
			int randint = getRandInt(0, 1);
			 if (randint == 1) {
				resultnum = music[DECESION_MUSIC_SRL].tags[index];
			 }
			 else {
				 resultnum = music[SECOND_MUSIC_SRL].tags[index];
				// resultnum =  music[SECOND_MUSIC_SRL].tags[index];
				
				// bodymsg(music[SECOND_MUSIC_SRL].title);
			 }

			//중복 있는지 검사하기
			 //같은 음악일 경우 스킵
			 if (DECESION_MUSIC_SRL == SECOND_MUSIC_SRL && resultnum !=0) return resultnum;
			 if (randint == 1) {
				 //1위꺼로 한 경우
				 //2위한테 있는지 확인
				 dup = ASearch(music[SECOND_MUSIC_SRL].tags, TAGS_MAX_COUNT, resultnum) != -1 ? TRUE : FALSE;
				
			 }
			 else {
				 //2위꺼로 한 경우
				 dup = ASearch(music[DECESION_MUSIC_SRL].tags, TAGS_MAX_COUNT, resultnum) != -1 ? TRUE : FALSE;
			 }
		 }
		 
		 return resultnum;
		
	 }
	 else {
		 //그냥 질문하는 경우
		return getRandInt(1, TAGS_COUNT);
	 }
 }

 int ASearch(const int a[], int n, int key)
 {
	 int i;
	 for (i = 0; i<n; i++)
	 {
		 if (a[i] == key)
			 return i;
	 }
	 return -1;
 }
