objs = server client

makeAll : $(objs)

server : server.c server.h
	gcc server.c -o server
	
client : client.c client.h
	gcc client.c -o client
	

.PHONY : clean
clean : 
	-rm $(objs)