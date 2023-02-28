# FLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC = gcc # -m32 -no-pie -fno-pie

# LDLIBS = -lm


integral: integral.o func.o
	gcc $(CFLAGS) -o integral func.o integral.o -m32

.PHONY: clean all test
all: integral


integral.o: integral.c
	gcc -m32 -c -o integral.o integral.c

func.o: func.asm
	nasm -f elf32 -o func.o func.asm

clean:
	rm -rf *.o integral
test:
	./integral -R 1:2:0.4:0.5:0.000001:0.448178
	./integral -I 1:-1.821137:0.448178:0.000001:8.322253

