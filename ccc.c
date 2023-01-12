#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
 
int nod(int x, int y)
{
    if (x == y)
        return 0;
    int i;
    for (i = x; i > 1; i--)
        if ((x % i == 0) && (y % i == 0))
            return i;
 
    return 0;
}
 
char findmaxpoz(char *x, int q)
{
    int mas[32];
    for (int i = 0; i < 32; i++)
        mas[i] = 0;
 
    for (int i = 0; i < q; i++)
        mas[(x[i] - '?')]++;
    int maxc = 0, pos = 0;
    for (int i = 0; i < 32; i++)
        if (mas[i] > maxc)
        {
            maxc = mas[i];
            pos = i;
        }
    pos -= '?' - '?';
    return pos + '?';
}
 
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
 
    int dlina[100000];
    int key = 0, cmp = 0, sz = 0, kolsim = 0;
    char lgram1[4], lgram2[4], c, ch;
    char *str = (char *)calloc(1, sizeof(char));
    FILE *file = fopen("out.txt", "r");
    if (file == NULL)
    {
        printf("***???? ?? ??????***");
        return 1;
    }
 
    while (!feof(file))
        if (fread(&c, 1, 1, file))
        {
            str[sz++] = c;
            str = (char *)realloc(str, sz + 1);
        }
 
    for (int i = 0; i < strlen(str); i++)
    {
        lgram1[0] = str[i];
        lgram1[1] = str[i + 1];
        lgram1[2] = str[i + 2];
        lgram1[3] = '\0';
 
        for (int j = i + 1; j < strlen(str); j++)
        {
            lgram2[0] = str[j];
            lgram2[1] = str[j + 1];
            lgram2[2] = str[j + 2];
            lgram2[3] = '\0';
 
            if (!strcmp(lgram1, lgram2))
            {
                dlina[cmp++] = j - i;
            }
        }
    }
    int numnod[cmp];
    for (int i = 0; i < cmp; i++)
        numnod[i] = 0;
 
    for (int i = 0; i < cmp; i++)
        for (int j = i + 1; j < cmp; j++)
            numnod[nod(dlina[i], dlina[j])]++;
    for (int i = 0; i < cmp; i++) 
        if (numnod[key] < numnod[i])
            key = i;
    printf("*********************************\n");
    printf("????? ????? ????? %d ????????\n", key);
    printf("*********************************\n");
    free(str);
 
    /* ????????? ?????? */
 
    fseek(file, 0, SEEK_SET);
    cmp = 0;
    char *st = (char *)calloc(1, sizeof(char));
    while (!feof(file))
        if (fread(&ch, 1, 1, file))
        {
            if ((ch >= '?') || (ch <= '?') || (ch >= '?') || (ch <= '?'))
            {
                st[cmp] = ch;
                cmp++;
                st = (char *)realloc(st, cmp + 1);
            }
        }
    cmp--;
    printf("???????????? ????: ");
    fclose(file);
    char key_name[key];
    char buffer[cmp / key];
    for (int i = 0; i < key; i++)
    {
        for (int j = i; j < cmp; j += key)
        {
            buffer[kolsim] = st[j];
            kolsim++;
        }
        key_name[i] = findmaxpoz(buffer, kolsim);
        printf("%c", key_name[i]);
        kolsim = 0;
    }
    printf("\n");
    printf("*********************************\n");
    free(st);
 
    return 0;
}