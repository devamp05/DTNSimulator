#
#
# Devam Punitbhai Patel
# dns682
# 11316715
# Narek Tamrazyan
# uwo916
# 11315451
CC = gcc
CCFLAGS = -g -Wextra -z noexecstack
CPPFLAGS = -std=gnu90 -Wall -pedantic -g -Wextra
OBJPATH = build/obj/
BINPATH = build/bin/
LIBPATH = build/lib/
ARCH = $(shell uname -m)
CC_ARM = arm-linux-gnueabihf-
CC_PPC = powerpc-linux-gnu-
TARGET = DTN-sim

all: build liblist.a ${TARGET} cp

build:
	mkdir -p ./build/obj
	mkdir -p ./build/lib
	mkdir -p ./build/bin

cp:
	cp ${BINPATH}* .

DTN-sim: DTN-sim.o	
	${CC} ${CCFLAGS} -o ${BINPATH}DTN-sim ${OBJPATH}DTN-sim.o \
	-llist -L ${LIBPATH}	
	#-L /student/cmpt332/rtt/lib/Linuxx86_64 -lRtt -lRttUtils \
	#-L /usr/incluse/tirpc -ltirpc -I /student/cmpt332/rtt/include

DTN-sim.o: DTN-sim.c
	${CC} ${CPPFLAGS} -o ${OBJPATH}DTN-sim.o -c DTN-sim.c -I ./
	#-I /student/cmpt332/rtt/include -I /usr/include/tirpc
	
clean:
	rm -fr ./build
	
