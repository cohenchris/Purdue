/* callback.h */

syscall cbregister(void (* fp)(void), umsg32 *);
syscall cbunregister(void);

long timeslice_ipc(void);
