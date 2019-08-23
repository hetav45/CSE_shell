objects = shell.o interpreter.o prompt.o pwd.o cd.o echo.o ls.o pinfo.o history.o

shell : $(objects)
	gcc -o shell $(objects) 

$(objects) : shell.h

.PHONY : clean
clean : 
	rm shell $(objects)	