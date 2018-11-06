# Makefile for check_wall_ink_data

TARGET=check_wall_ink_data
CPPSRC=$(TARGET).cpp
SRC=$(TARGET).c
CPPC= g++
CC= gcc
STD= c99
CPPSTD=c++98
CFLAGS= -Wall -g --std=$(STD)
CPPFLAGS= -Wall -g --std=$(CPPSTD)

#have it include all .h files in current directory
CPPFLAGS+= -I. 
CFLAGS+= -I.

LOCAL_CPPSRC= 
LOCAL_SRC= sha1.c 

CPPSRC+= $(LOCAL_CPPSRC)
SRC+= $(LOCAL_SRC)

OUTPUT=$(TARGET).bin

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

cpp:
	$(CPPC) $(CPPFLAGS) $(CPPSRC) -o $(OUTPUT)

clean:
	rm *.bin *.lst *.dmp *.hex *.elf $(OUTPUT)



