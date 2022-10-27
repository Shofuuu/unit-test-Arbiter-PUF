check : puflog.c log.c uart.c parameters.c
	@gcc -Wall -c log.c uart.c puflog.c parameters.c
	@gcc -o puflog puflog.o log.o uart.o parameters.o
	@rm -rf *.o puflog
	@echo "Check OK"
build : puflog.c log.c uart.c parameters.c
	@gcc -Wall -c log.c uart.c puflog.c parameters.c
	@gcc -o puflog puflog.o log.o uart.o parameters.o
	@echo "Build OK"
clean :
	@rm -rf *.o puflog
test :
	@echo "Test OK"
