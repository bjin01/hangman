/*
 * versuch_1.c
 *
 *  Created on: Dec 26, 2021
 *  Author: osboxes
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

int getch () {
    int ch;
    struct termios tc_attrib;
    if (tcgetattr(STDIN_FILENO, &tc_attrib))
        return -1;

    tcflag_t lflag = tc_attrib.c_lflag;
    tc_attrib.c_lflag &= ~ICANON & ~ECHO;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tc_attrib))
        return -1;

    ch = getchar();

    tc_attrib.c_lflag = lflag;
    tcsetattr (STDIN_FILENO, TCSANOW, &tc_attrib);
    return ch;
}

struct Wortspiel {
	int anzahl_versuche;
	int wortlaenge;
	char eingaben_buchstabe[1];
	char erratenes_wort[100];
};


int zeigewort(struct Wortspiel *w) {
	
	int s;
	printf("Das erratene Wort ist: ");
	for (s = 0; s < w->wortlaenge; s++) {
		printf("%c ", w -> erratenes_wort[s]);
	}
	printf("\n");
	return 0;
}

int main() {
	char wort[100];
    char ch = ' ';
	int i = 0;
	int j;
	struct Wortspiel wortspiel1;
	
	//Wort das erraten werden soll, wählen
	printf("\nGeben Sie ein Wort ein: ");
	while (i<=10){
        wort[i] = getch();
        ch = wort[i];
        if(ch=='\n') {
            break;
        }
        else {
            printf("*");
        }
        i++;
    }
    wort[i]='\0';

	int a = strlen(wort);
	int e;
    char unterstrich[a];
	wortspiel1.anzahl_versuche = a*2;
	wortspiel1.wortlaenge = a;
    for(e=0; e < a; e++) {
		wortspiel1.erratenes_wort[e] = '_';
    }
	printf("\nDie Wortlaenge ist %d\n", wortspiel1.wortlaenge);
	zeigewort(&wortspiel1);

	int c;
	int anzahl_richtiger_buchstaben = 0;
	for(i=0; i <= wortspiel1.anzahl_versuche; ++i) {
		
		char buchstabe;
		if (anzahl_richtiger_buchstaben == a){
			printf("hip hip hurra. Alles richtig.\n");
			break;
		}
		printf("Gib einen Buchstaben ein: ");
		fflush(stdout);
		scanf(" %c", &buchstabe);
		wortspiel1.eingaben_buchstabe[0] = buchstabe;
		printf("Der eingegebene Buchstabe ist: %c\n", wortspiel1.eingaben_buchstabe[0]);

		int gefunden = 0;

		for(j = 0; j < a; j++){
			if (wortspiel1.erratenes_wort[j] == wortspiel1.eingaben_buchstabe[0]) {
				printf("haha, den Buchstaben %c hast du bereits erraten.\n", wortspiel1.eingaben_buchstabe[0]);
			}
			else {
				if(wort[j] == wortspiel1.eingaben_buchstabe[0]) {
					printf("hurra\n");
					wortspiel1.erratenes_wort[j] = wortspiel1.eingaben_buchstabe[0];
					gefunden += 1;
					anzahl_richtiger_buchstaben += 1;					
				}
			}
		}
		wortspiel1.anzahl_versuche = wortspiel1.anzahl_versuche + gefunden;
		printf("%d Versuche wurden aufgrund richtiger Eingabe gutgeschrieben.\n", gefunden);
		
		if (wortspiel1.anzahl_versuche - i != 0) {
			printf("\nVersuche es noch einmal bitte, du hast noch %d Versuche!\n", wortspiel1.anzahl_versuche - i);
		}
		else {
			printf("Ende, vorbei, keine Versuche mehr, du hast das Spiel verloren.\n");
			break;	
		}
		zeigewort(&wortspiel1);
		printf("\n");
	}
	printf("Game over.\n");
	return 0;
}