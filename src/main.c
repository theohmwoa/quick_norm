/*
** EPITECH PROJECT, 2022
** tabs_to_space
** File description:
** main
*/

#include "my.h"

bool end_of_line(char *str, int i)
{
    for (; str[i] != '\0'; i++) {
        if (str[i] == '\n')
            return (true);
        if (str[i] != ' ' && str[i] != '\t')
            return (false);
    }
    return (true);
}

char *exchange_tabs(char *str)
{
    int buffsize = 8096;
    char *buff = malloc(buffsize);
    int index = 0;
    if (buff == NULL)
        return (NULL);
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] == '\t' || str[i] == ' ') && end_of_line(str, i))
            continue;
        if (str[i] == '\n' && str[i + 1] == '\n' && str[i + 2] == '\n')
            continue;
        if (str[i] == '\t') {
            buff[index] = ' ';
            index++;
            buff[index] = ' ';
            index++;
            buff[index] = ' ';
            index++;
            buff[index] = ' ';
            index++;
        } else {
            buff[index] = str[i];
            index++;
        }
        if (index >= buffsize) {
            buffsize += 8096;
            buff = realloc(buff, buffsize);
            if (buff == NULL)
                return (NULL);
        }
    }
    buff[index] = '\0';
    return (buff);
}

char *read_file(char *filepath)
{
    char *buffer = NULL;
    int fd = open(filepath, O_RDONLY);
    int size = 0;
    if (fd == -1)
        return (NULL);
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    buffer = malloc(sizeof(char) * (size + 1));
    read(fd, buffer, size);
    buffer[size] = '\0';
    close(fd);
    return (buffer);
}

int write_new_content(char *filepath)
{
    char *buffer = read_file(filepath);
    if (buffer == NULL)
        return (84);
    char *new_buffer = exchange_tabs(buffer);
    if (new_buffer == NULL)
        return (84);
    int fd = open(filepath, O_WRONLY | O_TRUNC);
    if (fd == -1)
        return (84);
    write(fd, new_buffer, strlen(new_buffer));
    if (new_buffer[strlen(new_buffer) - 1] != '\n')
        write(fd, "\n", 1);
    close(fd);
    if (strcmp(buffer, new_buffer) != 0)
        printf("Fixed: \033[0;34m%s\033[0m\n", filepath);
    free(buffer);
    free(new_buffer);
    return 0;
}

int contains_str(char *str, char *to_find)
{
    int i = 0;
    int j = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] == to_find[j]) {
            j++;
            if (to_find[j] == '\0')
                return (i - j + 1);
        } else
            j = 0;
    }
    return (-1);
}

void open_folder(char *folderpath)
{
    DIR *dir = opendir(folderpath);
    struct dirent *file;
    if (dir == NULL)
        return;
    while ((file = readdir(dir)) != NULL) {
        if (file->d_name[0] == '.')
            continue;
        if (file->d_type == DT_DIR) {
            char *new_folderpath = malloc(sizeof(char) * (strlen(folderpath) + strlen(file->d_name) + 2));
            if (new_folderpath == NULL)
                return;
            new_folderpath[0] = '\0';
            strcat(new_folderpath, folderpath);
            strcat(new_folderpath, "/");
            strcat(new_folderpath, file->d_name);
            open_folder(new_folderpath);
            free(new_folderpath);
        } else {
            char *new_filepath = malloc(sizeof(char) * (strlen(folderpath) + strlen(file->d_name) + 2));
            if (new_filepath == NULL)
                return;
            new_filepath[0] = '\0';
            strcat(new_filepath, folderpath);
            strcat(new_filepath, "/");
            strcat(new_filepath, file->d_name);
            if (contains_str(file->d_name, ".c") == -1)
                printf("Skipped: \033[0;31m%s\033[0m\n", new_filepath);
            else
                write_new_content(new_filepath);
            free(new_filepath);
        }
    }
    closedir(dir);
}

int main(int ac, char **av)
{
    setbuf(stdout, NULL);
    if (ac == 2) {
        if (av[1][0] == '-' && av[1][1] == 'h') {
            puts("USAGE\n\t./tab_to_space [-h] [folder ...]\nDESCRIPTION\n\t-h\t\tDisplay the usage and quit.");
            return (0);
        }
    }
    for (int i = 1; i < ac; i++) {
        if (av[i][0] == '-')
            continue;
        open_folder(av[i]);
    }
    return (0);
}
