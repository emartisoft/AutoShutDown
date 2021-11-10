/*
1. Copy this app to startup folder (C:\Users\<USER>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup)
  (to open startup folder from terminal, write "shell:startup"
2. Copy windowsOpenHours.txt to C:\Windows

windowsOpenHours.txt
------------------------
9
10
11
17
18
------------------------
Windows running: 9:00-11:59 and 17:00-18:59
*/

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 10
#define TIMER   60
void getOpenHours(void);
void shutDown(void);
int openHour[24];
int i;

int main()
{
    sleep(TIMER); // After starts, wait TIMER seconds...
    bool done = false;
    bool shutdown = false;
    getOpenHours();
    printf("\nStarting...\n");

    while(!done)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        for(int x=0;x<24;x++)
        {
           if (openHour[x]==-1) break;
           if (openHour[x]==tm.tm_hour) { shutdown=false; break;}
           shutdown=true;
        }

        if(shutdown)
        {
            shutDown();
            done=true;
        }
        else
        {
            sleep(TIMER/4);
            getOpenHours();
        }
    }

    return 0;
}

void shutDown(void)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "shutdown %s %s %s", "/s", "/f", "/t 0");
    system(buffer);
}

void getOpenHours(void)
{
    FILE *f = fopen("c:\\Windows\\windowsOpenHours.txt", "r");
    for(i=0;i<24;i++) {openHour[i]=-1;};

    if(f)
    {
        i=0;
        char buffer[BUFFERSIZE];
        while(fgets(buffer, BUFFERSIZE, f))
        {
            openHour[i] = atoi(buffer);
            i++;
        }
    }
    fclose(f);
}
