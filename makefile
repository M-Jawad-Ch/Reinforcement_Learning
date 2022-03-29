build: test.cpp
	g++ test.cpp -lsfml-graphics -lsfml-window -lsfml-system -o test.exe

buildPE: pathEditor.cpp
	g++ pathEditor.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pathEditor.exe

path:
	./pathEditor.exe

run:
	./test.exe

clean:
	rm *.exe