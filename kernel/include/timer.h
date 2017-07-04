#ifndef MINI_OS_TIMER_H_
#define MINI_OS_TIMER_H_

#include <stddef.h>

typedef void (*proc_switch_ptr)(void);

void init_timer(uint32_t frequency);
void init_switch_proc(proc_switch_ptr switch_func); 

#endif /* MINI_OS_TIMER_H_ */
