/*
 * config_tui.h
 *
 *  Created on: 2017. 12. 6.
 *      Author: craftingmod
 */
#include <wchar.h>
#include "data.h"

#ifndef CONFIG_TUI_H_
#define CONFIG_TUI_H_

extern char *help_str;
extern long PSEUDO_JHRUNNING;
extern wchar_t mainscreen_str[][300];
extern char music_intro[];
extern char music_playing[];
extern const wchar_t *appversion;
extern struct Tag tags[];
extern struct Music music[];
extern int VCC;
#endif /* CONFIG_TUI_H_ */
