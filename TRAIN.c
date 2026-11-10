#include <stdio.h>

int main(int argc, char **argv)
{
    printf("Hello, World!\n");

    int a = 3;
    int b = 5;
    int z = b;


    if(a < b){
        printf("%d", z);
    }else{
        printf("%d", a, "\n");
    }

    switch(a){
        case 1:
            printf("its one");
            break;
        case 2:
            printf("its two");
            break;
        case 3:
            printf("its three");
            break;
        default:
            break;
    }
    
    return 0;
}