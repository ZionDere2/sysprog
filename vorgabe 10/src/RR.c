#include "../lib/RR.h"

static queue_object *RR_queue;
//You can add more global variables
static unsigned int RR_quantum;
static unsigned int quantum_left;

process *RR_tick(process *running_process)
{

            if (running_process == NULL || running_process->time_left == 0 ||
                             quantum_left == 0) {
                        if (running_process != NULL && running_process->time_left > 0 &&
                                                 quantum_left == 0) {
                                    queue_add(running_process, RR_queue);
                            }
                        if (running_process != NULL && running_process->time_left == 0) {
                                    // finished process, do not re-add
                                    }
                        running_process = queue_poll(RR_queue);
                        quantum_left = RR_quantum;
                }

                    if (running_process != NULL) {
                        running_process->time_left--;
                        if (quantum_left > 0)
                                    quantum_left--;
                }

                    return running_process;
}

int RR_startup(int quantum)
{
            RR_queue = new_queue();
            if (RR_queue == NULL)
                        return 1;
            RR_quantum = quantum > 0 ? (unsigned int)quantum : 1;
            quantum_left = RR_quantum;
            return 0;
}

process *RR_new_arrival(process *arriving_process, process *running_process)
{

            if (arriving_process != NULL) {
                        queue_add(arriving_process, RR_queue);
                }
            return running_process;
}

void RR_finish()
{
    	// TODO
                    free_queue(RR_queue);
}
