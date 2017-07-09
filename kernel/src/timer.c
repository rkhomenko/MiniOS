#include <common.h>
#include <timer.h>
#include <isr.h>
#include <monitor.h>
#include <module.h>


extern process_dispatcher_ptr process_dispatcher;
extern get_current_process_ptr get_current_process;

static void timer_callback(struct registers regs)
{
    struct process* proc = get_current_process();
    struct process* next_proc = NULL;

    process_dispatcher();

    next_proc = get_current_process();

    if (proc != NULL) {
        if (proc->state == PROCESS_STOPED) {
            monitor_write("Process terminated: id ");
            monitor_write_hex(proc->id + 1);
            monitor_put('\n');
        }
        if (proc->state == PROCESS_PAUSED) {
            monitor_write("Process paused: id ");
            monitor_write_hex(proc->id + 1);
            monitor_put('\n');
        }
    }

    if (next_proc != NULL) {
        monitor_write("Process continued: id ");
        monitor_write_hex(proc->id + 1);
        monitor_put('\n');
    }
}

void init_timer(uint32_t frequency)
{
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}