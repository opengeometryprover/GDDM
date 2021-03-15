OBJECTIVOS = filterJGEXtoFOF
FLAGS  = -lfl -lm  
CC     = gcc
OBJS   = lex.yy.c ${OBJECTIVOS}.tab.c
#LEX    = flex
#YACC   = bison


#all:    ${OBJECTIVOS}
all:    filterJGEXtoFOF

clean:
	-rm ${OBJECTIVOS} lex.yy.c *.tab.* *.output

filterJGEXtoFOF: filterJGEXtoFOF.l filterJGEXtoFOF.y 
	flex filterJGEXtoFOF.l
	bison  -v -d filterJGEXtoFOF.y -Wcounterexamples
	gcc lex.yy.c filterJGEXtoFOF.tab.c  -lfl -lm  -o filterJGEXtoFOF
