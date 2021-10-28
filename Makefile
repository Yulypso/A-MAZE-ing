CC = gcc
CC_FLAGS = -Wall
LIB_FLAGS = -lncurses
SRCDIR   = src
EXEC = RIMAZE

all: compile run

run: 
	@./$(EXEC)

compile:
	@$(CC) $(CC_FLAGS) $(SRCDIR)/*.c -o $(EXEC) $(LIB_FLAGS)

clean:
	@rm -rf $(BINDIR) $(EXEC)