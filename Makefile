Shell: main.o kycommandline.o processor.o
	gcc -o Shell main.c kycommandline.c processor.c

main.o: main.c kycommandline.h processor.h

kycommandline.o: kycommandline.c kycommandline.h

processor.o: processor.c processor.h kycommandline.h

clean:
	rm *~ *.o *#