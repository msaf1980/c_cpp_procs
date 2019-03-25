CFLAGS = -Wall -Wextra

test: check
	./check

check: procutil.o test.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f check procutil.o test.o

.PHONY: test clean
