CC = gcc
CC_FLAGS = -Wall -lncurses
SRCDIR   = src
EXEC = EXEC

all: compile run clean

run: compile
	@./$(EXEC)

compile:
	@$(CC) $(CC_FLAGS) $(SRCDIR)/*.c -o $(EXEC)

clean:
	@rm -rf $(BINDIR) $(EXEC)