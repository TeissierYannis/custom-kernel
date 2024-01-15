#include "../headers/system.h"

// Global variables for process and thread management
process_t *current_process = NULL;
thread_t *current_thread = NULL;
process_t *process_list[MAX_THREADS]; // Array to store all processes
unsigned int process_count = 0; // Counter for process IDs

void create_process(void (*start_function)()) {
    // Allocate and initialize a new process
    process_t *process = malloc(sizeof(process_t));
    process->id = generate_new_process_id(); // You need to implement this function
    process->state = READY;
    process->context.eip = (unsigned int)start_function;

    // Add the process to the process list
    add_process_to_list(process); // You need to implement this function

    // If this is the first process, make it the current process
    if (current_process == NULL) {
        current_process = process;
    }
}

void terminate_process(unsigned int id) {
    // Find the process with the given ID
    process_t *process = find_process_by_id(id); // You need to implement this function
    if (process == NULL) {
        return; // Process not found
    }

    // Remove the process from the process list
    remove_process_from_list(process); // You need to implement this function

    // Free the process structure
    free(process);
}

void create_thread(process_t *process, void (*start_function)()) {
    // Allocate and initialize a new thread
    thread_t *thread = malloc(sizeof(thread_t));
    thread->id = generate_new_thread_id(); // You need to implement this function
    thread->state = READY;
    thread->context.eip = (unsigned int)start_function;

    // Add the thread to the process's thread list
    add_thread_to_process(process, thread); // You need to implement this function

    // If this is the first thread, make it the current thread
    if (current_thread == NULL) {
        current_thread = thread;
    }
}

void terminate_thread(process_t *process, unsigned int id) {
    // Find the thread with the given ID
    thread_t *thread = find_thread_by_id(process, id); // You need to implement this function
    if (thread == NULL) {
        return; // Thread not found
    }

    // Remove the thread from the process's thread list
    remove_thread_from_process(process, thread); // You need to implement this function

    // Free the thread structure
    free(thread);
}

void switch_context(context_t *old_context, context_t *new_context) {
    // Save the old context
    old_context->esp = get_esp(); // You need to implement this function
    old_context->ebp = get_ebp(); // You need to implement this function
    old_context->eip = get_eip(); // You need to implement this function

    // Restore the new context
    set_esp(new_context->esp); // You need to implement this function
    set_ebp(new_context->ebp); // You need to implement this function
    set_eip(new_context->eip); // You need to implement this function
}

void schedule() {
    // Find the next ready thread
    thread_t *next_thread = find_next_ready_thread(); // You need to implement this function
    if (next_thread == NULL) {
        return; // No ready thread found
    }

    // Switch context to the next thread
    switch_context(&current_thread->context, &next_thread->context);

    // Update the current thread
    current_thread = next_thread;
}

unsigned int generate_new_process_id() {
    return process_count++;
}

void add_process_to_list(process_t *process) {
    process_list[process->id] = process;
}

process_t *find_process_by_id(unsigned int id) {
    return process_list[id];
}

void remove_process_from_list(process_t *process) {
    process_list[process->id] = NULL;
}

unsigned int generate_new_thread_id() {
    if (current_process == NULL) {
        return -1; // No current process
    }
    return current_process->id * MAX_THREADS + current_process->thread_count++;
}

void add_thread_to_process(process_t *process, thread_t *thread) {
    process->threads[thread->id % MAX_THREADS] = thread;
}

thread_t *find_thread_by_id(process_t *process, unsigned int id) {
    return process->threads[id % MAX_THREADS];
}

void remove_thread_from_process(process_t *process, thread_t *thread) {
    process->threads[thread->id % MAX_THREADS] = NULL;
}

unsigned int get_esp() {
    unsigned int esp;
    __asm__ volatile ("mov %%esp, %0" : "=r"(esp));
    return esp;
}

unsigned int get_ebp() {
    unsigned int ebp;
    __asm__ volatile ("mov %%ebp, %0" : "=r"(ebp));
    return ebp;
}

unsigned int get_eip() {
    unsigned int eip;
    __asm__ volatile (
        "call get_eip_label\n"
        "get_eip_label: pop %0"
        : "=r"(eip)
    );
    return eip;
}

void set_esp(unsigned int esp) {
    __asm__ volatile ("mov %0, %%esp" : : "r"(esp));
}

void set_ebp(unsigned int ebp) {
    __asm__ volatile (
        "mov %0, %%ebp"
        :
        : "r"(ebp)
    );
}

void set_eip(unsigned int eip) {
    __asm__ volatile (
        "mov %0, %%ebp"
        :
        : "r"(eip)
    );
}

thread_t *find_next_ready_thread() {
    if (current_process == NULL) {
        return NULL; // No current process
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        if (current_process->threads[i] != NULL && current_process->threads[i]->state == READY) {
            return current_process->threads[i];
        }
    }
    return NULL; // No ready thread found
}