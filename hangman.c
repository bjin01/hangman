/*
 * versuch_1.c
 *
 *  Created on: Dec 26, 2021
 *  Author: osboxes
 */
/* string.h wird wegen strlen gebraucht. */
#include <string.h>

#include <stdio.h>

/* termios.h und unistd.h werden von function getch benötigt */
#include <termios.h>
#include <unistd.h>

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
	printf("Das erratene Wort ist: ");
	for (s = 0; s < w->wortlaenge; s++) {
		printf("%c ", w -> erratenes_wort[s]);
	}
	printf("\n");
	return 0;
}

/* Hier beginnt die Hauptfunktion */

int main() {
	char wort[100];
    char ch = ' ';
	int i = 0;
	int j;
	struct Wortspiel wortspiel1;
	
	//Wort das erraten werden soll muss hier eingegeben werden.
	printf("\nGeben Sie ein Wort ein: ");
    /* Dann folgt hier eine while Schleife mit der jedes eingegebene Zeichen
     * durch einen Stern ersetzt wird bis ein \n new-line Zeichen kommt, 
     * dann wid die while Schleife verlassen.  
    */
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
    /* Es wird hier ein Null Zeichen in den wort array geschrieben
     * um das Ende des Wortes zu markieren. 
    */
    wort[i]='\0';

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
	printf("\nDie Wortlaenge ist %d\n", wortspiel1.wortlaenge);
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
			printf("hip hip hurra. Alles richtig.\n");
			break;
		}
		printf("Gib einen Buchstaben ein: ");
		fflush(stdout);
		scanf(" %c", &buchstabe);
		wortspiel1.eingaben_buchstabe[0] = buchstabe;
		printf("Der eingegebene Buchstabe ist: %c\n", wortspiel1.eingaben_buchstabe[0]);

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
				printf("haha, den Buchstaben %c hast du bereits erraten.\n", wortspiel1.eingaben_buchstabe[0]);
			}
            /* sonst prüfen wir ob der Tipp mit einem Buchstaben übereinstimmt. Wenn ja dann hurra. */
			else {
				if(wort[j] == wortspiel1.eingaben_buchstabe[0]) {
					printf("hurra\n");
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
		printf("%d Versuche wurden aufgrund richtiger Eingabe gutgeschrieben.\n", gefunden);
		
        /* Die verbleibende Anzahl an Versuchen wird hier berechnet wenn nicht gleich Null ist und dem User angezeigt. */
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