main:
	mkdir -p build/html
	cp src/html/* build/html/
	g++ -o build/server src/main.cpp src/server/Server.cpp src/parser/HttpParser.cpp 
