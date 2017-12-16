#pragma once

void closeQuest(void);

void AnswerYes(void);
void AnswerMaybeYes(void);
void AnswerUnknown(void);
void AnswerMaybeNot(void);
void AnswerNo(void);

void display_menu(int, int);
int listbox(void);
void printBodyInt(int);

void PlayMode(int);
void PlayResult(int);
void PlayGiveUp(int);

void initPlay(void);
void startPlay(int);

int getRandInt(int, int);

int decesion(int, int);
int getQuestionNumber(void);
int ASearch(const int [], int,int);
