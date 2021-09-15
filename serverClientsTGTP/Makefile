# Server & Client TGTP
# requires BOOST and MySQL libraries
CC     = g++

.PHONY: clean all

all: serverTGTP client

clean:
	rm -rf serverTGTP client

serverTGTP: serverTGTP.cpp connectionData.hpp
	${CC} -o $@ serverTGTP.cpp -lmysqlcppconn

client: client.cpp
	${CC} -o $@ client.cpp	
