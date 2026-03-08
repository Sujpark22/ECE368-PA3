CC = gcc
TARGET = pa2

CFLAGS = -std=c99 -Wall -Wshadow -Wvla -g -pedantic -fstack-protector-strong --param ssp-buffer-size=1
OFLAGS = -std=c99 -Wall -Wshadow -Wvla -O3 -pedantic -fstack-protector-strong --param ssp-buffer-size=1

SRCS = main.c tree.c pack.c files.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

########################################
# Run tests for provided examples
########################################

testall: $(TARGET)
	./pa2 examples/3.pr output_3.po output_3.dim output_3.pck
	diff output_3.po examples/3.po
	diff output_3.dim examples/3.dim
	diff output_3.pck examples/3.pck

	./pa2 examples/8.pr output_8.po output_8.dim output_8.pck
	diff output_8.po examples/8.po
	diff output_8.dim examples/8.dim
	diff output_8.pck examples/8.pck

########################################
# Memory check (Valgrind)
########################################

memory: $(TARGET)
	valgrind -s --leak-check=full --error-exitcode=135 \
	./pa2 3.pr output_3.po output_3.dim output_3.pck

	valgrind -s --leak-check=full --error-exitcode=135 \
	./pa2 8.pr output_8.po output_8.dim output_8.pck

########################################

clean:
	rm -f $(OBJS) $(TARGET) \
	output_3.po output_3.dim output_3.pck \
	output_8.po output_8.dim output_8.pck

.PHONY: all testall memory clean