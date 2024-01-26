// Ali Moazeni
// 402106637

#include <stdio.h>
#include <string.h>

int main(int argc , char *argv[])
{
    if      (!strcmp(argv[1] , "config") &&  !strcmp(argv[2] , "user.name" )) //git config user.name " "
    {
        FILE *file;
        file = fopen(".gb/local-name.txt" , "w"); 
        if(file == NULL)
        {
            printf("fatal: not in a gb directory\n");
            return 0;
        }
        fprintf(file , "%s" , argv[argc - 1]);
        fclose(file);
    }
    
    return 0;
}