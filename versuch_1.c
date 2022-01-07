/*
 * versuch_1.c
 *
 *  Created on: Dec 26, 2021
 *      Author: osboxes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main() {
	char wort[1][100];
	int i;
	int j;

	//Wort das erraten werden soll, wählen
	printf("\nGeben Sie ein Wort ein: ");
	fflush(stdout);
	scanf("%s", &wort);


	int a = strlen(wort);
	int e;
    char unterstrich[a];
    printf("Das Wort: ");
    for(e=0; e < a; e++) {
    	printf("_ ");
    }
    printf("\n");


	int c;
	for(i=0; i < a; i++) {
		bool b = false;
		char buchstabe;
		printf("Gib einen Buchstaben ein: ");
		scanf(" %c", &buchstabe);
		printf("Der eingegebene Buchstabe ist: %c\n", buchstabe);

		int k;
		for (k = 0; k < 3; k++) {
			for(j = 0; j < a; ++j){
				if(wort[0][j] == buchstabe) {
					printf("hurra\n");

					for ( c = j; c < a ; c++ ){
						wort[0][c] = wort[0][c+1];
					}
					a = a-1;
				}
				/*else {

					b = false;
				}*/
				//printf("Restwort ist\n");

			}
		}
		for( c = 0 ; c < a ; c++ ){
			printf("%c", wort[0][c]);
		}
		if (a != 0) {
			printf("\nVersuche es noch einmal bitte, du hast noch %d Versuche!", a);
		}
		printf("\n");
	}

	printf("ende, vorbei.");

	return 0;

}
