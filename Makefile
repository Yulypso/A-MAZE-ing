CC = gcc
CC_FLAGS = -Wall -lncurses
SRCDIR   = src
EXEC = RIMAZE

all: compile run clean

run: 
	@./$(EXEC)

compile:
	@$(CC) $(CC_FLAGS) $(SRCDIR)/*.c -o $(EXEC)

clean:
	@rm -rf $(BINDIR) $(EXEC)