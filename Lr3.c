#include <stdio.h>
#include <malloc.h>

struct Node {
	void *value;
	struct Node *next;
	struct Node *prev;
};
struct List {
	size_t list_size;
	struct Node *head;
	struct Node *tail;
};

struct List* create_list() {
	struct List *tmp = (struct List*) malloc(sizeof(struct List));
	tmp->list_size = 0;
	tmp->head = NULL;
	tmp->tail = NULL;
	return tmp;
}

int main() {
	struct List *list = create_list();
	while (1)
	{
		char* data = (char*) malloc(sizeof(char) * 256);
		scanf("%s", data);
		struct Node *node = (struct Node*) malloc(sizeof(struct Node));
        node->value = data;
        node->next = NULL;
        node->prev = list->tail;

        if (list->tail) {
            list->tail->next = node;
        }
        list->tail = node;

        if (list->head == NULL) {
            list->head = node;
        }
        list->list_size++;
	}

	if (!is_empty(list)) {
		void *node = list->head->value;
        printf("%d ", *((int*)node));
        if (list->head->next != NULL) {
            print_list(list->head->next);
        }
	}

	return 0;
}
