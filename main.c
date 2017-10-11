#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 100
#define MAX_WORD_SIZE 50

typedef struct list
{
  char word[MAX_WORD_SIZE];
  int count;
  struct list *next;
} list;

//добавление узла в список
void add_node(list **head, char *word_buff)
{
    list *tmp = (list*) malloc(sizeof(list));
    strcpy(tmp->word, word_buff);
    tmp->count++;
    tmp->next = (*head);
    (*head) = tmp;
}

//удаление списка
void delete_list(list **head)
{
    list* prev = NULL;
    while ((*head)->next)
    {
        prev = (*head);
        (*head) = (*head)->next;
        free(prev);
    }
    free(*head);
}

//вывод списка
void print_list(const list *head)
{
    while (head)
    {
        printf("%s %d\n", head->word, head->count);
        head = head->next;
    }
    printf("\n");
}

int check_list(list *head, char *word_buff)
{
    int j = 0;
    while(head)
    {
        if(strcmp(head->word, word_buff) == 0)
        {
            head->count = head->count + 1;
            j++;
        }
        head = head->next;
    }
    return j;
}

void merge(list *a, list *b, list **c) {
    list tmp;
    *c = NULL;
    if (a == NULL) {
        *c = b;
        return;
    }
    if (b == NULL) {
        *c = a;
        return;
    }
    if (a->count < b->count) {
        *c = a;
        a = a->next;
    } else {
        *c = b;
        b = b->next;
    }
    tmp.next = *c;
    while (a && b) {
        if (a->count < b->count) {
            (*c)->next = a;
            a = a->next;
        } else {
            (*c)->next = b;
            b = b->next;
        }
        (*c) = (*c)->next;
    }
    if (a) {
        while (a) {
            (*c)->next = a;
            (*c) = (*c)->next;
            a = a->next;
        }
    }
    if (b) {
        while (b) {
            (*c)->next = b;
            (*c) = (*c)->next;
            b = b->next;
        }
    }
    *c = tmp.next;
}

void split(list *src, list **low, list **high) {
    list *fast = NULL;
    list *slow = NULL;

    if (src == NULL || src->next == NULL) {
        (*low) = src;
        (*high) = NULL;
        return;
    }

    slow = src;
    fast = src->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            fast = fast->next;
            slow = slow->next;
        }
    }

    (*low) = src;
    (*high) = slow->next;
    slow->next = NULL;
}

void mergeSort(list **head) {
    list *low  = NULL;
    list *high = NULL;
    if ((*head == NULL) || ((*head)->next == NULL)) {
        return;
    }
    split(*head, &low, &high);
    mergeSort(&low);
    mergeSort(&high);
    merge(low, high, head);
}

int main()
{
    char file_path[MAX_PATH_SIZE];

    printf("Enter the path of the file: ");
    scanf("%s", file_path);

    FILE *file;
    file = fopen(file_path, "r");

    if (!file)
    {
        printf("\nCan't open file!\n");
        exit(1);
    }

    char buff = NULL;
    list *head = NULL;
    int i = 0;

    while(buff != EOF)
    {
        int k = 0;      //счетчик для word_buff
        char word_buff[MAX_WORD_SIZE] = {};

        while(isspace((buff = fgetc(file))) == 0 && ispunct(buff) == 0 && buff != EOF)
        {
            word_buff[k] = buff;
            k++;
        }
        if(i == 0)
        {
            add_node(&head, word_buff);
            i++;
        }
        else
        {
            if(check_list(head, word_buff) == 0)
            {
                add_node(&head, word_buff);
            }
        }
    }
    mergeSort(&head);
    print_list(head);
    delete_list(&head);
    fclose(file);
    return 0;
}
