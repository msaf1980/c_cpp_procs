CFLAGS = -Wall -Wextra

test: check
	./check

check: procutils.o test.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f test procutils.o test.o

.PHONY: test clean
