# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib 

# All Targets
all: splflix

# Tool invocations
#Executable "s" depends on the files Main.o and Action.o and User.o and Watchable.o and Session.o
splflix: bin/Main.o bin/Action.o bin/User.o bin/Watchable.o bin/Session.o
	@echo 'Building target: splflix'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/splflix bin/Main.o bin/Action.o bin/User.o bin/Watchable.o bin/Session.o $(LFLAGS)
	@echo 'Finished building target: splflix'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp
# Depends on the source and header files
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
# Depends on the source and header files
bin/Watchable.o: src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
# Depends on the source and header files
bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp
# Depends on the source and header files
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp

#Clean the build directory
clean: 
	rm -f bin/*
