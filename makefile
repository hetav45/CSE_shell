objects = shell.o interpreter.o prompt.o pwd.o cd.o echo.o ls.o pinfo.o

shell : $(objects)
	gcc -Wall -o shell $(objects) 

$(objects) : shell.h

.PHONY : clean
clean : 
	rm shell $(objects)	