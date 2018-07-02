file:config.o	linuxpasswd.o	dir.o	exec.o	file.o	main.o
	@gcc	-o	file	main.o	linuxpasswd.o	config.o	dir.o	exec.o	file.o
linuxpasswd.o:linuxpasswd.c config.h
	@gcc -c	linuxpasswd.c	config.h
config.o:config.c	config.h
	@gcc	-c	config.c	config.h
dir.o:dir.c	config.h
	@gcc -c	dir.c	config.h
exec.o:exec.c	config.h
	@gcc -c	exec.c	config.h
file.o:file.c	config.h
	@gcc -c	file.c	config.h
main.o:main.c config.h 
	@gcc -c	main.c	config.h
clean:
	@rm -f file	*.o config.h.gch
