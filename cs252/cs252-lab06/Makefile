.PHONY: monitor
monitor: sys_monitor.c procinfo.c sysinfo.c proc_actions.c graphinfo.c devices.c
	gcc `pkg-config --cflags gtk+-3.0` -g -o sys_monitor sys_monitor.c devices.c proc_actions.c graphinfo.c -lm procinfo.c sysinfo.c `pkg-config --libs gtk+-3.0`

.PHONY: taskmanager
taskmanager: sysinfo.c procinfo.c devices.c main.c proc_actions.c graphinfo.c
	gcc -g -o taskmanager sysinfo.c procinfo.c devices.c proc_actions.c graphinfo.c -lm main.c
