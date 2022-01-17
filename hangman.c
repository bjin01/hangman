/*
 * hangman.c
 *
 *  Created on: Jan 16, 2022
 *      Author: Jessica Jin
 */

/* string.h wird wegen strlen gebraucht. */

/*_GNU_SOURCE wird dafür verwendet um auf Linux externe Variablen zugreifen zu können.
 * z.b.: __progname. Diese wird den Name des Programms liefern und wir schreiben es in die Log-Datei hinein.
*/
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

extern const char* __progname;
/* PROGRAM_NAME ist ein Alias um den Programmname in die Log-Datei zu schreiben. */
#define PROGRAM_NAME __progname

/* die untere enum sorgt dafür damit mit show und no_show definiert werden kann welche Nachricht oder Log die in die 
 * Log-Datei geschrieben wird auh dem User auf Stdout angezeigt wird. Alle show Nachrichten werden auf stdout geschrieben aber auch
 * in die Log-Datei.
*/
typedef enum { show, no_show } boolean;

#define ZEILEN 100
#define WORTSIZE 100

/* Die print_to_file() Funktion schreibt die Log-Datei.
 * http://simplestcodings.blogspot.com/2010/10/simple-logger-in-c.html
*/
void print_to_file(const char* message, bool screen) {
	
    struct tm* current_tm;
    time_t time_now;
	FILE *f;

	if (screen == show) {
		printf("%s\n",message);
	}

	f = fopen("hangman.log", "a+");
	if (f == NULL) {
		printf("Datei hangman.log kann nicht geöffnet werden");
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


/* Hier wird ein struct namens Wortspiel verwendet in dem die Variablen definiert werden.
 * --------------------------------------------------------------------------------------
 * Grund für Struct Verwendung ist dass eine struct Variable das return von
 * mehreren Variablen aus der Unterfunktion an die main Funktion zurück ermöglicht.
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
	if(0 > asprintf(&logmessage, "Das erratene Wort ist: ")) return 1;
	print_to_file(logmessage, show);
	free(logmessage);

	for (s = 0; s < w->wortlaenge; s++) {

		printf("%c ", w -> erratenes_wort[s]);
	}
	printf("\n");
	if(0 > asprintf(&logmessage, "%s", w -> erratenes_wort)) return 1;
	print_to_file(logmessage, no_show);
	free(logmessage);
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
			lines_from_file[i][(int) strlen(lines_from_file[i]) - 1] = '\0';
			i++;
    	}
		tot = i;
		char* logmessage;
		/* asprintf(&logmessage, "The content of the file %s is: ",wortfile);
		print_to_file(logmessage, no_show);
		free(logmessage);

		for(i = 0; i < tot; ++i) {
			asprintf(&logmessage, " %s", lines_from_file[i]);
			print_to_file(logmessage, no_show);
			free(logmessage);
		} */
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

    char ch = ' ';
	int i = 0;
	int j;
	struct Wortspiel wortspiel1;
	char wort[100];
	char *rand_wort;
	//bool screen = show;
	char* logmessage;
	int opt;
	int spielrunden = 0;
    char *wortliste = NULL;
    
	/* Hier unten in der while Schleife werden die Command line Argumente eingelesen. 
	 * -i mit Dateiname. In der Datei sind die Wörter gepeichert aus der nach Zufallsprinzip ein Wort zu Raten gewählt wird.
	 * -r für Anzahl Spielrunden. Wenn z.B: -r 5 eingegeben wird dann werden 5 Runden gespielt. User kann das Spiel jederzeit mit Strg-c abbrechen.
	*/
    while((opt = getopt(argc, argv, ":i:r:")) != -1)
    {

        switch(opt)
        {
            case 'i':
				wortliste = optarg;
				if(0 > asprintf(&logmessage, "Wortliste Datei: %s\n", wortliste)) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
                break;
            case 'r':
				spielrunden = atoi(optarg);
				if(0 > asprintf(&logmessage, "Spielrunden angegeben: %d\n", spielrunden)) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
                break;
            case ':':
				if(0 > asprintf(&logmessage, "Die Option braucht einen Wert.\n")) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
                break;
            case '?':
				if(0 > asprintf(&logmessage, "Die verwendete Option ist unbekannt: %c\n", optopt)) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
                break;
        }
    }

	int z;

	/* Hier wird die Anzahl der Spielrunden ermittelt. Wenn der User keine
	 * Anzahlspielrunden angegeben hat dann wird die Spielrunden auf 1 gesetzt.
	*/
	if (spielrunden == 0) {
		spielrunden += 1;
		if(0 > asprintf(&logmessage, "Spielrundenzahl: %d\n", spielrunden)) return 1;
		print_to_file(logmessage, show);
		free(logmessage);
	}

	/* Die Haupt-For-Schleife, solange bis alle Spielrunden gespielt worden. */
	for (z = 0; z < spielrunden; ++z) {

		char wort[100] = {0};
		int i = 0;
		int rw = 0;
		wortspiel1.anzahl_versuche = 0;
		if (wortliste != NULL) {
			rand_wort = lesen_von_datei(wortliste);
			rw = (int) strlen(rand_wort);
		}

		if(0 > asprintf(&logmessage, "\nHangman Spiel Beginn\n")) return 1;

		print_to_file(logmessage, show);
		free(logmessage);

		int k;

		/* Wenn das random_wort rw nicht gleich null ist dann bedeutet es dass das Wort aus der Wortliste kommt und das 
		 * Random-Wort an die Variable wort übergeben wird. 
		*/
		if (rw != 0) {

			for (k = 0; k < rw; k++) {
				wort[k] = tolower(rand_wort[k]);
			}
			wort[k]='\0';
		}
		else {
			/* Wenn das rw gleich null ist dann muss das Wort vom User eingegeben werden.
			 * getpass() Funktion ist gut um hier die Worteingabe vor dem Spieler zu verstecken.
			 * strncpy() ist praktisch um das eingegebene Wort direkt in die Variable wort zu kopieren.
			*/
			strncpy(wort, getpass("Geben Sie das Wort ein: "), 99);
			fflush(stdin);
		}

		if(0 > asprintf(&logmessage, "Das zu erratene Wort ist: %s", wort)) return 1;
		print_to_file(logmessage, no_show);
		free(logmessage);

		int a = (int) strlen(wort);
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

		if(0 > asprintf(&logmessage, "Die Wortlaenge ist: %d", wortspiel1.wortlaenge)) return 1;
		print_to_file(logmessage, show);
		free(logmessage);
		zeigewort(&wortspiel1);

		int c;

		/* anzahl_richtiger_buchstaben wird verwendet um wenn der Wert gleich der Wortlänge a ist
		* dann hat der Spieler alle Buchstaben des Wortes richtig geraten und das Spiel gewonnen.
		*/
		int anzahl_richtiger_buchstaben = 0;

		/* Die untere for Schleife läuft so lange bis i kleiner gleich anzahl_versuche ist.
		* wobei der integer Wert von anzahl_versuche nach unten und oben verändert werden kann
		* abhängig von der sog. Gutschrift, also wenn der User einen Buchstaben richtig getippt hat dann
		* bekommt man einen Versuch gutgeschrieben. Wenn aber ein Tipp gleich mehrere gleiche Buchstaben
		* im Wort erwischt hat dann zählt jeder Buchstabe als eine Gutschrift dazu. Dadurch wird anzahl_versuche
		* auch erhöht.
		*/
		for(i=0; i <= wortspiel1.anzahl_versuche; ++i) {

			char buchstabe;
			if (anzahl_richtiger_buchstaben == a){
				//wortspiel1.anzahl_versuche = a;
				if(0 > asprintf(&logmessage, "hip hip hurra. Alles richtig.")) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
				wortspiel1.anzahl_versuche = -1;
				continue;
			}

			printf("Gib einen Buchstaben ein: ");

			/* Die scanf() Zeile und das Leerzeichen vor %c ist absolut notwendig da sonst ab dem zweiten Durchlauf automatisch und
			 * fälschlicherweise ein \n als Zeichen automatisch eingelesen wird und der User kann keinen Buchstaben eingeben.
			*/
			scanf(" %c", &buchstabe);
			fflush(stdin);
			wortspiel1.eingaben_buchstabe[0] = tolower(buchstabe);
			if(0 > asprintf(&logmessage, "Der eingebene Versuchsbuchstabe ist: %c.", wortspiel1.eingaben_buchstabe[0])) return 1;
			print_to_file(logmessage, show);
			free(logmessage);

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
					if(0 > asprintf(&logmessage, "haha, den Buchstaben %c hast du bereits erraten.", wortspiel1.eingaben_buchstabe[0])) return 1;
					print_to_file(logmessage, show);
					free(logmessage);
				}
				/* sonst prüfen wir ob der Tipp mit einem Buchstaben übereinstimmt. Wenn ja dann hurra. */
				else {
					
					if(tolower(wort[j]) == wortspiel1.eingaben_buchstabe[0]) {
						if(0 > asprintf(&logmessage, "hurra, Der Buchstabe: %c kommt tatsächlich vor.", wortspiel1.eingaben_buchstabe[0])) return 1;
						print_to_file(logmessage, show);
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

			/* Hier wird die Anzahl möglicher Versuche mit der sogenannten Gutschrift gefunden wenn der Tipp richtig war.
			 * Wenn ein richtig getippter Buchstabe mehrmals vorkommt dann wird die Gutschrift dementsprechend erhöht.
			 * Die if Bedingung und minus 1 ist deswegen notwendig weil beim ersten Tippversuch ist i noch Null deshalb muss auch dieser 
			 * Versuch von der anzahl_versuche abgezogen werden.
			*/
			if (i == 0) {
				wortspiel1.anzahl_versuche = wortspiel1.anzahl_versuche - 1 + gefunden;
			} else {
				wortspiel1.anzahl_versuche = wortspiel1.anzahl_versuche + gefunden;
			}
			
			if(0 > asprintf(&logmessage, "%d Versuche wurden aufgrund richtiger Eingabe gutgeschrieben.\n", gefunden)) return 1;
			print_to_file(logmessage, show);
			free(logmessage);

			/* Die verbleibende Anzahl an Versuchen wird hier berechnet wenn nicht gleich Null ist und dem User angezeigt. */
			if (wortspiel1.anzahl_versuche - i != 0) {
				if(0 > asprintf(&logmessage, "Versuche es noch einmal bitte, du hast noch %d Versuche!", wortspiel1.anzahl_versuche - i)) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
			}
			else {
				if(0 > asprintf(&logmessage, "Ende, vorbei, keine Versuche mehr, du hast das Spiel verloren.")) return 1;
				print_to_file(logmessage, show);
				free(logmessage);
				wortspiel1.anzahl_versuche = -1;
				/* das continue ist dafür da um aus der for-Schleife zu springen. */
				continue;
			}
			zeigewort(&wortspiel1);
			printf("\n");
		}
		if(0 > asprintf(&logmessage, "Diese Spielrunde ist vorbei.")) return 1;
		print_to_file(logmessage, show);
		free(logmessage);
	}
	if(0 > asprintf(&logmessage, "Game over.")) return 1;
	print_to_file(logmessage, show);
	free(logmessage);
	return 0;
}
