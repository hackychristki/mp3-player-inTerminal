#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>


ma_engine engine;
ma_sound sound;


char *lastN(const char *str, size_t n)
{
    size_t len = strlen(str);
    return (char *)str + len - n;
}

static bool use_color;

static void style(const char *code)
{
    if (use_color) fputs(code, stdout);
}

static void draw_header(const char *screen)
{
    style("\x1b[1;96m");
    puts("  ============================================================");
    puts("    MP3 PLAYER                                        |:|::|:|");
    puts("  ============================================================");
    style("\x1b[0m");
    printf("\n    %s\n\n", screen);
}

static void draw_playback_status(bool playing)
{
    static unsigned int frame;
    static const char *const meters[] = {"|.::|", "|:.:|", "|::.|", "|:.:|"};

    printf("\r    ");
    style(playing ? "\x1b[1;96m" : "\x1b[1;93m");
    printf("%s  %-7s", playing ? meters[frame++ % 4] : "|   |",
           playing ? "PLAYING" : "PAUSED");
    style("\x1b[0m");
    fflush(stdout);
}

int main(int argc, char **argv)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD console_mode;
    use_color = GetConsoleMode(console, &console_mode) &&
                SetConsoleMode(console, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    while(true){    
        system("cls"); // Clear the screen
        draw_header("LIBRARY");


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
                style("\x1b[96m");
                printf("    %3d  ", songsCount);
                style("\x1b[0m");
                puts(songs[songsCount-1]);
                songsCount++;
            }
            } 
        closedir( musicFile );
        }else{
            puts("    Could not open the music folder.");
            
        }

        puts("  ------------------------------------------------------------");

        int choice = 0;
        bool quit = false;

        if(songsCount != 1){
            puts("    [999] Quit\n");
            do{
                printf("    Track number > ");
                fflush(stdout);
                scanf("%d", &choice);
                if(choice == 999){
                    quit = true;
                    break;
                }
                while (getchar() != '\n'); // Clear leftover characters from input buffer
            }while(!(choice >= 1 && choice <= songsCount - 1));

            // printf("%s now playing... \n", songs[choice - 1]);
        }else{
            puts("    No MP3 tracks available.");
            return 1;
        }

        if(quit == true){
            return 0;
        }
        

        if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
            printf("Failed to initialize audio engine.\n");
            return 1;
        }




        bool toMenu = false;

        bool playNext = false;
        bool playLast = false;

        while(true){

            system("cls"); // Clear the screen
            char path[300] = "music/";

            strcat(path, songs[choice - 1]);


            if (ma_sound_init_from_file(&engine, path, 0, NULL, NULL, &sound) != MA_SUCCESS) {
                printf("Failed to load: %s\n", path);
                ma_engine_uninit(&engine);
                return 1;
            }

            bool playing = true;

            ma_sound_start(&sound);
            draw_header("NOW PLAYING");
            style("\x1b[1m");
            printf("    %s\n", songs[choice - 1]);
            style("\x1b[0m");
            printf("    Track %d of %d\n\n", choice, songsCount - 1);
            puts("  --------------------------------------------------------------");
            puts("    [p] Pause/Resume   [n] Next   [l] Previous   [q] Library\n");


            while (true) {
                // non-blocking: only read when a key is actually waiting
                if (_kbhit()) {
                    int input = _getch();

                    if (input == 'p') {
                        if (playing) ma_sound_stop(&sound);
                        else         ma_sound_start(&sound);
                        playing = !playing;
                        if (!playing) draw_playback_status(false);
                    }
                    else if (input == 'q'){
                        toMenu = true;
                        break;
                    } else if (input == 'n'){
                        playNext = true;
                        break;
                    } else if (input == 'l'){
                        playLast = true;
                        break;
                    }
                }

                if (playing) draw_playback_status(true);
                if (ma_sound_at_end(&sound)) break;

                usleep(100000);
            }

            

            printf("\n");

            ma_sound_uninit(&sound);
            

            if(playNext){
                playNext = false;
                choice++;
                if(choice >= songsCount){
                    choice = 1;
                }
                continue;
            }else if(playLast){
                playLast = false;
                choice--;
                if(choice <= 0) choice = songsCount - 1;
                continue;
            }else{
                break;
            }

        }

        ma_engine_uninit(&engine);

        if(toMenu){
            toMenu = false;
            continue;
        }
    }
    return 0;
}


