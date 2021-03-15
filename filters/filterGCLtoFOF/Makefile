OBJECTIVOS = filterGCLtoFOF
FLAGS  = -lfl -lm  
CC     = gcc
OBJS   = lex.yy.c ${OBJECTIVOS}.tab.c
#LEX    = flex
#YACC   = bison


all:    ${OBJECTIVOS}

clean:
	-rm ${OBJECTIVOS} lex.yy.c *.tab.* filterGCLtoFOF.output

#{OBJECTIVOS}:
#	flex $@.l
#	bison -d  $@.y
#	${CC} ${OBJS} ${FLAGS} -o $@

filterGCLtoFOF: filterGCLtoFOF.l filterGCLtoFOF.y 
	flex filterGCLtoFOF.l
	bison  -v -d filterGCLtoFOF.y
	gcc lex.yy.c filterGCLtoFOF.tab.c  -lfl -lm  -o filterGCLtoFOF

