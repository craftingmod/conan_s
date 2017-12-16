#include <wchar.h>
#include "config_conan.h"
#include "data.h"

#ifdef ALT_X
    #define _help_str "Use arrow keys and Enter to select (Alt-X to quit) - Created by Junghoon Lee, 2017"
#else
    #define _help_str "Use arrow keys and Enter to select"
#endif

#ifdef _MSC_VER
	#define _VCC 1
#else
    #define _VCC 0
#endif

#define NO_PSEUDO_JH 0

#ifndef _MSC_VER
   # define _PSEUDO_JHRUNNING !NO_PSEUDO_JH
#else
# define _PSEUDO_JHRUNNING 0
#endif
/* Version Field */
const wchar_t *appversion = L"1.0.1217";

/* Music Field */
struct Tag tags[] = {
	{ L"오류", L"사용하지 않는 태그 번호입니다." },//0
	{ L"사랑", L"사랑과 관련된 음악입니까?" },//1
	{ L"남성", L"남자 가수가 부른 부분이 있는 음악인가요?" },//2 
	{ L"여성", L"여자 가수가 부른 부분이 있는 음악인가요?" },//3
	{ L"추억", L"어린 시절(유아기,청소년기) 추억을 상기시키는 음악인가요?" },//4
	{ L"혼성", L"혼성으로 부른 음악인가요?" },//5
	{ L"이별", L"이별과 관련된 스토리가 담긴 곡인가요?" },//6
	{ L"그리움", L"그리워하는 마음이 담긴 곡인가요?" },//7
	{ L"랩", L"랩 요소가 포함된 곡인가요?" },//8
	{ L"고음", L"고음이 많이 있는 곡인가요?" },//9
	{ L"듀엣", L"듀엣 곡인가요?" },//10
	{ L"그룹", L"여러명이서 부른 곡인가요?" },//11
	{ L"달달함", L"사랑을 시작하거나 하고있는 달달한 곡인가요?" },//12
	{ L"힐링", L"힐링되는 평화로운 노래인가요?" },//13
	{ L"애절", L"무언가 간절한, 애절함이 느껴지는 곡인가요?" },//14
	{ L"외국어", L"가사 중에 한국어가 아닌 영어 등 외국어가 포함되어 있었나요?" },//15
	{ L"애니매이션", L"애니매이션에서 사용됐던 음악인가요?" },//16
	{ L"행복", L"행복할거같은 감정이 담겨있는 곡인가요?" },//17
	{ L"솔로", L"혼자 부른 곡인가요?" },//18
	{ L"들썩", L"춤을 출것같이 몸이 들썩이는 음악인가요? 둠칫둠칫" },//19
	{ L"음식", L"뭔가 먹을만한 맛있는 것들이 나오는 노래인가요?" },//20
	{ L"짝사랑", L"짝사랑하는 곡인가요?(헤어진게 아님)" },//21
	{ L"휘파람", L"휘파람 소리가 나는 부분이 있는 곡인가요?" },//22
	{ L"베이비", L"가사에 '베이비(Baby)'라는 구절이 들어가나요?" },//23
	{ L"후회", L"자신이 했던 행동을 후회하고 있는 내용이 담긴 노래인가요?" },//24
	{ L"위아(중독성", L"가사중에 We are(위아)라는 구절이 중독성있게 사용되고 있나요?" },//25
	{ L"아아", L"가사중에 아아~~ 하는 부분이 있나요?" },//26
	{ L"술", L"술이 가사 내용에 들어간 곡인가요?" },//27
	{ L"음음", L"으으으음~, 음음~과 같은 부분이 있나요?" },//28
	{ L"생각", L"누군가 생각이 난다는 내용이 가사에 있나요? " },//29
	{ L"색깔", L"색(Color)과 관련된 음악인가요?" },//30
	{ L"좋아", L"좋아한다는 말이 가사에 있나요?" },//31
	{ L"운명", L"운명에 관해 이야기하는 곡인가요?" },//32
	{ L"키스", L"키스, 뽀뽀, 입맞춤과 관련된 내용이 있나요?" },//33
	{ L"안녕", L"안녕이라는 내용이 가사에 있나요?" },//34
	{ L"아름답다", L"아름답다라는 표현이 있는 곡인가요?" },//35
	{ L"잊다", L"뭔가를 잊으려 하는 곡인가요?" },//36
	{ L"감성", L"새벽에 들을만한 감성적인 곡인가요?" },//37
	{ L"날씨", L"날씨와 관련있는 곡인가요?" },//38
	{ L"오류", L"사용하지 않는 태그 번호입니다." }//끝
	,{NULL,NULL} //EndPointer
};

struct Music music[] = {
		{ L"나의 사춘기에게", L"볼빨간사춘기" ,{ 3,4,7,14,18,26, 35,37 } },//0
		{ L"좋니", L"윤종신" ,{ 1,2,6,7,14,18 ,31,37} },//0
		{ L"좋아", L"윤종신" ,{ 1,2,3,4,6,10,11 ,31,37} },//0
		{ L"그리워하다", L"비투비" ,{ 1,2,7,9, 8,11,15 } },//0
		{ L"밤이 되니까", L"펀치" ,{ 3,4,6,7,18,27,29,37 } },//0
		{ L"사랑하지 않은 것처럼", L"버즈" ,{ 1,2,6,7,9,14,18,24 ,34,36,37} },//0
		{ L"비도 오고 그래서", L"헤이즈" ,{ 1,2,3,5,7,10,11,14, 29,37,38 } },//0
		{ L"선물", L"멜로망스" ,{ 1,2,12,17,18 } },//0
		{ L"가을 아침", L"아이유" ,{ 3,4,13,17,18 } },//0
		{ L"All Of My Life", L"박원" ,{ 1,2,6,7,9,14,15,18,24 ,37} },//0
	    { L"보노보노 OP", L"임지숙" ,{ 3,4,13, 16,18} },//0
		{ L"썸탈거야", L"볼빨간사춘기" ,{ 1,3,12,15,17,18,21,29 } },//0
		{ L"시차(We Are) (Feat. 로꼬& Gray)", L"우원재" ,{ 2, 8,10, 11, 15, 19, 25} },//0
		{ L"Blue", L"볼빨간사춘기" ,{ 1,3,6,7, 15,18,30,31 } },//0
		{ L"가시나", L"선미" ,{ 1,3,6,7,15,18,19 } },//
		{ L"밤편지", L"아이유" ,{ 1,3,4,7,9,12,13,18 , 28, 33,37} },//16
		{ L"빨간 맛 (Red Favor)", L"레드벨벳" ,{1, 3,11,15,19,20,23, 30 } },//17
		{ L"에너제틱 (Energetic)", L"위너원" ,{ 1,2,11,15,19,23, 33 } },//18
		{ L"고쳐주세요", L"볼빨간사춘기" ,{ 1,3,7,8,10,11,15,21, 31} },//19
		{ L"DNA", L"방탄소년단" ,{ 1,2,8,11,15,19,22,23,32 } },//20
		{ L"마지막처럼", L"블랙핑크" ,{ 1,3,7,8,11,15,19,23 ,29 ,33} },//21
		{ L"남이 될 수 있을까", L"볼빨간사춘기, 스무살" ,{ 1,2,3,5,6,7,9,10,11,15,36,37 } },//22
		{ L"널 너무 모르고", L"헤이즈" ,{1,3,6,7,8,15,18 ,24,37} },//23
		{ L"Artist", L"지코" ,{ 2,8,15,18,19,23,25} },//24
		{ L"명탐정코난 극장판 오프닝", L"남도일(쿠도 신이치) & 코난(에도가와 코난)", {2,4,5,10,16,18,19}}//22
		,{NULL,NULL,{0}} //EndPointer
};
/* GUI Field */
// Memory drain, so what?
wchar_t mainscreen_str[][300] = {
		L"  ______                        \n",
		L" / _____)                       \n",
		L"| /       ___  ____    ____ ____  \n",
		L"| |      / _ \\ |  _ \\ / _  |  _ \\ \n",
		L"| \\\\____ | | | | | | ( ( | | | | |\n",
		L" \\______)\\___/ |_| |_|\\_||_|_| |_|\n\n",
		L"\'어... 그 노래 제목이 뭐였더라...\' 내가 지금 흥얼거리고 있는 노래가 궁금할 때\n\n",
		L" 1.0  패치 변경 내역\n",
		L"======================================\n",
		L"지원하는 노래 특징 갯수 39개로 확장, 표정 지원, 버그 픽스, 로딩 화면 개선\n",
		L"\n\n",
		L" 찾아낼 수 있는 음악\n",
		L"======================================\n",
		L"24개의 멜론 상위 샘플 음악 및 기타 2개\n",
		L"\n\n",
		L" 코난 - 노래 찾는 탐정은 음악의 고유한 특징을 질문, 추정하여 찾고자 하는 음악이 무엇인지 알아냅니다.\n",
		L" 기존에는 음원을 재생해야 찾을 수 있었지만, Conan은 머리속에서 재생되는 음악을 찾습니다. 지금 플레이해보세요.",
		L"" // don't remove as it is end.
};
/* filename field */
char music_intro[] = "title.mp3";
char music_playing[] = "main.mp3";

/* define field */
char *help_str = _help_str;
long PSEUDO_JHRUNNING = _PSEUDO_JHRUNNING;
int VCC = _VCC;
