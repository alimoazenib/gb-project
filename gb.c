// Ali Moazeni
// 402106637

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int countFilesInDirectory(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    int count = 0;

    if (dir == NULL) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            count++;
        }
    }

    closedir(dir);
    return count;
}

int compareFileDates(char *global, char *local) 
{
    FILE *global1;
    global1 = fopen(global , "r"); 
    if(global1 == NULL)
        return 2;
    else
        fclose(global1);

    FILE *local1;
    local1 = fopen(local , "r"); 
    if(local1 == NULL)
        return 1;
    else
        fclose(local1);

    struct stat attr1, attr2;
    if(stat(global, &attr1) == 0 && stat(local, &attr2) == 0) 
    {
        if(attr1.st_mtime > attr2.st_mtime) 
            return 1; // global is newer

        else if(attr1.st_mtime > attr2.st_mtime) 
            return 2; // local is newer
    }
}

bool checkEmptyFolder(const char *folderPath) 
{
    DIR *dir = opendir(folderPath);
    struct dirent *entry;
    int isEmpty = 1;

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            isEmpty = 0;
            break;
        }
    }

    closedir(dir);

    if (isEmpty) 
        return true;
    else 
        return false;
}

char* getFileName(const char* filePath) 
{
    char* fileName = strrchr(filePath, '/');
    if (fileName != NULL) {
        return fileName + 1;
    }
    return (char*)filePath;
}

void replaceBackslash(char *str) 
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\') {
            str[i] = '/';
        }
    }
}

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

bool file_or_folder(char *path) {
    struct stat s;
    if (stat(path, &s) == 0) 
    {
        if (s.st_mode & S_IFDIR) 
            return false;
        else if (s.st_mode & S_IFREG) 
            return true;
    }
}

int main(int argc , char *argv[])
{

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    {// checking the alias command
    CheckExistGbFolder();
    FILE *lfilealias;
    lfilealias = fopen(".gb\\alias-commands.txt" , "r");
    if (lfilealias == NULL)
    {
        FILE *filealias;
        filealias = fopen("D:\\gb-project\\files\\alias-commands.txt" , "r");

        char line[1000];
        fgets(line , sizeof(line) , filealias);
        char *tokenPtr = strtok(line , ">");
        while (tokenPtr != NULL) 
        {
            if (!strncmp(tokenPtr , argv[1] , strlen(argv[1])))
            {
                tokenPtr = strtok(NULL, ">?");
                system(tokenPtr);   
                fclose(filealias);
                return 0;
            }
            tokenPtr = strtok(NULL, ">?");
        }
        
        fclose(filealias);
    }

    else
    {
        char line[1000];
        fgets(line , sizeof(line) , lfilealias);
        char *tokenPtr = strtok(line , ">");
        while (tokenPtr != NULL) 
        {
            if (!strncmp(tokenPtr , argv[1] , strlen(argv[1])))
            {
                tokenPtr = strtok(NULL, ">?");
                system(tokenPtr);   
                fclose(lfilealias);
                return 0;
            }
            tokenPtr = strtok(NULL, ">?");
        }
        
        fclose(lfilealias);
    }
    }
    chdir(cwd);
    
    if      (!strcmp(argv[1] , "config") &&  !strcmp(argv[2] , "user.name" )) //git config user.name " "
    {
        CheckExistGbFolder();
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
        CheckExistGbFolder();
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
            FILE *filealias;
            filealias = fopen("D:\\gb-project\\files\\alias-commands.txt" , "r");
            bool exist = 0;
            char line[1000];
            fgets(line , sizeof(line) , filealias);
            char *tokenPtr = strtok(line , ">");
            while (tokenPtr != NULL) 
            {
                if (!strcmp(tokenPtr , argv[3]+6))
                {
                    exist = 1;
                    break;;
                }
                tokenPtr = strtok(NULL, ">?");
            }

            if (exist == 0)
            {           
                FILE *file1;
                file1 = fopen("D:\\gb-project\\files\\alias-commands.txt" , "a");
                fprintf(file1 , ">%s>%s?" , argv[3]+6 , argv[4]);
                printf("global alias successfully created\n");
                fclose(file1);
            }

            else
                printf("This alias is already exists\n");
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
            CheckExistGbFolder();
            FILE *file1;
            file1 = fopen(".gb\\alias-commands.txt" , "a");
            if(file1 == NULL)
            {
                printf("fatal: not in a gb directory\n");
                return 0;
            }

            FILE *filealias;
            filealias = fopen(".gb\\alias-commands.txt" , "r");
            bool exist = 0;
            char line[1000];
            fgets(line , sizeof(line) , filealias);
            char *tokenPtr = strtok(line , ">");
            while (tokenPtr != NULL) 
            {
                if (!strcmp(tokenPtr , argv[3]+6))
                {
                    exist = 1;
                    break;;
                }
                tokenPtr = strtok(NULL, ">?");
            }

            if(exist == 0)
            {
                fprintf(file1 , ">%s>%s?" , argv[2]+6 , argv[3]);
                printf("local alias successfully created\n");
                fclose(file1);
            }
            else
                printf("This alias is already exists\n");
        }

        else
            printf("The command you are trying to add an alias to is invalid\n");
        
        fclose(file);
        
    }

    else if (!strcmp(argv[1] , "init")) // git init
    {
        char cwd1[1024];
        getcwd(cwd1, sizeof(cwd1));

        if (CheckExistGbFolder())
            printf("Gb repository already exists\n");
        
        else
        {
            chdir(cwd1);
            CreateDirectory(".gb" , NULL);
            SetFileAttributes(".gb" , FILE_ATTRIBUTE_HIDDEN);
            CreateDirectory(".gb\\stage" , NULL);
            CreateDirectory(".gb\\commits" , NULL);
            CreateDirectory(".gb\\commits\\master" , NULL);
            CreateDirectory(".gb\\stage\\laststage" , NULL);

            FILE *file;
            file = fopen(".gb\\commits\\curbranch.txt" , "w");
            fprintf(file , "master");
            fclose(file);

            printf("Initialized empty Gb repository\n");
        }
    }
    
    else if (!strcmp(argv[1] , "add" )) // git add
    {
        char cwd1[1024];
        getcwd(cwd1, sizeof(cwd1));

        if (!CheckExistGbFolder())
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
        
        else
        {
            if (!strcmp(argv[2] , "-n"))
            {
                chdir(cwd1);
                struct dirent *entry;
                DIR *dir = opendir(".");
                
                bool isstage = false;
                while ((entry = readdir(dir)) != NULL)
                {
                    if(entry->d_type == DT_REG)
                    {
                        CheckExistGbFolder();
                        struct dirent *stage;
                        DIR *dir1 = opendir(".gb\\stage");
                        while ((stage = readdir(dir1)) != NULL)
                        {
                            if (!strcmp(entry->d_name , stage->d_name))
                            {
                                isstage = true;
                            }
                            
                        }
                    if (isstage == true)
                        printf("%s > stage\n" , entry->d_name);
                    else
                        printf("%s > unstage\n" , entry->d_name);
                    }
                }
            }
            
            else
            {
                chdir(".gb\\stage\\laststage");
                struct dirent *lstage;
                DIR *dir1 = opendir(".");
                while ((lstage = readdir(dir1)) != NULL)
                {
                    if(lstage->d_type == DT_REG && strcmp(lstage->d_name , ".") && strcmp(lstage->d_name , ".."))
                        remove(lstage->d_name);
                }   

                int begin = 2;
                if(!strcmp(argv[2] , "-f"))
                    begin = 3;

                for (int i = begin; i < argc; i++)
                {
                    chdir(cwd1);
                    char pathh[100];
                    
                    sprintf(pathh , "%s\\%s" , cwd1 , argv[i]);

                    if(file_or_folder(pathh))
                    {
                        FILE *file;
                        file = fopen(argv[i] , "r");
                        if (file == NULL)
                            printf("file does not exist\n");

                        else
                        {
                            fclose(file);
                            CheckExistGbFolder();
                            char copy[50];
                            sprintf(copy , "copy %s\\%s .gb\\stage > nul" , cwd1 , argv[i]);
                            system(copy);
                            sprintf(copy , "copy %s\\%s .gb\\stage\\laststage > nul" , cwd1 , argv[i]);
                            system(copy);
                            printf("file added successfully\n");
                        }
                    }
                    else
                    {
                        struct dirent *entry;
                        DIR *dir = opendir(argv[i]);
                        if (dir == NULL) 
                            printf("folder does not exist\n");
                        
                        else
                        {
                            while ((entry = readdir(dir)) != NULL)
                            {
                                if(entry->d_type == DT_REG)
                                {
                                    char copy[50];
                                    sprintf(copy , "copy %s\\%s\\%s .gb\\stage > nul" , cwd1 , argv[i] , entry->d_name);
                                    system(copy);
                                    sprintf(copy , "copy %s\\%s\\%s .gb\\stage\\laststage > nul" , cwd1 , argv[i] , entry->d_name);
                                    system(copy);
                                    printf("%s added successfully\n", entry->d_name);
                                } 
                            }
                            closedir(dir);
                        }
                        
                    }
                }
            }
        }
    }
    
    else if (!strcmp(argv[1] , "reset" )) // git reset
    {
        char cwd1[1024];
        getcwd(cwd1, sizeof(cwd1));

        if (!CheckExistGbFolder())
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
        
        else
        {
            int begin = 2;
            if(!strcmp(argv[2] , "-f"))
                begin = 3;

            for (int i = begin; i < argc; i++)
            {
                chdir(cwd1);
                char pathh[100];
                
                sprintf(pathh , "%s\\%s" , cwd1 , argv[i]);

                if(file_or_folder(pathh))
                {
                    FILE *file;
                    file = fopen(argv[i] , "r");
                    if (file == NULL)
                        printf("file or folder does not exist\n");

                    else
                    {
                        fclose(file);
                        CheckExistGbFolder();
                        struct dirent *stage;
                        chdir(".gb\\stage");
                        DIR *dir1 = opendir(".");
                        replaceBackslash(argv[i]);
                        char *filename = getFileName(argv[i]);
                        bool isstage;
                        while ((stage = readdir(dir1)) != NULL)
                        {
                            if (!strcmp(filename , stage->d_name))
                            {
                                char delete[50];
                                isstage = true;
                                sprintf(delete , "del %s > nul" , filename);
                                system(delete);
                            }
                        }
                        if(isstage == true)
                            printf("%s unstaged successfully\n" , filename);
                        else
                            printf("%s is already unstage\n" , filename);
                    }
                }

                else
                {
                    struct dirent *entry;
                    DIR *dir = opendir(argv[i]);
                    if (dir == NULL) 
                        printf("folder does not exist\n");
                    
                    else
                    {
                        CheckExistGbFolder();

                        while ((entry = readdir(dir)) != NULL)
                        {
                            if(entry->d_type == DT_REG)
                            {
                                bool isstage = false;
                                struct dirent *stage;
                                chdir(".gb\\stage");
                                DIR *dir1 = opendir(".");
                                while ((stage = readdir(dir1)) != NULL)
                                {
                                    if (!strcmp(entry->d_name , stage->d_name))
                                    {
                                        isstage = true;
                                        remove(stage->d_name);
                                        printf("%s unstaged successfully\n" , entry->d_name);
                                    }
                                    
                                }
                                if(isstage == false)
                                    printf("%s is already unstage\n" , entry->d_name);
                                
                            } 
                        }
                        closedir(dir);
                    }
                    
                }
            }
            
        }
    }

    else if (!strcmp(argv[1] , "commit" )) // git commit
    {
        char cwd1[1024];
        getcwd(cwd1, sizeof(cwd1));

        if (!CheckExistGbFolder())
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
        
        else
        {
            bool stage = checkEmptyFolder(".gb\\stage");

            if (stage == true)
                printf("The stage is empty\n");
            
            else
            {
                if (argv[3] == NULL)
                    printf("error: the commit message is empty\n");

                else if (strlen(argv[3]) > 72)
                    printf("the commit message is out of size\n");
                
                else
                {
                    int which_name;
                    int which_email;

                    char name[20];
                    char email[50];

                    which_name = compareFileDates(".gb\\local-name.txt" , "D:\\gb-project\\files\\global-name.txt");
                    which_email = compareFileDates(".gb\\local-email.txt", "D:\\gb-project\\files\\global-email.txt");

                    printf("%d %d\n\n", which_name , which_email);
                    if (which_name == 1)
                    {
                        FILE *localname;
                        localname = fopen(".gb\\local-name.txt" , "r");
                        fgets(name , sizeof(name) , localname);
                        fclose(localname);
                    }

                    else
                    {
                        FILE *globalname;
                        globalname = fopen("D:\\gb-project\\files\\global-name.txt" , "r");
                        fgets(name , sizeof(name) , globalname);
                        fclose(globalname);
                    }
                    
                    if (which_email == 1)
                    {
                        FILE *localemail;
                        localemail = fopen(".gb\\local-email.txt" , "r");
                        fgets(email , sizeof(email) , localemail);
                        fclose(localemail);
                    }

                    else
                    {
                        FILE *globalemail;
                        globalemail = fopen("D:\\gb-project\\files\\global-email.txt" , "r");
                        fgets(email , sizeof(email) , globalemail);
                        fclose(globalemail);
                    }
                    

                    chdir(".gb\\commits");
                    FILE *file;
                    file = fopen("curbranch.txt" , "r");
                    char curbranch[20];
                    fgets(curbranch , sizeof(curbranch) , file);

                    struct dirent *branch;
                    DIR *dir1 = opendir(curbranch);

                    int max_name = 100;
                    while ((branch = readdir(dir1)) != NULL)
                    {
                        if(branch->d_type == DT_DIR && atoi(branch->d_name) > max_name)
                            max_name = atoi(branch->d_name);
                    }

                    closedir(dir1);
                    chdir(curbranch);
                    char commitid[10];
                    sprintf(commitid , "%d" , max_name+1);
                    CreateDirectory(commitid , NULL);

                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);


                    chdir(commitid);

                    FILE *commitinfo;
                    commitinfo = fopen("commitinfo.txt" , "w");
                    fprintf(commitinfo , "id: %s\nbranch: %s\nmessage: %s\nuser.name: %s\nuser.email: %s\ntime: %d-%02d-%02d %02d:%02d:%02d",
                                        commitid , curbranch , argv[3] , name , email ,
                                         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                                         tm.tm_hour, tm.tm_min, tm.tm_sec);
                    fclose(file);

                    chdir("..");


                    char copy[50];
                    sprintf(copy , "copy %d\\* %d > nul" , max_name , max_name+1);
                    system(copy);

                    CheckExistGbFolder();

                    sprintf(copy , "copy .gb\\stage\\* .gb\\commits\\%s\\%d > nul", curbranch , max_name+1);
                    system(copy);
                    
                    printf("Commit id:     %s\n" , commitid);
                    printf("Time:          %d-%02d-%02d %02d:%02d:%02d\n",
                        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                        tm.tm_hour, tm.tm_min, tm.tm_sec);
                    printf("commit messag: %s\n" , argv[3]);
                } 

            }
        }
        
    }

    return 0;
}