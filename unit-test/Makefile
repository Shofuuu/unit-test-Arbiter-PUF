check : puflog.c log.c uart.c parameters.c modeop.c
	@gcc -Wall -c log.c uart.c puflog.c parameters.c modeop.c
	@gcc -o puflog puflog.o log.o uart.o parameters.o modeop.o
	@rm -rf *.o puflog
	@echo "Check OK"
build : puflog.c log.c uart.c parameters.c modeop.c
	@gcc -Wall -c log.c uart.c puflog.c parameters.c modeop.c
	@gcc -o puflog puflog.o log.o uart.o parameters.o modeop.c
	@echo "Build OK"
clean :
	@rm -rf *.o puflog
test :
	@echo "Test OK"
