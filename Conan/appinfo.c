#include "appinfo.h"
#include "config_conan.h"

/****************************Appinfo ***************************/

 void AppInfo() {
	clsbody();

	bodymsg(L"\n  ______                        \n");
	bodymsg(L" / _____)                       \n");
	bodymsg(L"| /       ___  ____    ____ ____  \n");
	bodymsg(L"| |      / _ \\ |  _ \\ / _  |  _ \\ \n");
	bodymsg(L"| \\\\____ | | | | | | ( ( | | | | |\n");
	bodymsg(L" \\______)\\___/ |_| |_|\\_||_|_| |_|\n\n");
	bodymsg(L"About Conan - The Music Detective\n\n");

	bodymsg(L" App (");
	if(VCC){
		bodymsg(L"Visual Studio build");
	}else{
		bodymsg(L"gcc build");
	}
	bodymsg(L") / Verison ");
	bodymsg(appversion);
	bodymsg(L"\n");
	bodymsg(L"======================================\n");
	bodymsg(L" 코난 - 노래 찾는 탐정은 음악의 고유한 특징을 질문, 추정하여 찾고자 하는 음악이 무엇인지 알아냅니다.\n");
	bodymsg(L" 기존에는 음원을 재생해야 찾을 수 있었지만, Conan은 머리속에서 재생되는 음악을 찾습니다. 지금 플레이해보세요.");

	bodymsg(L"\n\n");

	bodymsg(L" Developer & Contributors\n");
	bodymsg(L"======================================\n");
	bodymsg(L"이정훈");
	if(PSEUDO_JHRUNNING){
		bodymsg(L", 벨붕");
	}else{
		bodymsg(L", 김수현, 양형준, 이상민, 장재혁");
	}
	bodymsg(L"\n");

	bodymsg(L"\n");

	bodymsg(L" ChangeLog\n");
	bodymsg(L"======================================\n");
	bodymsg(L"0.14 소리가 남\n");
	bodymsg(L"0.10 플레이 기능 작동\n");
	bodymsg(L"0.7 Play Result 페이지 인터페이스 구성됨,Play Mode 인터페이스 구성됨\n");
	bodymsg(L"\n");

	bodymsg(L" jhrun.tistory.com | tarks.net |");

	bodymsg(L" Created by Junghoon Lee, 2017\n");

}


/****************************AboutTeam ***************************/

 void AboutTeam() {
	clsbody();
	bodymsg(L"::'#####:::'##::::'##::::'##:::\n:'##.. ##::. ##::'##:::'####:::\n'##:::: ##::. ##'##::::.. ##:::\n ##:::: ##:::. ###::::::: ##:::\n ##:::: ##::: ## ##:::::: ##:::\n. ##:: ##::: ##:. ##::::: ##:::\n:. #####::: ##:::. ##::'######:\n::.....::::..:::::..:::......::\n\n");

	bodymsg(L"Team Info\n");
	bodymsg(L"======================================\n");
	bodymsg(L"0x1");
	bodymsg(L"\n\n");

	bodymsg(L"이정훈(20170854) - 팀장, 개발\n");
	bodymsg(L"김수현(20170814) - 자료정리, 발표자료, 음원 데이터베이스 구성\n");
	bodymsg(L"양형준(20170842) - 감정표현과 관련한 자료조사\n");
	bodymsg(L"이상민(20170850) - 음원 데이터베이스 구성\n");
	bodymsg(L"장재혁(20170858) - 감정표현과 관련한 자료조사\n");


}
