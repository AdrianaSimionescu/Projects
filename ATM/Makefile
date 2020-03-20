CFLAGS = -g -Wall -Wextra -lm
OBJECTS = atm.o atm_functions.o
EXEC = atm

build:	atm

%.o:	%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run:	build
	./$(EXEC)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)
