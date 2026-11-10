#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    printf("MP3 PLAYER \n");
    printf("------------------------------------------\n");


    DIR* musicFile = opendir( "music" );

    struct dirent* hFile;
    
    int songsCount = 1;
    if ( musicFile ) {
        while ((hFile = readdir( musicFile )) != NULL ) {
       if ( !strcmp( hFile->d_name, "."  )) continue;
       if ( !strcmp( hFile->d_name, ".." )) continue;

        
            printf( "%d. %s\n",songsCount, hFile->d_name);
            
        } 
    closedir( musicFile );
    }else{
        printf("didnt find folder");
    }

    printf("------------------------------------------\n");


    // ma_engine engine;
    // ma_sound sound;

    // if (argc < 2) {
    //     printf("Usage: %s <file.mp3>\n", argv[0]);
    //     return 1;
    // }

    // if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
    //     printf("Failed to initialize audio engine.\n");
    //     return 1;
    // }

    // if (ma_sound_init_from_file(&engine, argv[1], 0, NULL, NULL, &sound) != MA_SUCCESS) {
    //     printf("Failed to load: %s\n", argv[1]);
    //     ma_engine_uninit(&engine);
    //     return 1;
    // }

    // ma_sound_start(&sound);
    // printf("Playing %s\nPress Enter to quit...\n", argv[1]);
    // // getchar();
    // bool playing = true; 

    // while(true){
    //     char input;
        
    //     scanf("%c", &input);
    //     printf("gelesen: %d, playing: %d\n", input, playing);
    //     if(input == 'p' && playing){
    //         ma_sound_stop(&sound);
    //         playing = false;
    //     }
    //     else if(input == 'p' && !playing){
    //         ma_sound_start(&sound);
    //         playing = true;
    //     }
    //     else if(input == 'q'){
    //         break;
    //     }
    // }

    // ma_sound_uninit(&sound);
    // ma_engine_uninit(&engine);
    return 0;
}
