CFLAGS = -Wall -Wextra

test: check
	./check

check: procspawn.o test.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f test procspawn.o test.o

.PHONY: test clean
