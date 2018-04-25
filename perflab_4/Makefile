# Student's Makefile for the CS:APP Performance Lab
CC = g++
CFLAGS = -Wall -O3 -g -Wno-maybe-uninitialized
LIBS = -lm -lpthread

OBJS = helper.o driver.o kernels.o fcyc.o clock.o

all: driver

driver: $(OBJS) helper.h fcyc.h clock.h defs.h config.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

handin:
	cp kernels.c $(HANDINDIR)/$(TEAM)-$(VERSION)-kernels.c

clean: 
	-rm -f $(OBJS) driver core *~ *.o


