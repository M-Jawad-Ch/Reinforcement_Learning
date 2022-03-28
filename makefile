main.exe: main.cpp
	g++ main.cpp -lpthread -lsfml-graphics -lsfml-window -lsfml-system -o main.exe

clean:
	rm *.exe

run: main.exe
	./main.exe