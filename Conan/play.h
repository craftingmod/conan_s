#pragma once

void    PlayMode(int);
void    PlayResult(int);
void    PlayGiveUp(int);
void startPlay(int);
void initPlay(void);
void AnswerYes(void);
void AnswerMaybeYes(void);
void AnswerUnknown(void);
void AnswerMaybeNot(void);
void AnswerNo(void);
int ASearch(const int a[], int size, int key);
int getQuestionNumber(void);
int getRandInt(int, int);
int TAGS_COUNT;
int TAGS_MAX_COUNT;
int MUSIC_COUNT;
int PLAY_STATUS; // 0: startplay 1:  playing 2: result 3: give up
int QUESTION_SRL; //���� �ѹ�
int QUESTION_TRY_COUNT; //���ݱ��� �� ���� Ƚ��
int Point[];
int SELECTED_ANSWER;
int readyForShowResult; //0: not yet 1:thinking 2: result
int readyForShowResultCount;