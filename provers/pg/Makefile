# 'pg' makefile
#
# This file is part of 'pg', the OGP Provers-Generator (PG) for the Geometry
# Deductive Database Method (GDDM).
#
# Copyright (C) 2025 Nuno Baeta, Pedro Quaresma
# Distributed under GNU GPL 3.0 or later.


CC = g++
CFLAGS =
LEX = flex
LFLAGS =
YACC = bison
YFLAGS = -d
LDLIBS =
RM = rm
RMFLAGS = -f

PROG = pg
OBJS = pg.o axioms.o generate.o warnerr.o scanner.o parser.o

$(PROG):	$(OBJS)
		$(CC) -o $@ $^

pg.o:		pg.hpp axioms.hpp warnerr.hpp parser.h
axioms.o:	axioms.hpp
generate.o:	generate.hpp
warnerr.o:	warnerr.hpp
parser.h:	parser.c
scanner.o:	parser.h


.PHONY: clean distclean

clean:
	$(RM) $(RMFLAGS) $(OBJS) parser.c parser.h scanner.c
distclean:
	$(RM) $(RMFLAGS) $(PROG) $(OBJS) parser.c parser.h scanner.c

%.c:	%.l
	$(LEX) $(LFLAGS) -o $@ $<
%.c:	%.y
	$(YACC) $(YFLAGS) -o $@ $<
