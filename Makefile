objects = main.o debug.o encode.o sendData.o codeTools.o
includePath = include

edit : $(objects)
	cc -o leoj $(objects) 

main.o : main.c $(includePath)/headers.h
	cc -c main.c
debug.o : $(includePath)/debug.c $(includePath)/headers.h
	cc -c $(includePath)/debug.c
encode.o : $(includePath)/encode.c $(includePath)/headers.h
	cc -c $(includePath)/encode.c
sendData.o : $(includePath)/sendData.c $(includePath)/headers.h
	cc -c $(includePath)/sendData.c
codeTools.o : $(includePath)/codeTools.c $(includePath)/headers.h
	cc -c $(includePath)/codeTools.c

.PHONY : clean
clean :
		-rm $(objects)