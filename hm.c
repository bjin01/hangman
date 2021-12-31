#include<stdio.h>

int main()
{
    char wort[1][100];
    int i;
 
    printf("Das Wort eingeben\n");
    scanf("%s",wort);
    printf("Das Wort ist %s\n",wort);
    printf("Das Wort hat %d Buchstaben.\n", strlen(wort));
    int s = strlen(wort);
    for(i = 0; i < s; i++)
          printf("%d = %c\n",i,wort[0][i]);
    return 0;
}