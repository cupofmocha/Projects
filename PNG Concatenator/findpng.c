#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>  /* for printf().  man 3 printf */
#include <stdlib.h> /* for exit().    man 3 exit   */
#include <string.h> /* for strcat().  man strcat   */

#define MAX_FILE_NUM 1000

char **listFiles(char *argv)
{
    DIR *p_dir;
    struct dirent *p_dirent;
    char str[64];

    char **list_of_filenames = (char **)malloc(MAX_FILE_NUM * sizeof(char));
    int file_counter = 0;

    if ((p_dir = opendir(argv)) == NULL)
    {
        sprintf(str, "opendir(%s)", argv);
        perror(str);
        exit(2);
    }

    while ((p_dirent = readdir(p_dir)) != NULL)
    {
        char *str_path = p_dirent->d_name; /* relative path name! */
        list_of_filenames[file_counter] = malloc(strlen(p_dirent->d_name) + 1);

        strcpy(list_of_filenames[file_counter], str_path);

        file_counter++;
    }

    list_of_filenames[file_counter] = NULL;

    if (closedir(p_dir) != 0)
    {
        perror("closedir");
        exit(3);
    }

    return list_of_filenames;
}

int checkIsPNG(char *str_path)
{
    char s[100];

    char value;
    char str[4];
    int count = 0;

    //open file command
    FILE *fp; //file pointer
    fp = fopen(str_path, "rb");

    //read file command
    while (fread(&value, sizeof(char), 1, fp))
    {
        str[count] = value;
        count++;
        if (count == 4)
            break; //we only need the first 4 bytes to validate whether the file is a real PNG
    }

    fclose(fp);
    // checking the appropriate bytes to validate whether it is a real PNG file
    if (str[1] == 'P' && str[2] == 'N' && str[3] == 'G')
    {
        return 1;
    }
    return 0;
}

char *fileType(char *item)
{
    int i;
    char *ptr;
    struct stat buf;

    if (lstat(item, &buf) < 0)
    {
        perror("lstat error");
    }

    if (S_ISREG(buf.st_mode))
        ptr = "regular";
    else if (S_ISDIR(buf.st_mode))
        ptr = "directory";
    else if (S_ISCHR(buf.st_mode))
        ptr = "character special";
    else if (S_ISBLK(buf.st_mode))
        ptr = "block special";
    else if (S_ISFIFO(buf.st_mode))
        ptr = "fifo";
#ifdef S_ISLNK
    else if (S_ISLNK(buf.st_mode))
        ptr = "symbolic link";
#endif
#ifdef S_ISSOCK
    else if (S_ISSOCK(buf.st_mode))
        ptr = "socket";
#endif
    else
        ptr = "**unknown mode** \n";

    return ptr;
}

int findPng(char *directory, int *ptrValue)
{
    char **listOfFileNames = listFiles(directory);
    int counter = 0;
    int i;
    char *ptr;

    chdir(directory);

    for (i = 2; listOfFileNames[i] != NULL; ++i)
    {
        ptr = fileType(listOfFileNames[i]);

        if (ptr == "directory")
        {
            findPng(listOfFileNames[i], ptrValue);
        }
        else if (ptr == "regular")
        {
            if (checkIsPNG(listOfFileNames[i]))
            {
                char abspath[256] = "";
                getcwd(abspath, 256);
                printf("%s/%s\n", strrchr(abspath, '/'), listOfFileNames[i]);
                *ptrValue = *ptrValue + 1;
            }
        }
    }
    chdir("..");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }
    int value = 0;

    findPng(argv[1], &value);

    if (value == 0)
    {
        printf("findpng: No PNG file found");
    }

    return 0;
}