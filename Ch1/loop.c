#include <stdio.h>
#include <unistd.h>

int main (void) {
 printf("star loop.............\n");
 while(1)
 {
    sleep(3);
    printf("hello, word\n");
 }

}