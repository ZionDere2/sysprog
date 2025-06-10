#include "../lib/HRRN.h"

static queue_object *HRRN_queue;
//You can add more global variables and structs here

static HRRN_process *create_hrrn_proc(process *p)
{
        HRRN_process *hp = malloc(sizeof(HRRN_process));
        if (hp == NULL)
                return NULL;
        hp->this_process = p;
        hp->waiting_time = 0;
        hp->rr = 0.0f;
        return hp;
}

static process *poll_highest_rr()
{
        if (HRRN_queue == NULL || HRRN_queue->next == NULL)
                return NULL;

        queue_object *prev = HRRN_queue;
        queue_object *curr = HRRN_queue->next;
        queue_object *best_prev = prev;
        float best_rr = -1.0f;

        while (curr != NULL) {
                HRRN_process *hp = curr->object;
                float rr = (float)(hp->waiting_time + hp->this_process->time_left) /
                                                        hp->this_process->time_left;
                if (rr > best_rr) {
                        best_rr = rr;
                        best_prev = prev;
                }
                prev = curr;
                curr = curr->next;
        }

        queue_object *best = best_prev->next;
        best_prev->next = best->next;
        HRRN_process *hp = best->object;
        process *p = hp->this_process;
        free(hp);
        free(best);
        return p;
}

process *HRRN_tick(process *running_process)
{

            // increase waiting time for all queued processes
                    queue_object *iter = HRRN_queue->next;
            while (iter != NULL) {
                        HRRN_process *hp = iter->object;
                        hp->waiting_time++;
                        iter = iter->next;
                }

                    if (running_process == NULL || running_process->time_left == 0) {
                        running_process = poll_highest_rr();
                }

                    if (running_process != NULL) {
                        running_process->time_left--;
                }

                    return running_process;
}

int HRRN_startup()
{

            HRRN_queue = new_queue();
            if (HRRN_queue == NULL)
                        return 1;
            return 0;
}

process *HRRN_new_arrival(process *arriving_process, process *running_process)
{

            if (arriving_process != NULL) {
                        HRRN_process *hp = create_hrrn_proc(arriving_process);
                        if (hp != NULL)
                                    queue_add(hp, HRRN_queue);
                }
            return running_process;
}

void HRRN_finish()
{
    	// TODO
                    if (HRRN_queue == NULL)
                        return;

                    queue_object *iter = HRRN_queue->next;
            while (iter != NULL) {
                        queue_object *next = iter->next;
                        HRRN_process *hp = iter->object;
                        free(hp);
                        free(iter);
                        iter = next;
                }
            free(HRRN_queue);
}
