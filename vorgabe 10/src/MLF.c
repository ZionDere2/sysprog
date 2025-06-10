#include "../lib/MLF.h"

static queue_object **MLF_queues;

static unsigned int current_level;
static unsigned int quantum_left;

static unsigned int factorial(unsigned int n)
{
        unsigned int res = 1;
        for (unsigned int i = 2; i <= n; i++)
                res *= i;
        return res;
}

static unsigned int quantum_for_level(unsigned int lvl)
{
        if (lvl >= 3)
                return 0; // not used
        return factorial(lvl + 1);
}

static process *fetch_next_process()
{
        for (unsigned int i = 0; i < 4; i++) {
                if (MLF_queues[i]->next != NULL) {
                        current_level = i;
                        if (i == 3)
                                return queue_poll_tail(MLF_queues[i]);
                        else
                                return queue_poll(MLF_queues[i]);
                }
        }
        return NULL;
}

process *MLF_tick(process *running_process)
{

            if (running_process == NULL || running_process->time_left == 0 ||
                             (current_level < 3 && quantum_left == 0)) {
                        if (running_process != NULL && running_process->time_left > 0 &&
                                                 current_level < 3 && quantum_left == 0) {
                                    unsigned int next_lvl = current_level < 3 ? current_level + 1 : 3;
                                    queue_add(running_process, MLF_queues[next_lvl]);
                            }

                                running_process = fetch_next_process();
                        if (running_process != NULL) {
                                    if (current_level < 3)
                                                quantum_left = quantum_for_level(current_level);
                                    else
                                            quantum_left = running_process->time_left;
                            } else {
                                    quantum_left = 0;
                            }
                }

                    if (running_process != NULL) {
                        running_process->time_left--;
                        if (current_level < 3 && quantum_left > 0)
                                    quantum_left--;
                        else if (current_level == 3)
                                    quantum_left = running_process->time_left;
                }

                    return running_process;
}

int MLF_startup()
{

            MLF_queues = calloc(4, sizeof(queue_object *));
            if (MLF_queues == NULL)
                        return 1;
            for (unsigned int i = 0; i < 4; i++) {
                        MLF_queues[i] = new_queue();
                        if (MLF_queues[i] == NULL)
                                    return 1;
                }
            current_level = 0;
            quantum_left = 0;
            return 0;
}


process *MLF_new_arrival(process *arriving_process, process *running_process)
{

            if (arriving_process != NULL) {
                        queue_add(arriving_process, MLF_queues[0]);
                        if (running_process != NULL && current_level > 0 && current_level < 3) {
                                    queue_add(running_process, MLF_queues[current_level]);
                                    running_process = fetch_next_process();
                                    if (running_process != NULL) {
                                                quantum_left = quantum_for_level(current_level);
                                        }
                            }
                }
            return running_process;
}

void MLF_finish()
{
    	// TODO
                    if (MLF_queues == NULL)
                        return;
            for (unsigned int i = 0; i < 4; i++) {
                        free_queue(MLF_queues[i]);
                }
            free(MLF_queues);
}
