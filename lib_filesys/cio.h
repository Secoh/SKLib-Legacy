// Terminal-level I/O
//
// Copyright [2016-2017] Secoh
//
// Licensed under the Apache License, Version 2.0 (the "License") - see http://www.apache.org/licenses/LICENSE-2.0
// You may not use this file except in compliance with the License.
// Software is distributed on "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//

#if defined(_WIN32)

#include<conio.h>
#define kbhit _kbhit


#include<io.h>
#define x_isftty(file) (_isatty(_fileno(file)))		// use with stdin/out/err


#elif defined(__GNUC__)


#define x_isftty(file) (isatty(fileno(file)))



// lets just rewrite and declare missing functions
char getch();
int kbhit();

/* terminal I/O shall be C++ only
   forget about conio-style window clipping for a while
   ?? all work shall be done in wchar_t / Unicode

   consider using ncurses for Linux, and extend terminal functionality:

   is terminal?
   kbhit, getch - already there
   getscreeninfo - get size of available text window
   gotoxy - position cursor within terminal
   clearscreen - remember screen state, erase screen, prepare for using console
   restorescreen - finish with console, restore state if possible (called before exit from the program)
   textcolor(color [, background]) - change attribute
   textwrite - output text, flavors: (text[,color[,back]]), (x,y,text[,color[,back]])
   scrollwindow - x1,y1,x2,y2,count: positive-up, negative-down
*/

#endif


// ToDo: what about positioning around the screen for output?

