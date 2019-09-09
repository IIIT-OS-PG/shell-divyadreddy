cpp=g++
deafult: dish

dish: dshell.o input_command.o redirection.o execute_command.o pipe_command.o initialize_shell.o
	$(cpp)	-o dish dshell.o input_command.o redirection.o execute_command.o pipe_command.o initialize_shell.o

dshell.o: dshell.cpp 
	$(cpp)	-c dshell.cpp 

input_command.o: input_command.cpp 
	$(cpp)	-c input_command.cpp

execute_command.o: execute_command.cpp 
	$(cpp)	-c execute_command.cpp

redirection.o: redirection.cpp
	$(cpp)	-c redirection.cpp 

pipe_command.o: pipe_command.cpp
	$(cpp)	-c pipe_command.cpp 

initialize_shell.o: initialize_shell.cpp
	$(cpp)	-c initialize_shell.cpp 