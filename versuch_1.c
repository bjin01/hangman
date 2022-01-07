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


	int c;
	for(i=0; i < a; i++) {
		bool b = false;
		char buchstabe;
		printf("Gib einen Buchstaben ein: ");
		scanf(" %c", &buchstabe);
		printf("Der eingegebene Buchstabe ist: %c\n", buchstabe);
		for(j = 0; j < a; ++j){
			if(wort[0][j] == buchstabe) {
				printf("hurra\n");

				for ( c = j; c < a - 1 ; c++ ){
					wort[0][c] = wort[0][c+1];
				}
			}
			else {

				b = false;
			}
			printf("Restwort ist\n");

			for( c = 0 ; c < a - 1 ; c++ ){
				printf("%c\n", wort[                                                                                                                                                                                                                                                         n0][c]);
			}
		}
		if (!b) {
			printf("\nVersuche es noch einmal bitte!");
		}
		printf("\n");
	}

	printf("ende, vorbei.");

	return 0;

}
