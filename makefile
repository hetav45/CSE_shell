CFLAGS = -Wall -Wextra
#CFLAGS += -g

objects = shell.o interpreter.o prompt.o pwd.o cd.o echo.o ls.o pinfo.o history.o setenv.o unsetenv.o jobs.o kjob.o fg.o bg.o overkill.o cronjob.o

shell : $(objects)
	gcc ${CFlAGS} -o shell $(objects) 

$(objects) : shell.h

.PHONY : clean
clean : 
	rm shell $(objects)	