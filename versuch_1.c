/*
 * versuch_1.c
 *
 *  Created on: Dec 26, 2021
 *  Author: osboxes
 */

#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <time.h>
//#include <stdbool.h>

struct Wortspiel {
	int anzahl_versuche;
	char eingaben_buchstabe[1];
	char erratenes_wort[100];
};


int zeigewort(struct Wortspiel *w) {
	int wortlength = strlen(w -> erratenes_wort);
	int s;
	printf("Das errate Wort ist: ");
	for (s = 0; s < wortlength; s++) {
		printf("%c ", w -> erratenes_wort[s]);
	}
	printf("\n");
	return 0;
}

int main() {
	char wort[100];
	int i;
	int j;
	struct Wortspiel wortspiel1;
	
	//Wort das erraten werden soll, wählen
	printf("\nGeben Sie ein Wort ein: ");
	fflush(stdout);
	scanf("%s", wort);


	int a = strlen(wort);
	int e;
    char unterstrich[a];
	wortspiel1.anzahl_versuche = a*2;
    //printf("Das Wort: ");
    for(e=0; e < a; e++) {
		wortspiel1.erratenes_wort[e] = '_';
		//printf("%c",wortspiel1.erratenes_wort[e]);
    	//printf(" ");
    }
	//printf("\n");
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
			if(wort[j] == wortspiel1.eingaben_buchstabe[0]) {
				printf("hurra\n");
				wortspiel1.erratenes_wort[j] = wortspiel1.eingaben_buchstabe[0];
				gefunden += 1;
				anzahl_richtiger_buchstaben += 1;
				i--;
				
				
			}
		}
		wortspiel1.anzahl_versuche = wortspiel1.anzahl_versuche + gefunden;
		printf("%d Versuche wurden aufgrund richtiger Eingabe gutgeschrieben.\n", gefunden);
		
		
		if (wortspiel1.anzahl_versuche - i != 0) {
			if (i == 0) {
				i++;
			}
			printf("\nVersuche es noch einmal bitte, du hast noch %d Versuche!\n", wortspiel1.anzahl_versuche - i);
		}
		else {
			printf("ende, vorbei, keine Versuche mehr.\n");
			break;	
		}
		zeigewort(&wortspiel1);
		printf("\n");
	}

	printf("Game over.\n");

	return 0;

}