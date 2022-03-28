build:
	g++ pathEditor.cpp -lsfml-graphics -lsfml-window -lsfml-system -o pathEditor.exe

path:
	./pathEditor.exe

clean:
	rm *.exe