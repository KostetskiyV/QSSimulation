CC := gcc
CFLAGS := -std=c99 -Werror -Wall -Wextra -Wpedantic -g -Wfloat-equal -Wfloat-conversion

app.exe: main.o queueing_system.o array_queue.o list_queue.o list.o request.o printers.o creative_mode.o
	$(CC) -o $@ $^ -lm

./%.o : ./%.c ./*.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.exe