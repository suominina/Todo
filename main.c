#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define FILE_NAME "data.txt"

struct todo {
    int num;
    char content[1024];
};

int initialize_todo_list(struct todo *todo_list);
void show_commands_list();
void show_tasks(struct todo *todo_list);
void add_task(struct todo *todo_list, int num_of_tasks);
void delete_task(struct todo *todo_list, int num_of_tasks);


int main(int argc, char **argv)
{
    struct todo todo_list[40];
    char buf[1024];
    int cmd;
    int num_of_tasks = 0;

    show_commands_list();
    num_of_tasks = initialize_todo_list(&todo_list[0]);
    for (;;) {
        fprintf(stdout, "> ");
        fscanf(stdin, "%s", buf);
        cmd = atoi(buf);
        num_of_tasks = initialize_todo_list(&todo_list[0]);

        switch(cmd) {
            case 0:
                exit(EXIT_SUCCESS);
            case 1:
                show_commands_list();
                break;
            case 2:
                show_tasks(&todo_list[0]);
                break;
            case 3:
                add_task(&todo_list[0], num_of_tasks);
                num_of_tasks++;
                break;
            case 4:
                delete_task(&todo_list[0], num_of_tasks);
                num_of_tasks--;
                break;
            default:
                show_commands_list();
                break;
        }
    }

    return 0;
}

int initialize_todo_list(struct todo *todo_list) {
    FILE *f;
    char buf[1024];
    if (!(f = fopen(FILE_NAME, "r"))) {
        fprintf(stderr, "file not found");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while ((fgets(buf, sizeof(buf), f)) != NULL) {
        strcpy((todo_list+i)->content, buf);
        (todo_list+i)->num = i;
        i++;
    }

    return i;
}

void show_commands_list()
{
    puts("---------------");
    puts("1: show commands");
    puts("2: show tasks");
    puts("3: add a new task");
    puts("4: delete a task");
    puts("0: exit");
    puts("---------------");
}


void show_tasks(struct todo *todo_list)
{
    FILE *f;
    if (!(f = fopen(FILE_NAME, "r"))) {
        fprintf(stderr, "file not found");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    int i = 1;
    while (fgets(buf, sizeof buf, f) != NULL) {
        fprintf(stdout, "%d ", i);
        fputs(buf, stdout);
        i++;
    }

    fclose(f);
}

void add_task(struct todo *todo_list, int num_of_tasks)
{
    FILE *f;
    char buf[1024];
    if (!(f = fopen(FILE_NAME, "a"))) {
        fprintf(stderr, "file not found");
        exit(EXIT_FAILURE);
    }

    int idx = num_of_tasks;

    fflush(stdin); // fgets() doesnt work without this
    printf("> ");
    fgets(buf, sizeof(buf), stdin);
    strcpy((todo_list+idx)->content, buf);
    (todo_list+idx)->num = num_of_tasks+1;
    fprintf(f, "%s", (todo_list+idx)->content);

    puts("added a new task");

    fclose(f);
}

void delete_task(struct todo *todo_list, int num_of_tasks)
{
    FILE *f;
    char buf[100];
    int task_tobe_deleted;

    puts("task number to delete");
    fprintf(stdout, "> ");
    fscanf(stdin, "%s", buf);
    task_tobe_deleted = atoi(buf);

    if (!(f = fopen(FILE_NAME, "w"))) {
        fprintf(stderr, "file not found");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_of_tasks; i++) {
        if ((todo_list+i)->num != (task_tobe_deleted-1)) {
            fprintf(f, "%s", (todo_list+i)->content);
        }
    }

    puts("task deleted");
    fclose(f);
}
