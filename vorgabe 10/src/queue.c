
#include "../lib/queue.h"
#include <stdlib.h>

int queue_add(void *new_object, queue_object *queue)
{

            if (queue == NULL || new_object == NULL) {
                        return 1;
                }

                    queue_object *new_elem = malloc(sizeof(queue_object));
            if (new_elem == NULL) {
                        return 1;
                }

                    new_elem->object = new_object;
            new_elem->next = NULL;

                    queue_object *iter = queue;
            while (iter->next != NULL) {
                        iter = iter->next;
                }
            iter->next = new_elem;
            return 0;
}

void *queue_poll(queue_object *queue)
{

            if (queue == NULL || queue->next == NULL) {
                        return NULL;
                }

                    queue_object *first = queue->next;
            void *obj = first->object;
            queue->next = first->next;
            free(first);
            return obj;
}

queue_object *new_queue()
{

            queue_object *queue = malloc(sizeof(queue_object));
            if (queue == NULL) {
                        return NULL;
                }
            queue->object = NULL;
            queue->next = NULL;
            return queue;
}

void free_queue(queue_object *queue)
{

                    if (queue == NULL)
                        return;

                    queue_object *iter = queue->next;
            while (iter != NULL) {
                        queue_object *next = iter->next;
                        free(iter);
                        iter = next;
                }
            free(queue);
}

void *queue_peek(queue_object *queue)
{

            if (queue == NULL || queue->next == NULL) {
                        return NULL;
                }
            return queue->next->object;
}


void *queue_poll_tail(queue_object *queue)
{

            if (queue == NULL || queue->next == NULL) {
                        return NULL;
                }

                    queue_object *prev = queue;
            queue_object *curr = queue->next;
            while (curr->next != NULL) {
                        prev = curr;
                        curr = curr->next;
                }
            prev->next = NULL;
            void *obj = curr->object;
            free(curr);
            return obj;
}



