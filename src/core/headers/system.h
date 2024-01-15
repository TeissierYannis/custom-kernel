#ifndef __SYSTEM_H
#define __SYSTEM_H
#define NULL ((void*)0)

#define BLOCK_SIZE 4096
#define TOTAL_MEMORY 0x1000000
#define TOTAL_BLOCKS (TOTAL_MEMORY / BLOCK_SIZE)

#define VGA_BUFFER_ADDR 0xB8000
#define VGA_BUFFER_FRAME (VGA_BUFFER_ADDR / 0x1000) // Frame number for VGA buffer

#define PAGE_SIZE    4096  // Define the size of a page (typically 4 KB)
#define TOTAL_PAGES  (TOTAL_MEMORY / PAGE_SIZE)

#define SECTOR_SIZE 512
#define MAX_FILES 100
#define FILENAME_MAX 255

#define MAX_THREADS 100

typedef unsigned long size_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;


/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};


/* main.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* screen.c */
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

/* gdt.c */
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* idt.c */
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

/* isrs.c */
extern void isrs_install();
extern void fault_handler(struct regs *r);

/* irq.c */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);
extern void irq_remap(void);
extern void irq_install();
extern void irq_handler(struct regs *r);

/* timer.c */
extern void timer_wait(int ticks);
extern void timer_phase(int hz);
extern void timer_handler(struct regs *r);
extern void timer_install();

/* keyboard.c */
extern void keyboard_handler(struct regs *r);
extern void keyboard_install();

/* phys_memory.c */
extern void set_bit(unsigned int bit);
extern void clear_bit(unsigned int bit);
extern unsigned int test_bit(unsigned int bit);
extern unsigned int first_free();
extern void alloc_block(unsigned int block);
extern void free_block(unsigned int block);

/* paging.c */
extern unsigned int block_to_addr(unsigned int block);
extern unsigned int virtual_to_physical(unsigned int virtual_addr);
extern void setup_paging();

/* page_alloc.c */
extern void set_page_used(int page);
extern void set_page_free(int page);
extern int is_page_free(int page);
extern int find_free_page();
extern void* allocate_page();
extern void free_page(void* page_addr);

/* heap.c */
extern void* malloc(size_t size);
extern void free(void* ptr);
extern void init_heap();

/* disk.c */
extern void disk_init();
extern void disk_read_sector(unsigned int sector, unsigned char* buffer);
extern void disk_write_sector(unsigned int sector, const unsigned char* buffer);

/* fs.c */
extern void fs_initialize();
extern int fs_create(const char* filename);
extern int fs_open(const char* filename);
extern int fs_read(int file, char* buf, int nbytes);
extern int fs_write(int file, const char* buf, int nbytes);
extern void fs_close(int file);

/* util.c */
extern int strcmp(const char *s1, const char *s2);
extern char *strncpy(char *dest, const char *src, size_t n);

/* processes.c */
// Context structure
typedef struct {
    unsigned int esp, ebp, eip, eflags, eax, ebx, ecx, edx;
} context_t;

// Process and thread states
typedef enum {
    RUNNING,
    READY,
    BLOCKED
} state_t;

// Thread structure
typedef struct {
    unsigned int id;
    state_t state;
    context_t context;
} thread_t;


// Process structure
typedef struct {
    unsigned int id;
    state_t state;
    context_t context;
    thread_t *threads[MAX_THREADS];
    int thread_count;
} process_t;

extern void create_process(void (*start_function)());
extern void terminate_process(unsigned int id);
extern void create_thread(process_t *process, void (*start_function)());
extern void terminate_thread(process_t *process, unsigned int id);
extern void switch_context(context_t *old_context, context_t *new_context);
extern void schedule();
extern thread_t *find_next_ready_thread(void);
extern void set_eip(unsigned int eip);
extern void set_ebp(unsigned int ebp);
extern void set_esp(unsigned int esp);
extern unsigned int get_eip();
extern unsigned int get_ebp();
extern unsigned int get_esp();
extern thread_t *find_thread_by_id(process_t *process, unsigned int id);
extern void add_thread_to_process(process_t *process, thread_t *thread);
extern unsigned int generate_new_thread_id();
extern void remove_process_from_list(process_t *process);
extern process_t *find_process_by_id(unsigned int id);
extern void add_process_to_list(process_t *process);
extern unsigned int generate_new_process_id();
extern void remove_thread_from_process(process_t *process, thread_t *thread);

#endif