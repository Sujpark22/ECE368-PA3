CC = gcc
TARGET = pa3

CFLAGS = -std=c99 -Wall -Wshadow -Wvla -g -pedantic -fstack-protector-strong --param ssp-buffer-size=1
OFLAGS = -std=c99 -Wall -Wshadow -Wvla -O3 -pedantic -fstack-protector-strong --param ssp-buffer-size=1

SRCS = main.c tree.c reroot.c
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
	./pa3 examples/3.po out_3lr.pr out_3rl.pr out_3.rdim out_3.opt
	diff out_3lr.pr examples/3lr.pr
	diff out_3rl.pr examples/3rl.pr
	diff out_3.rdim examples/3.rdim
	diff out_3.opt examples/3.opt

	./pa3 examples/8.po out_8lr.pr out_8rl.pr out_8.rdim out_8.opt
	diff out_8lr.pr examples/8lr.pr
	diff out_8rl.pr examples/8rl.pr
	diff out_8.rdim examples/8.rdim
	diff out_8.opt examples/8.opt

########################################
# Larger example runs (no diff)
########################################

large: $(TARGET)
	./pa3 examples/100.po out_100lr.pr out_100rl.pr out_100.rdim out_100.opt
	./pa3 examples/500.po out_500lr.pr out_500rl.pr out_500.rdim out_500.opt
	./pa3 examples/1K.po out_1Klr.pr out_1Krl.pr out_1K.rdim out_1K.opt

########################################
# Memory check (Valgrind)
########################################

memory: $(TARGET)
	valgrind -s --leak-check=full --error-exitcode=135 \
		./pa3 examples/3.po out_3lr.pr out_3rl.pr out_3.rdim out_3.opt
	valgrind -s --leak-check=full --error-exitcode=135 \
		./pa3 examples/8.po out_8lr.pr out_8rl.pr out_8.rdim out_8.opt

########################################

clean:
	rm -f $(OBJS) $(TARGET) \
		out_3lr.pr out_3rl.pr out_3.rdim out_3.opt \
		out_8lr.pr out_8rl.pr out_8.rdim out_8.opt \
		out_100lr.pr out_100rl.pr out_100.rdim out_100.opt \
		out_500lr.pr out_500rl.pr out_500.rdim out_500.opt \
		out_1Klr.pr out_1Krl.pr out_1K.rdim out_1K.opt

.PHONY: all testall large memory clean