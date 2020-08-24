run:main.o FSC_FitsX.o
	g++ main.o FSC_FitsX.o -o run `pkg-config opencv4 --cflags --libs opencv4`
FSC_FitsX.o:FSC_FitsX.cpp
	g++ -c FSC_FitsX.cpp -o FSC_FitsX.o
main.o:main.cpp
	g++ -c main.cpp -o main.o `pkg-config opencv4 --cflags --libs opencv4`

clean:
	rm -f *.o