dbserver: dbserver.o
	g++ -o dbserver server.o database.o

dbserver.o: server.cc server.hh database.cc database.hh
	g++ -c server.cc database.cc

clean:
	rm -rf *.o dbserver