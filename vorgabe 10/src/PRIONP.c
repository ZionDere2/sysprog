
#include "../lib/PRIONP.h"

static queue_object* PRIONP_queue;
//You can add more global variables here
static queue_object *PRIONP_queue;



    static process *poll_highest_priority()
    {
                    if (PRIONP_queue == NULL)
                            return NULL;

                    queue_object *prev = PRIONP_queue;
                    queue_object *curr = PRIONP_queue->next;
                    if (curr == NULL)
                            return NULL;

                    queue_object *best_prev = prev;
                    unsigned int best_prio = ((process *)curr->object)->priority;
                    while (curr != NULL) {
                                process *p = curr->object;
                                if (p->priority > best_prio) {
                                            best_prio = p->priority;
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

    process *PRIONP_tick(process *running_process)
    {
                    if (running_process == NULL || running_process->time_left == 0) {
                                running_process = poll_highest_priority();
                        }

                    if (running_process != NULL) {
                                running_process->time_left--;
                        }

                    return running_process;
    }

    int PRIONP_startup(){
            PRIONP_queue = new_queue();
            if (PRIONP_queue == NULL) {
                    return 1;
                }
            return 0;
    }

    process* PRIONP_new_arrival(process* arriving_process, process* running_process){

            if(arriving_process != NULL){
                    queue_add(arriving_process, PRIONP_queue);
                }
            return running_process;
    }

    void PRIONP_finish(){
            // TODO
                    free_queue(PRIONP_queue);
    }
