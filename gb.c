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

char *search_file_recursive(const char *dir_path, const char *file_name) {
    DIR *dir;
    struct dirent *entry;
    char *result = NULL;

    dir = opendir(dir_path);
    if (dir == NULL) 
    {
        perror("opendir");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".gb") == 0) 
            continue;

        char entry_path[1000];
        snprintf(entry_path, sizeof(entry_path), "%s\\%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) 
        {
            result = search_file_recursive(entry_path, file_name);
            if (result != NULL) 
                break;
        } 
        else if (entry->d_type == DT_REG && strcmp(entry->d_name, file_name) == 0) 
        {
            result = entry_path;
            break;
        }
    }

    closedir(dir);
    return result;
}

bool intorstr(char *input)
{
    input[strcspn(input, "\n")] = 0;
    bool is_number = 1;
    for (int i = 0; i < strlen(input); i++) 
    {
        if (!isdigit(input[i]))
        {
            is_number = 0;
            break;
        }
    }
    return is_number;
}

int checkWordInFile(const char *filePath, const char *word) 
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL) 
    {
        perror("Error opening file");
        return -1;
    }

    char buffer[1000];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        if (strstr(buffer, word) != NULL) 
        {
            found = 1;
            break;
        }
    }

    fclose(file);

    return found;
}

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
        if (entry->d_type == DT_REG && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
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
        file = fopen(".gb/local-email.txt" , "w"); 
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
            CreateDirectory(".gb\\stage\\laststage" , NULL);
            CreateDirectory(".gb\\stage\\unstage" , NULL);

            FILE *file;
            file = fopen(".gb\\commits\\curbranch.txt" , "w");
            fprintf(file , "master");
            fclose(file);

            FILE *file1;
            file1 = fopen(".gb\\commits\\branches.txt" , "w");
            fprintf(file1 , "master");
            fclose(file1);

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
            if (!strcmp(argv[2] , "-redo"))
            {
                char move[50];
                sprintf(move , "move .gb\\stage\\unstage\\* .gb\\stage > nul");
                system(move);
                printf("All unstaged files are back in stage mode\n");
            }

            else if (!strcmp(argv[2] , "-n"))
            {
                chdir(cwd1);
                struct dirent *entry;
                DIR *dir = opendir(".");
                
                while ((entry = readdir(dir)) != NULL)
                {
                    bool isstage = false;
                    if(entry->d_type == DT_REG)
                    {
                        CheckExistGbFolder();
                        struct dirent *stage;
                        DIR *dir1 = opendir(".gb\\stage");
                        while ((stage = readdir(dir1)) != NULL)
                        {
                            if (!strcmp(entry->d_name , stage->d_name))
                                isstage = true;
                            
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
                                    CheckExistGbFolder();
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
        
        else if (!strcmp(argv[2] , "-undo"))
        {
            chdir(".gb\\stage\\laststage");
            struct dirent *lstage;
            DIR *dir = opendir(".");
            while ((lstage = readdir(dir)) != NULL)
            {
                if(lstage->d_type == DT_REG)
                {
                    struct dirent *stage;
                    chdir("..");
                    DIR *dir1 = opendir(".");
                    while ((stage = readdir(dir1)) != NULL)
                    {
                        if (!strcmp(lstage->d_name , stage->d_name))
                        {
                            char move[50];
                            sprintf(move , "move %s unstage > nul" , stage->d_name);
                            system(move);
                        }
                    }
                    chdir("laststage");
                    
                } 
            }
            printf("The last staged files returned to unstaged mode\n");
        }
        
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
                                char move[50];
                                isstage = true;
                                sprintf(move , "move %s unstage > nul" , filename);
                                system(move);
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
                                        char move[50];
                                        sprintf(move , "move %s unstage > nul" , stage->d_name);
                                        system(move);
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
                    int countfiletocommit;
                    countfiletocommit = countFilesInDirectory(".gb\\stage");

                    int which_name;
                    int which_email;

                    char name[20];
                    char email[50];

                    which_name = compareFileDates(".gb\\local-name.txt" , "D:\\gb-project\\files\\global-name.txt");
                    which_email = compareFileDates(".gb\\local-email.txt", "D:\\gb-project\\files\\global-email.txt");

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

                    struct dirent *commits;
                    DIR *dir1 = opendir(".");

                    int max_name = 100;
                    while ((commits = readdir(dir1)) != NULL)
                    {
                        if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                            max_name = atoi(commits->d_name);
                    }

                    closedir(dir1);
                    char commitid[10];
                    sprintf(commitid , "%d" , max_name+1);
                    CreateDirectory(commitid , NULL);

                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);


                    chdir(commitid);

                    FILE *commitinfo;
                    commitinfo = fopen("commitinfo.txt" , "w");
                    fprintf(commitinfo , "id: %s\nbranch: %s\nmessage: %s\nuser.name: %s\nuser.email: %s\nnumber of files: %d\ntime: %d-%02d-%02d %02d:%02d:%02d",
                                        commitid , curbranch , argv[3] , name , email , countfiletocommit ,
                                         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                                         tm.tm_hour, tm.tm_min, tm.tm_sec);
                    fclose(file);

                    chdir("..");


                    char copy[50];
                    sprintf(copy , "copy %d\\* %d > nul" , max_name , max_name+1);
                    system(copy);

                    CheckExistGbFolder();

                    sprintf(copy , "move .gb\\stage\\* .gb\\commits\\%d > nul" , max_name+1);
                    system(copy);

                    FILE *file2;
                    file2 = fopen(".gb\\commits\\curcommit.txt" , "w");
                    fprintf(file2 , "100");
                    fclose(file2);
                    
                    printf("Commit id:     %s\n" , commitid);
                    printf("Time:          %d-%02d-%02d %02d:%02d:%02d\n",
                        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                        tm.tm_hour, tm.tm_min, tm.tm_sec);
                    printf("commit messag: %s\n" , argv[3]);
                } 

            }
        }
        
    }

    else if (!strcmp(argv[1] , "log" )) // git log
    {
        if (!CheckExistGbFolder())
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
        
        else if(argc > 2)
        {
            if (!strcmp(argv[2] , "-n"))
            {
                int n = atoi(argv[3]);

                chdir(".gb\\commits");
                
                struct dirent *commits;
                DIR *dir1 = opendir(".");

                int max_name = 100;
                while ((commits = readdir(dir1)) != NULL)
                {
                    if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                        max_name = atoi(commits->d_name);
                }

                closedir(dir1);

                while (1)
                {
                    if (n == 0)
                        break;

                    char commitid[10];
                    sprintf(commitid , "%d" , max_name);
                    char pathtoopen[50];
                    sprintf(pathtoopen , "%s\\commitinfo.txt" , commitid);
                    FILE *file1;
                    file1 = fopen(pathtoopen , "r");
                    if (file1 == NULL)
                        break;
                    char ch;
                    while((ch = fgetc(file1)) != EOF)
                        printf("%c" , ch);
                    printf("\n\n");
                    fclose(file1);
                    max_name--;
                    n--;
                }
            }

            else if (!strcmp(argv[2] , "-branch") || !strcmp(argv[2] , "-author") || !strcmp(argv[2] , "-search"))
            {
                chdir(".gb\\commits");
                
                struct dirent *commits;
                DIR *dir1 = opendir(".");

                int max_name = 100;
                while ((commits = readdir(dir1)) != NULL)
                {
                    if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                        max_name = atoi(commits->d_name);
                }

                closedir(dir1);

                while (1)
                {
                    char commitid[10];
                    sprintf(commitid , "%d" , max_name);
                    char pathtoopen[50];
                    sprintf(pathtoopen , "%s\\commitinfo.txt" , commitid);
                    FILE *file1;
                    file1 = fopen(pathtoopen , "r");
                    if (file1 == NULL)
                        break;
                    char ch;

                    bool existword = checkWordInFile(pathtoopen , argv[3]);

                    if (existword)
                    {
                        while((ch = fgetc(file1)) != EOF)
                            printf("%c" , ch);
                        printf("\n\n");
                    }
                    fclose(file1);
                    max_name--;
                }
            }
            
        }

        else
        {
            chdir(".gb\\commits");
            FILE *file;
            file = fopen("curbranch.txt" , "r");
            char curbranch[20];
            fgets(curbranch , sizeof(curbranch) , file);
            struct dirent *commits;
            DIR *dir1 = opendir(".");

            int max_name = 100;
            while ((commits = readdir(dir1)) != NULL)
            {
                if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                    max_name = atoi(commits->d_name);
            }

            closedir(dir1);

            while (1)
            {
                char commitid[10];
                sprintf(commitid , "%d" , max_name);
                char pathtoopen[50];
                sprintf(pathtoopen , "%s\\commitinfo.txt" , commitid);
                FILE *file1;
                file1 = fopen(pathtoopen , "r");
                if (file1 == NULL)
                    break;
                char ch;
                while((ch = fgetc(file1)) != EOF)
                    printf("%c" , ch);
                printf("\n\n");
                fclose(file1);
                max_name--;
            }
        }

    }

    else if (!strcmp(argv[1] , "branch" )) // git branch
    {
        if (!CheckExistGbFolder())
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
        
        else if (argc == 3)
        {
            bool exist = checkWordInFile(".gb\\commits\\branches.txt" , argv[2]);

            if (exist)
                printf("This branch already exists\n");

            else
            {
                FILE *file;
                file = fopen(".gb\\commits\\branches.txt" , "a");
                fprintf(file , "\n");
                fprintf(file , argv[2]);
                fclose(file);
                printf("Branch created successfully\n");
            }

        }
        
        else
        {
            FILE *file1;
            file1 = fopen(".gb\\commits\\branches.txt" , "r");
            char ch;
            while((ch = fgetc(file1)) != EOF)
                printf("%c" , ch);
            printf("\n");
        }
        
    }

    else if (!strcmp(argv[1] , "checkout" )) // git checkout
    {
        if (!CheckExistGbFolder())
        {
            printf("fatal: not a Gb repository (or any of the parent directories)\n");
            return 0;
        }
       
        bool stage = checkEmptyFolder(".gb\\stage");

        if (stage == false)
        {
            printf("Error: The stage is not empty\n");
            return 0;
        }

        bool isnumber = intorstr(argv[2]);

        if (isnumber)
        {
            chdir(".gb\\commits");
                
            struct dirent *commits;
            DIR *dir1 = opendir(".");

            int max_name = 100;
            while ((commits = readdir(dir1)) != NULL)
            {
                if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                    max_name = atoi(commits->d_name);
            }

            closedir(dir1);

            while (1)
            {
                char commitid[10];
                sprintf(commitid , "%d" , max_name);
                if (!strcmp(commitid , argv[2]))
                {
                    struct dirent *head;
                    DIR *dir1 = opendir(commitid);

                    while ((head = readdir(dir1)) != NULL)
                    {
                        if(head->d_type == DT_REG && strcmp(head->d_name , "commitinfo.txt") && strcmp(head->d_name , ".") && strcmp(head->d_name , ".."))
                        {
                            char cwd[1024];
                            getcwd(cwd, sizeof(cwd));

                            CheckExistGbFolder();

                            char cwd1[1024];
                            getcwd(cwd1, sizeof(cwd1));
                            char *toPaste = search_file_recursive(cwd1 , head->d_name);

                            chdir(cwd);
                            
                            char copy[50];
                            sprintf(copy , "copy %s\\%s %s > nul" , commitid , head->d_name , toPaste);
                            system(copy);
                        }
                    }
                    printf("checkout to commit successfully\n");
                    break;
                }
                max_name--;
            }
        }

        else
        {
            if (!strcmp(argv[2] , "HEAD"))
            {
                chdir(".gb\\commits");

                char head[20];
                FILE *curbranch;
                curbranch = fopen("curbranch.txt" , "r");
                fgets(head , sizeof(head) , curbranch);
                fclose(curbranch);
                    
                struct dirent *commits;
                DIR *dir1 = opendir(".");

                int max_name = 100;
                while ((commits = readdir(dir1)) != NULL)
                {
                    if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                        max_name = atoi(commits->d_name);
                }

                closedir(dir1);

                while (1)
                {
                    char commitid[10];
                    sprintf(commitid , "%d" , max_name);
                    char pathtoopen[50];
                    sprintf(pathtoopen , "%s\\commitinfo.txt" , commitid);
                    FILE *file1;
                    file1 = fopen(pathtoopen , "r");
                    if (file1 == NULL)
                        break;

                    bool existword = checkWordInFile(pathtoopen , head);

                    if (existword)
                    {
                        struct dirent *head;
                        DIR *dir1 = opendir(commitid);

                        while ((head = readdir(dir1)) != NULL)
                        {
                            if(head->d_type == DT_REG && strcmp(head->d_name , "commitinfo.txt") && strcmp(head->d_name , ".") && strcmp(head->d_name , ".."))
                            {
                                char cwd[1024];
                                getcwd(cwd, sizeof(cwd));

                                CheckExistGbFolder();

                                char cwd1[1024];
                                getcwd(cwd1, sizeof(cwd1));
                                char *toPaste = search_file_recursive(cwd1 , head->d_name);

                                chdir(cwd);
                                
                                char copy[50];
                                sprintf(copy , "copy %s\\%s %s > nul" , commitid , head->d_name , toPaste);
                                system(copy);
                            }
                        }
                        break;
                    }
                    max_name--;
                }
                printf("checkout to branch successfully\n");
            }
            
            else
            {
                chdir(".gb\\commits");
                    
                FILE *file;
                file = fopen("curbranch.txt" , "w");
                fprintf(file , argv[2]);
                fclose(file);

                struct dirent *commits;
                DIR *dir1 = opendir(".");

                int max_name = 100;
                while ((commits = readdir(dir1)) != NULL)
                {
                    if(commits->d_type == DT_DIR && atoi(commits->d_name) > max_name)
                        max_name = atoi(commits->d_name);
                }

                closedir(dir1);

                while (1)
                {
                    char commitid[10];
                    sprintf(commitid , "%d" , max_name);
                    char pathtoopen[50];
                    sprintf(pathtoopen , "%s\\commitinfo.txt" , commitid);
                    FILE *file1;
                    file1 = fopen(pathtoopen , "r");
                    if (file1 == NULL)
                        break;

                    bool existword = checkWordInFile(pathtoopen , argv[2]);

                    if (existword)
                    {
                        struct dirent *head;
                        DIR *dir1 = opendir(commitid);

                        while ((head = readdir(dir1)) != NULL)
                        {
                            if(head->d_type == DT_REG && strcmp(head->d_name , "commitinfo.txt") && strcmp(head->d_name , ".") && strcmp(head->d_name , ".."))
                            {
                                char cwd[1024];
                                getcwd(cwd, sizeof(cwd));

                                CheckExistGbFolder();

                                char cwd1[1024];
                                getcwd(cwd1, sizeof(cwd1));
                                char *toPaste = search_file_recursive(cwd1 , head->d_name);

                                chdir(cwd);
                                
                                char copy[50];
                                sprintf(copy , "copy %s\\%s %s > nul" , commitid , head->d_name , toPaste);
                                system(copy);
                            }
                        }
                        break;
                    }
                    max_name--;
                }
                printf("checkout to branch successfully\n");
            }
        }
    }

    return 0;
}