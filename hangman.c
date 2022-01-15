/*
 * versuch_1.c
 *
 *  Created on: Dec 26, 2021
 *  Author: osboxes
 */
/* string.h wird wegen strlen gebraucht. */

/*_GNU_SOURCE wird benötigt für */
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 
/* http://simplestcodings.blogspot.com/2010/10/simple-logger-in-c.html*/

/* termios.h und unistd.h werden von function getch benötigt */
#include <termios.h>
#include <unistd.h>

extern const char* __progname;
#define PROGRAM_NAME __progname

#define ZEILEN 100 
#define WORTSIZE 100

void print_to_file(const char* message) {
    struct tm* current_tm;
    time_t time_now;
	FILE *f;
	printf("%s\n",message);
	f = fopen("/tmp/hangman.log", "a+");
	if (f == NULL) {
		printf("Can not open file hangman.log");
		return;
	}

    time(&time_now);
    current_tm = localtime(&time_now);

    int res = fprintf(f,
            "%s: %02i:%02i:%02i - %s\n"
                , PROGRAM_NAME
                , current_tm->tm_hour
                , current_tm->tm_min
                , current_tm->tm_sec
                , message );

    if (res == -1) {
        printf("Unable to write to log file!");
        return;
    }

    fclose(f);
}

/* 
 * Die getch() Funktion sorgt dafür dass die Eingabe des zu ratenen Wortes 
 * durch Asterix ersetzt werden. Wie eine Passworteingabe. 
 * Quelle: https://forum.ubuntuusers.de/topic/getch-fuer-gcc/
 */
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

/* Hier wird ein struct namens Wortspiel verwendet in dem die Variablen definiert werden.
 * --------------------------------------------------------------------------------------
 * Grund für Struct Verwendung ist eine struct Variable ermöglicht das return von 
 * mehreren Variablen aus der Unterfunktion an die main Funktion zurück.
 * --------------------------------------------------------------------------------------
 * anzahl_versuche - wie oft hat der Spieler schon versucht erfolglos zu raten.
 * wortlaenge - zeigt wie viele Zeichen der Array erratenes_wort hat.
 * eingaben_buchstaben - ist ein char array, von der Grösse 1, also 
 * nur ein Buchstabe passt hinein.
 * erratenes_wort - ist ein char array mit max 100 Zeichen. 
 * Wenn der Spieler Buchstaben richtig geraten hat dann werden diese Buchstaben 
 * hier in diesem array gespeichert.
 */
 
struct Wortspiel {
	int anzahl_versuche;
	int wortlaenge;
	char eingaben_buchstabe[1];
	char erratenes_wort[100];
};


/* Die Funktion zeigewort gibt das vom User erratene Wort an den Standard-output aus. 
 * Der Zeichen-Array erratenes_wort enthält entweder underscore oder die Buchstaben 
 * die der Spieler bereits richtig geraten hat.
 */
int zeigewort(struct Wortspiel *w) {
	
	int s;
	char* logmessage;
	if(0 > asprintf(&logmessage, "Das erratene Wort ist: ")) return printf("error happened.");
	print_to_file(logmessage);
	free(logmessage);
	
	for (s = 0; s < w->wortlaenge; s++) {
		printf("%c ", w -> erratenes_wort[s]);
	}
	printf("\n");
	return 0;
}

char * lesen_von_datei(char *wortfile) {
	char *randam_wort;
	char lines_from_file[ZEILEN][WORTSIZE];
	int num;
	if (wortfile != NULL) {
		FILE *fptr = NULL;
		;
		int i = 0;
    	int tot = 0;
		

		fptr = fopen(wortfile, "r");
    	while(fgets(lines_from_file[i], WORTSIZE, fptr)) {
			lines_from_file[i][strlen(lines_from_file[i]) - 1] = '\0';
			i++;
    	}
		tot = i;
		printf("\n The content of the file %s  are : \n",wortfile);    
		for(i = 0; i < tot; ++i) {
			printf(" %s\n", lines_from_file[i]);
		}
		i = 0;
		srand(time(0));
		num = (rand() % (tot - 0 + 1)) + 0;
		
		randam_wort = lines_from_file[num];
		printf("\n");
		

	}

	return randam_wort;
}

/* Hier beginnt die Hauptfunktion */

int main(int argc, char *argv[]) {
	char wort[100];
    char ch = ' ';
	int i = 0;
	int j;
	struct Wortspiel wortspiel1;
	
	char *rand_wort;

	int opt;
	int spielrunden = 0;
    char *wortliste = NULL;
    // put ':' in the starting of the
    // string so that program can 
    //distinguish between '?' and ':' 
    while((opt = getopt(argc, argv, ":i:r:")) != -1) 
    { 
        switch(opt) 
        { 
            case 'i':
				wortliste = optarg;
				printf("Wortliste Datei: %s\n", wortliste); 
                break;
            case 'r': 
				spielrunden = atoi(optarg);
                printf("Spielrunden angegeben: %d\n", spielrunden); 
                break; 
            case ':': 
                printf("Die Option braucht einen Wert.\n"); 
                break; 
            case '?': 
                printf("Die verwendete Option ist unbekannt: %c\n", optopt);
                break; 
        } 
    } 

	int z;
	if (spielrunden == 0) {
		spielrunden += 1;
		printf("Spielrundenzahl: %d\n", spielrunden);
	}
	char* logmessage;
	for (z = 0; z < spielrunden; z++) {
		
		if (wortliste != NULL) {
			rand_wort = lesen_von_datei(wortliste);
			/* printf("Das zufällige Wort ist: %s\n", rand_wort);
			printf("Das Wort hat %d Buchstaben.\n", (int) strlen(rand_wort)); */
		}
		

		
		if(0 > asprintf(&logmessage, "Hangman Spiel Beginn\n")) return printf("error happened.");
		print_to_file(logmessage);
		free(logmessage);
		
		int k;
		if ((int) strlen(rand_wort) > 0) {
			for (k = 0; k < (int) strlen(rand_wort); k++) {
				wort[k] = rand_wort[k];
				//printf("Buchstabe ins wort ist: %c\n", rand_wort[k]);
			}	
			wort[k]='\0';
		}
		else {
			//Wort das erraten werden soll muss hier eingegeben werden.
			printf("\nGeben Sie ein Wort ein: ");
			/* Dann folgt hier eine while Schleife mit der jedes eingegebene Zeichen
			* durch einen Stern ersetzt wird bis ein \n new-line Zeichen kommt, 
			* dann wid die while Schleife verlassen.  
			*/
			while (i<=100){
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
		}
		/* Es wird hier ein Null Zeichen in den wort array geschrieben
		* um das Ende des Wortes zu markieren. 
		*/
		printf("DAS WORT: %s\n", wort);
		if(0 > asprintf(&logmessage, "Das zu erratene Wort ist: %s", wort)) return printf("error happened.");
		print_to_file(logmessage);
		free(logmessage);
		
		int a = strlen(wort);
		int e;
		char unterstrich[a];
		/* Die Anzahl der max. Versuche beträgt das doppelte von Wortzeichenanzahl. */
		wortspiel1.anzahl_versuche = a*2;
		wortspiel1.wortlaenge = a;

		/* Hier wird das Zeichen Array erratenes_wort mit lauter Underscore ersetzt,
		* solange bis der Spieler einen Buchstaben richtig getippt hat.
		*/
		for(e=0; e < a; e++) {
			wortspiel1.erratenes_wort[e] = '_';
		}
		/*printf("\nDie Wortlaenge ist %d\n", wortspiel1.wortlaenge);*/
		if(0 > asprintf(&logmessage, "Die Wortlaenge ist: %d", wortspiel1.wortlaenge)) return printf("error happened.");
		print_to_file(logmessage);
		free(logmessage);
		zeigewort(&wortspiel1);

		int c;
		/* anzahl_richtiger_buchstaben wird verwendet um wenn der Wert gleich der Wortlänge a ist
		* dann hat der Spieler alle Buchstaben des Wortes richtig geraten und das Spiel gewonnen.
		*/
		int anzahl_richtiger_buchstaben = 0;

		/* Die for Schleife läuft so lange bis i kleiner gleich anzahl_versuche ist.
		* wobei der integer Wert von anzahl_versuche nach unten und oben verändern kann 
		* abhängig von Gutschrift, also wenn der User einen Buchstaben richtig getippt hat dann
		* bekommt man einen Versuch gutgeschrieben. Wenn aber ein Tipp gleich mehrere gleiche Buchstaben
		* im Wort erwischt hat dann zählt jeder Buchstabe als Gutschrift dazu. Dadurch wird anzahl_versuche 
		* auch mal steigen können.
		*/ 
		for(i=0; i <= wortspiel1.anzahl_versuche; ++i) {
			
			char buchstabe;
			if (anzahl_richtiger_buchstaben == a){
				if(0 > asprintf(&logmessage, "hip hip hurra. Alles richtig.")) return printf("error happened.");
				print_to_file(logmessage);
				free(logmessage);
				continue;
			}
			printf("Gib einen Buchstaben ein: ");
			fflush(stdout);
			scanf(" %c", &buchstabe);
			wortspiel1.eingaben_buchstabe[0] = buchstabe;
			if(0 > asprintf(&logmessage, "Der eingebene Versuchsbuchstabe ist: %c.", wortspiel1.eingaben_buchstabe[0])) return printf("error happened.");
			print_to_file(logmessage);
			free(logmessage);
			//printf("Der eingegebene Buchstabe ist: %c\n", wortspiel1.eingaben_buchstabe[0]);

			/* Variable gefunden wurde verwendet um Buchstaben die in 
			* eineme Wort mehrfach auftauchen zu Häufigkeit zu zählen.
			*/
			int gefunden = 0;

			/* In der for Schleife wird solang durchlaufen bis j kleiner als Wortlänge ist. */
			for(j = 0; j < a; j++){

				/* Die if prüft ob der Tipp des Spielers bereits einmal als richtiger Buchstabe eingegeben hat, 
				* wenn ja dann wird ein Hinweis ausgegeben.
				*/ 
				if (wortspiel1.erratenes_wort[j] == wortspiel1.eingaben_buchstabe[0]) {
					if(0 > asprintf(&logmessage, "haha, den Buchstaben %c hast du bereits erraten.", wortspiel1.eingaben_buchstabe[0])) return printf("error happened.");
					print_to_file(logmessage);
					free(logmessage);
				}
				/* sonst prüfen wir ob der Tipp mit einem Buchstaben übereinstimmt. Wenn ja dann hurra. */
				else {
					if(wort[j] == wortspiel1.eingaben_buchstabe[0]) {
						if(0 > asprintf(&logmessage, "hurra, Der Buchstabe: %c kommt tatsächlich vor.", wortspiel1.eingaben_buchstabe[0])) return printf("error happened.");
						print_to_file(logmessage);
						free(logmessage);
						wortspiel1.erratenes_wort[j] = wortspiel1.eingaben_buchstabe[0];

						/* nicht verwirren, gefunden ist eine Variable die innerhalb der for Schleife läuft und wird für 
						* jeden Tippversuch neu auf Null initialisiert.
						* anzahl_richtiger_buchstaben ist aber die max. Anzahl der richtig geratenen Buchstaben im gesamten 
						* Spiel. Wenn die Grösse von anzahl_richtiger_buchstaben gleich gross wie die Wortlänge ist 
						* dann hat der Spieler alle Buchstaben erraten und das Spiel gewonnen.
						*/
						gefunden += 1;
						anzahl_richtiger_buchstaben += 1;					
					}
				}
			}

			/* Hier erhöhe ich die Anzahl möglicher Versuche mit der sogenannten Gutschrift gefunden. */
			wortspiel1.anzahl_versuche = wortspiel1.anzahl_versuche + gefunden;
			if(0 > asprintf(&logmessage, "%d Versuche wurden aufgrund richtiger Eingabe gutgeschrieben.\n", gefunden)) return printf("error happened.");
			print_to_file(logmessage);
			free(logmessage);
			
			/* Die verbleibende Anzahl an Versuchen wird hier berechnet wenn nicht gleich Null ist und dem User angezeigt. */
			if (wortspiel1.anzahl_versuche - i != 0) {
				if(0 > asprintf(&logmessage, "Versuche es noch einmal bitte, du hast noch %d Versuche!", wortspiel1.anzahl_versuche - i)) return printf("error happened.");
				print_to_file(logmessage);
				free(logmessage);
			}
			else {
				if(0 > asprintf(&logmessage, "Ende, vorbei, keine Versuche mehr, du hast das Spiel verloren.")) return printf("error happened.");
				print_to_file(logmessage);
				free(logmessage);
				continue;	
			}
			zeigewort(&wortspiel1);
			printf("\n");
		}
		if(0 > asprintf(&logmessage, "Diese Spielrunde ist vorbei.")) return printf("error happened.");
		print_to_file(logmessage);
		free(logmessage);
	}
	if(0 > asprintf(&logmessage, "Game over.")) return printf("error happened.");
	print_to_file(logmessage);
	free(logmessage);
	return 0;
}
