#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>



ma_engine engine;
ma_sound sound;


char *lastN(const char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}

void advance_cursor() {
  static int pos=0;
  char cursor[4]={'/','-','\\','|'};
  printf("%c\b", cursor[pos]);
  fflush(stdout);
  pos = (pos+1) % 4;
}

int main(int argc, char **argv)
{
    printf("+----------------------------------------+\n");
    printf("|------------MP3 PLAYER------------------|\n");
    printf("|----------------------------------------+\n");


    DIR* musicFile = opendir( "music" );

    struct dirent* hFile;
    
    char songs[100][260];

    int songsCount = 1;
    if ( musicFile ) {
        while (songsCount < 101 && (hFile = readdir( musicFile )) != NULL ) {
       if ( !strcmp( hFile->d_name, "."  )) continue;
       if ( !strcmp( hFile->d_name, ".." )) continue;

        if(hFile->d_namlen >= 4 && strcmp(lastN(hFile->d_name, 4), ".mp3") == 0){
            strcpy(songs[songsCount-1], hFile->d_name);
            printf( "| %d. %s \n",songsCount, songs[songsCount-1]);
            songsCount++;
        }
        } 
    closedir( musicFile );
    }else{
        printf("didnt find folder\n");
        
    }

    printf("+----------------------------------------+\n");

    int choice = 0;

    if(songsCount != 1){
        do{
            printf("now choose a song: ");
            scanf("%d", &choice);
            while (getchar() != '\n'); // Clear leftover characters from input buffer
        }while(!(choice >= 1 && choice <= songsCount - 1));

        // printf("%s now playing... \n", songs[choice - 1]);
    }else{
        printf("Music directory is empty");
        return 1;
    }
    

    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
        return 1;
    }

    char path[300] = "music/";

    strcat(path, songs[choice - 1]);


    if (ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound) != MA_SUCCESS) {
        printf("Failed to load: %s\n", path);
        ma_engine_uninit(&engine);
        return 1;
    }

    ma_sound_start(&sound);
    printf("Playing %s\nInput 'p' to pause, 'q' to quit... ", songs[choice - 1]);

    int i;
    for (i=0; i<100; i++) {
        advance_cursor();
        usleep(1000000);
    }
    printf("\n");

    // getchar();
    bool playing = true; 

    while(true){
        char input;
        
        scanf("%c", &input);
        if(input == 'p' && playing){
            ma_sound_stop(&sound);
            playing = false;
        }
        else if(input == 'p' && !playing){
            ma_sound_start(&sound);
            playing = true;
        }
        else if(input == 'q'){
            break;
        }
    }

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    return 0;
}


