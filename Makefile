objects = myhttpd.o debug.o encode.o sendData.o

edit : $(objects)
	cc -o myhttpd $(objects)

myhttpd.o : myhttpd.c bin/headers.h
	cc -c myhttpd.c
debug.o : bin/debug.c bin/headers.h
	cc -c bin/debug.c

encode.o : bin/encode.c bin/headers.h
	cc -c bin/encode.c

sendData.o : bin/sendData.c bin/headers.h
	cc -c bin/sendData.c

.PHONY : clean
clean :
		-rm edit $(objects)