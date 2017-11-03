SOURCES = ObjectAllocator.cpp driver-sample.cpp ALGraph.cpp
FLAGS = -std=c++14 -Wall

compile:
	g++ -o out $(SOURCES) $(FLAGS)

clean:
	rm *-app *.o *.obj out

1:
	g++ -o out $(SOURCES) $(FLAGS)
	./out 7 

8:
	g++ -o out $(SOURCES) $(FLAGS)
	./out 8 | more

run:
	g++ -o out $(SOURCES) $(FLAGS)
	./out > output.txt

echo:
	g++ -o out $(SOURCES) $(FLAGS)
	./out 

debug:
	g++ -o out -g $(SOURCES) $(FLAGS)
	valgrind -q --leak-check=full --tool=memcheck ./out > output.txt 2>&1 
