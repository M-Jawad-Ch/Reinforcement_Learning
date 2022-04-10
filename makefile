build: main.cpp
	g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread -o main.exe

buildPE: pathEditor.cpp
	g++ pathEditor.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pathEditor.exe

path:
	./pathEditor.exe

run:
	./main.exe

clean:
	rm *.exe