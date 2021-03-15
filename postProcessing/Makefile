FLAGS  = -lfl -lm  
CC     = gcc
OBJS   = lex.yy.c ${OBJECTIVOS}.tab.c
#LEX    = flex
#YACC   = bison


all:     refutationTime

clean:
	-rm refutationTime lex.yy.c *.tab.* *.output

refutationTime: refutationTime.l refutationTime.y 
	flex refutationTime.l
	bison  -v -d refutationTime.y -Wcounterexamples
	gcc lex.yy.c refutationTime.tab.c  -lfl -lm  -o refutationTime
