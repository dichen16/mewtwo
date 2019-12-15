#include <stdio.h>
#include <unistd.h>
#include <time.h>

char* get_time(void);

int main(){
    char* src;
    src = get_time();
    printf("%s", src);
    return 0;
}


char* get_time(void)
{
    time_t current_time;
    char* c_time_string;

    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

   //printf("Current time is %s", c_time_string);
   return c_time_string;
}
