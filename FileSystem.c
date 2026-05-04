#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 100
#define MAX_CONTENT_LENGTH 1000
#define MAX_ITEMS 1000

typedef struct File File;
typedef struct Directory Directory;

struct File {
    char name[MAX_NAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    Directory *parent;
};

struct Directory {
    File *files[MAX_ITEMS];
    Directory *dirs[MAX_ITEMS];
    Directory *parent;
    char name[MAX_NAME_LENGTH];
    int num_files;
    int num_dirs;
};

Directory *root;
Directory *current;

static void pwd_recursive(Directory *dir) {
    if (dir == NULL) {
        return;
    }
    if (dir->parent != NULL) {
        pwd_recursive(dir->parent);
    }
    printf("/%s", dir->name);
}

// Initialize the file system
// The file system will be in the shape of a tree
Directory *create_directory(const char *name, Directory *parent) {
    Directory *dir = (Directory *)malloc(sizeof(Directory));
    if (dir == NULL) {
        fprintf(stderr, "Failed to allocate directory\n");
        exit(1);
    }
    memset(dir, 0, sizeof(Directory));
    strcpy(dir->name, name);
    dir->parent = parent;
    dir->num_files = 0;
    dir->num_dirs = 0;
    return dir;
}

File *create_file(const char *name, Directory *parent) {
    File *file = (File *)malloc(sizeof(File));
    if (file == NULL) {
        fprintf(stderr, "Failed to allocate file\n");
        exit(1);
    }
    strcpy(file->name, name);
    file->parent = parent;
    file->content[0] = '\0';
    return file;
}

void print_working_directory(void) {
    pwd_recursive(current);
    printf("\n");
}

void ls(void) {
    printf("Directories:\n");
    for (int i = 0; i < current->num_dirs; i++) {
        printf("%s\n", current->dirs[i]->name);
    }
    printf("\nFiles:\n");
    for (int i = 0; i < current->num_files; i++) {
        printf("%s\n", current->files[i]->name);
    }
}

void mkdir(const char *name) {
    if (current->num_dirs >= MAX_ITEMS) {
        printf("You can't add any more directories to this directory, there is too much !\n");
        return;
    }
    current->dirs[current->num_dirs] = create_directory(name, current);
    current->num_dirs++;
}

void touch(const char *name) {
    if (current->num_files >= MAX_ITEMS) {
        printf("File limit reached!\n");
        return;
    }
    current->files[current->num_files] = create_file(name, current);
    current->num_files++;
}

void edit(char *name) {
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, name) == 0) {
            printf("Enter the new content for the file %s:\n", name);
            /* fgets is used to get input from the user */
            fgets(current->files[i]->content, MAX_CONTENT_LENGTH, stdin);
            return;
        }
    }
    printf("File %s not found\n", name);
}

void remove_file(char *name) {
    for (int i = 0; i < current->num_files; i++) {
        if (strcmp(current->files[i]->name, name) == 0) {
            free(current->files[i]);
            current->num_files--;
            return;
        }
    }
    printf("File %s not found\n", name);
}

void remove_directory(char *name) {
    for (int i = 0; i < current->num_dirs; i++) {
        if (strcmp(current->dirs[i]->name, name) == 0) {
            free(current->dirs[i]);
            current->num_dirs--;
            return;
        }
    }
    printf("Directory %s not found\n", name);
}

void cd(const char *name) {
    /* if the user types in ".." it takes them up to their parent directory if they have one */
    if (strcmp(name, "..") == 0) {
        if (current->parent != NULL) {
            current = current->parent;
            return;
        }
    }
    for (int i = 0; i < current->num_dirs; i++) {
        if (strcmp(current->dirs[i]->name, name) == 0) {
            current = current->dirs[i];
            return;
        }
    }
    printf("Directory not found\n");
}

void cat(const char *name) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(current->files[i]->name, name) == 0) {
            printf(current->files[i]->content);
            printf("\n");
            return;
        }
    }
    printf("That file doesn't exist!");
}

int main(void) {
    root = create_directory("root", NULL);
    current = root;

    char input[250];

    while (1) {
        printf("\n> ");
        if (fgets(input, (int)sizeof input, stdin) == NULL) {
            break;
        }

        /* Split on whitespace so "mkdir bruh" -> cmd="mkdir", arg="bruh" */
        char *cmd = strtok(input, " \t\n");
        char *arg = strtok(NULL, " \t\n");

        if (!cmd) {
            continue;
        }

        else if (strcmp(cmd, "pwd") == 0) {
            print_working_directory();
            continue;
        }
        else if (strcmp(cmd, "ls") == 0) {
            ls();
            continue;
        }
        else if (strcmp(cmd, "mkdir") == 0 && arg) {
            mkdir(arg);
            continue;
        }
        else if (strcmp(cmd, "rmdir") == 0 && arg) {
            remove_directory(arg);
            continue;
        }
        else if (strcmp(cmd, "touch") == 0 && arg) {
            touch(arg);
            continue;
        }
        else if (strcmp(cmd, "edit") == 0 && arg) {
            edit(arg);
            continue;
        }
        else if (strcmp(cmd, "rm") == 0 && arg) {
            remove_file(arg);
            continue;
        }
        else if (strcmp(cmd, "cd") == 0 && arg) {
            cd(arg);
            continue;
        }
        else if (strcmp(cmd, "exit") == 0) {
            break;
        }
        else if (strcmp(cmd, "cat") == 0 && arg) {
            cat(arg);
            continue;
        }
        else {
            printf("Unknown Command! Try `help` if you want the list of commands!");
        }

    }

    return 0;
}
