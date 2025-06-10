 #include "../lib/LCFSPR.h"

static queue_object* LCFSPR_queue;
-//You can add more global variables here

process* LCFSPR_tick (process* running_process){
    -    // TODO
            -    return NULL;
    +    if(running_process == NULL || running_process->time_left == 0){
        +        running_process = queue_poll_tail(LCFSPR_queue);
        +    }
    +    if(running_process != NULL){
        +        running_process->time_left--;
        +    }
    +    return running_process;
}

int LCFSPR_startup(){
    -    // TODO
            -    return 1;
    +    LCFSPR_queue = new_queue();
    +    if(LCFSPR_queue == NULL){
        +        return 1;
        +    }
    +    return 0;
}

process* LCFSPR_new_arrival(process* arriving_process, process* running_process){
    -    // TODO
            -    return NULL;
    +    if(arriving_process != NULL){
        +        if(running_process != NULL){
            +            queue_object *node = malloc(sizeof(queue_object));
            +            node->object = running_process;
            +            node->next = LCFSPR_queue->next;
            +            LCFSPR_queue->next = node;
            +        }
        +        queue_add(arriving_process, LCFSPR_queue);
        +        running_process = queue_poll_tail(LCFSPR_queue);
        +    }
    +    return running_process;
}

void LCFSPR_finish(){
    -	// TODO
            +        free_queue(LCFSPR_queue);
}
