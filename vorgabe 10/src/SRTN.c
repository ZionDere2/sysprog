#include "../lib/SRTN.h"

static queue_object *SRTN_queue;
//You can add more global variables here

static process *poll_shortest()
{
        if (SRTN_queue == NULL || SRTN_queue->next == NULL)
                return NULL;

        queue_object *prev = SRTN_queue;
        queue_object *curr = SRTN_queue->next;
        queue_object *best_prev = prev;
        unsigned int best_time = ((process *)curr->object)->time_left;

        while (curr != NULL) {
                process *p = curr->object;
                if (p->time_left < best_time) {
                        best_time = p->time_left;
                        best_prev = prev;
                }
                prev = curr;
                curr = curr->next;
        }

        queue_object *best = best_prev->next;
        best_prev->next = best->next;
        process *res = best->object;
        free(best);
        return res;
}
process *SRTN_tick(process *running_process)
{

            if (running_process == NULL || running_process->time_left == 0) {
                        running_process = poll_shortest();
                }

                    if (running_process != NULL) {
                        running_process->time_left--;
                }

                    return running_process;
}

int SRTN_startup()
{

            SRTN_queue = new_queue();
            if (SRTN_queue == NULL)
                        return 1;
            return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process)
{

            if (arriving_process != NULL) {
                        if (running_process != NULL &&
                                                 arriving_process->time_left < running_process->time_left) {
                                    queue_add(running_process, SRTN_queue);
                                    running_process = arriving_process;
                            } else {
                                    queue_add(arriving_process, SRTN_queue);
                            }
                }
            return running_process;
}

void SRTN_finish()
{
    	// TODO
                    free_queue(SRTN_queue);
}
