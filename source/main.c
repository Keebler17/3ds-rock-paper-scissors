#include <3ds.h>
#include <stdio.h>

int loops = 0;

int p1 = 0;
int p2 = 0;
int p1_score = 0;
int p2_score = 0;
bool p1_win = false;
bool p2_win = false;

bool runGame = false;

int selected = 0;
int turn = 1;

bool printResults = false;

PrintConsole top, bottom;

void printRock(bool selected);
void printPaper(bool selected);
void printScissors(bool selected);
void printTie();
void printP1Win();
void printP2Win();
void getWinner();
void printTurn(int player);
void printWin();
void printReturn();
void handleSelect(u32 kDown);
void handleSelection(u32 kDown);
void handleInput(u32 kDown);

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, &top);
	consoleInit(GFX_BOTTOM, &bottom);
	
	consoleSelect(&top);
	printf("\x1b[16;16HRock Paper Scissors");
	printf("\x1b[30;17HMade by Keebler17");
	
	consoleSelect(&bottom);
	printReturn();
	
	printTurn(1);
	
	while (aptMainLoop()) // needs less major cleanup than it did...
	{
		// handle inputs
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) {
			break; // exit loop, back to homebrew menu
		}
		
		if(loops == 240) {
			consoleInit(GFX_TOP, &top);
			runGame = true;
		}
				
		if(runGame) { // main game loop
			
			handleInput(kDown);
			printTurn(turn);
			
			consoleInit(GFX_TOP, &top);
			consoleSelect(&top);
			printf("\x1b[1;19H\x1b[34m%d     \x1b[37m-     \x1b[31m%d", p1_score, p2_score);
			
			
			if(printResults) {
				printWin();
			}
			
			printf("\x1b[37m");
			if(selected == 0) {
				printRock(true);
			} else if(selected == 1) {
				printPaper(true);
			} else if(selected == 2) {
				printScissors(true);
			}
			
			printRock(false);
			printPaper(false);
			printScissors(false);
			
			consoleSelect(&bottom);
			printReturn();
		}
		
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		loops++;

		//Wait for VBlank
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}

void printRock(bool selected) {
	printf("\x1b[15;23HRock");
	
	if(selected) {
		printf("\x1b[15;22H*");
	}
}

void printPaper(bool selected) {
	printf("\x1b[16;22HPaper");
	
	if(selected) {
		printf("\x1b[16;21H*");
	}
}

void printScissors(bool selected) {
	printf("\x1b[17;21HScissors");
	
	if(selected) {
		printf("\x1b[17;20H*");
	}
}

void printTie() {
	printf("\x1b[30;24H\x1b[37mTIE!");
}

void printP1Win() {
	printf("\x1b[30;22H\x1b[34mP1 WIN!");
}

void printP2Win() {
	printf("\x1b[30;22H\x1b[31mP2 WIN!");
}

void getWinner() {
	if (p1 == 0) {
		if (p2 == 1) {
			p2_score++;
			p2_win = true;
		} else if (p2 == 2) {
			p1_score++;
			p1_win = true;
		}
	} else if (p1 == 1) {
		if (p2 == 0) {
			p1_score++;
			p1_win = true;
		} else if (p2 == 2) {
			p2_score++;
			p2_win = true;
		}
	} else if (p1 == 2) {
		if (p2 == 0) {
			p2_score++;
			p2_win = true;
		} else if (p2 == 1) {
			p1_score++;
			p1_win = true;
		}
	}
}

void printTurn(int player) {
	if(player == 1) {
		printf("\x1b[1;16H\x1b[34mP1 TURN");
	} else {
		printf("\x1b[1;16H\x1b[31mP2 TURN");
	}
}

void printWin() {
	if(p1_win) {
		printP1Win();
	} else if(p2_win) {
		printP2Win();
	} else {
		printTie();
	}
}


void printReturn() {
	printf("\x1b[37m\x1b[29;8HPress START to return to");
	printf("\x1b[30;12HHomebrew Launcher");
}

void handleSelect(u32 kDown) {
	if(kDown & KEY_A) {
		consoleInit(GFX_BOTTOM, &bottom);
		consoleSelect(&bottom);
		
		if(turn == 1) {
			turn = 2;
			p1 = selected;
			
		} else {
			printResults = true;
			turn = 1;
			p2 = selected;
			
			p1_win = false;
			p2_win = false;
			
			consoleSelect(&top);
			getWinner();
		}
				
		selected = 0;
		printReturn();
	}
}

void handleSelection(u32 kDown) {
	if(kDown & KEY_DUP) {
		selected -= 1;
	}
	
	if(kDown & KEY_DDOWN) {
		selected += 1;
	}
	
	
	if(selected < 0) {
		selected += 3;
	}
	
	selected %= 3;
}

void handleInput(u32 kDown) {
	handleSelect(kDown);
	handleSelection(kDown);
}