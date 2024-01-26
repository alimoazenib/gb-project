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

    else if (!strcmp(argv[1] , "config") &&  !strcmp(argv[2] , "user.email")) //git config user.email " "
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

    else if (!strcmp(argv[1] , "config") &&  !strcmp(argv[3] , "user.name" )) //git config -–global user.name " "
    {
        FILE *file;
        file = fopen("D:\\myproject\\files\\global-name.txt" , "w");
        fprintf(file , "%s" , argv[argc - 1]);
        fclose(file);
        printf("Global name has been applied successfuly\n");
    }
    
    else if (!strcmp(argv[1] , "config") &&  !strcmp(argv[3] , "user.email")) //git config –-global user.email " "
    {
        FILE *file;
        file = fopen("D:\\myproject\\files\\global-email.txt" , "w");
        fprintf(file , "%s" , argv[argc - 1]);
        fclose(file);
        printf("Global email has been applied successfuly\n");
    }

    else if (!strcmp(argv[1] , "config") && !strncmp(argv[3] , "alias." , 6)) //git config --global alias.sth "sth"
    {
        FILE *file;
        file = fopen("D:\\myproject\\files\\valid-commands.txt" , "r");

        int isinvalid = 0;
        char line[100];
        fgets(line , 100 , file);
        char *tokenPtr = strtok(line , "?");
        while (tokenPtr != NULL) 
        {
            if (!strncmp(tokenPtr , argv[4] , strlen(tokenPtr)))
            {
                isinvalid = 1;
                break;
            }
            tokenPtr = strtok(NULL, "?");
        }

        if (isinvalid == 1)
        {
            FILE *file1;
            file1 = fopen("D:\\myproject\\files\\alias-commands.txt" , "a");
            fprintf(file1 , ">%s>%s?" , argv[3]+6 , argv[4]);
            fclose(file1);
        }

        else
            printf("The command you are trying to add an alias to is invalid\n");
        
        fclose(file);
    }
    
    else if (!strcmp(argv[1] , "config") && !strncmp(argv[2] , "alias." , 6)) //git config alias.sth "sth"
    {
        FILE *file;
        file = fopen("D:\\myproject\\files\\valid-commands.txt" , "r");

        int isinvalid = 0;
        char line[100];
        fgets(line , 100 , file);
        char *tokenPtr = strtok(line , "?");
        while (tokenPtr != NULL) 
        {
            if (!strncmp(tokenPtr , argv[3] , strlen(tokenPtr)))
            {
                isinvalid = 1;
                break;
            }
            tokenPtr = strtok(NULL, "?");
        }

        if (isinvalid == 1)
        {
            FILE *file1;
            file1 = fopen(".gb\\alias-commands.txt" , "a");
            if(file1 == NULL)
            {
                printf("fatal: not in a gb directory\n");
                return 0;
            }

            fprintf(file1 , ">%s>%s?" , argv[2]+6 , argv[3]);
            fclose(file1);
        }

        else
            printf("The command you are trying to add an alias to is invalid\n");
        
        fclose(file);
        
    }
    
    return 0;
}