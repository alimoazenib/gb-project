// Ali Moazeni
// 402106637

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <windows.h>

bool CheckExistGbFolder()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    struct dirent *entry;
    while (1)
    {
        // find .gb
        DIR *dir = opendir(".");
        if (dir == NULL) 
        {
            perror("Error opening current directory");
            return 1;
        }

        bool parent = false;
        while ((entry = readdir(dir)) != NULL) 
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".gb") == 0)
                return true;

            if (strcmp(entry->d_name, "..") == 0)
                parent = true;
        }
        
        closedir(dir);

        chdir("..");

        if (!parent)
            return false;
    }
}

int main(int argc , char *argv[])
{

    {// check the alias command
    FILE *filealias;
    filealias = fopen("D:\\gb-project\\files\\alias-commands.txt" , "r");

    char line[1000];
    fgets(line , sizeof(line) , filealias);
    char *tokenPtr = strtok(line , ">");

    while (tokenPtr != NULL) 
    {
        if (!strncmp(tokenPtr , argv[1] , strlen(tokenPtr)))
        {
            tokenPtr = strtok(NULL, ">?");

            char *tokenPtr1 = strtok(tokenPtr , " ");

            for (int i = 0 ; tokenPtr1 != NULL ; i++)
            {
                argv[i] = tokenPtr1;
                argc++;
                tokenPtr1 = strtok(NULL, " ");
            }
            
            break;
        }
        tokenPtr = strtok(NULL, ">?");
    }

    if (!strcmp(argv[1] , "config"))
        argc -= 2;
    
    fclose(filealias);
    }
    
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
        file = fopen("D:\\gb-project\\files\\global-name.txt" , "w");
        fprintf(file , "%s" , argv[argc - 1]);
        fclose(file);
        printf("Global name has been applied successfuly\n");
    }
    
    else if (!strcmp(argv[1] , "config") &&  !strcmp(argv[3] , "user.email")) //git config –-global user.email " "
    {
        FILE *file;
        file = fopen("D:\\gb-project\\files\\global-email.txt" , "w");
        fprintf(file , "%s" , argv[argc - 1]);
        fclose(file);
        printf("Global email has been applied successfuly\n");
    }

    else if (!strcmp(argv[1] , "config") && !strncmp(argv[3] , "alias." , 6)) //git config --global alias.sth "sth"
    {
        FILE *file;
        file = fopen("D:\\gb-project\\files\\valid-commands.txt" , "r");

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
            file1 = fopen("D:\\gb-project\\files\\alias-commands.txt" , "a");
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
        file = fopen("D:\\gb-project\\files\\valid-commands.txt" , "r");

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

    else if (!strcmp(argv[1] , "init")) // git init
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        if (CheckExistGbFolder())
            printf("Gb repository already exists\n");
        
        else
        {
            chdir(cwd);
            CreateDirectory(".gb" , NULL);
            SetFileAttributes(".gb" , FILE_ATTRIBUTE_HIDDEN);
            printf("Initialized empty Gb repository\n");
        }
    }
    
    return 0;
}