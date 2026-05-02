#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 100
#define MAX_CONTENT_LENGTH 1000
#define MAX_ITEMS 1000

typedef struct {
    char name[MAX_NAME_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    Directory *parent;
} File;

typedef struct {
    File *items[MAX_ITEMS];
    Directory *parent;
    char name[MAX_NAME_LENGTH];
    int num_items;
} Directory;

Directory *root;

// Initialize the file system
// The file system will be in the shape of a tree
void init_filesystem(void) {
    root = (Directory *)malloc(sizeof(Directory));
    if (root == NULL) {
        fprintf(stderr, "Failed to allocate memory for root directory gg\n")
        exit(1);
    }
    strcpy(root->name, "/");
    root->parent = NULL;
    root->num_items = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        root->items[i] = NULL;
    }
}

void create_file(const char *name, const char *content) {
    File *file = (File *)malloc(sizeof(File));
    if (file == NULL) {
        fprintf(stderr, "Failed to allocate memory for file %s\n", name);
        exit(1);
    }
    strcpy(file->name, name);
    strcpy(file->content, content);
    file->parent = root;
    root->items[root->num_items++] = file;
}

void create_directory(const char *name) {
    Directory *dir = (Directory *)malloc(sizeof(Directory));
    if (dir == NULL) {
        fprintf(stderr, "Failed to allocate memory for directory %s\n", name);
        exit(1);
    }
    strcpy(dir->name, name);
    dir->parent = root;
    dir->num_items = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        dir->items[i] = NULL;
    }
    root->items[root->num_items++] = dir;
}

void delete_file(const char *name) {
    for (int i = 0; i < root->num_items; i++) {
        if (strcmp(root->items[i]->name, name) == 0) {
            free(root->items[i]);
            root->items[i] = NULL;
            root->num_items--;
            return;
        }
    }
    fprintf(stderr, "File %s not found\n", name);
    exit(1);
}

void delete_directory(const char *name) {
    for (int i = 0; i < root->num_items; i++) {
        if (strcmp(root->items[i]->name, name) == 0) {
            free(root->items[i]);
            root->items[i] = NULL;
            root->num_items--;
            return;
        }
    }
    fprintf(stderr, "Directory %s not found\n", name);
    exit(1);
}