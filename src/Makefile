IDIR = ../include
CC = g++
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

_DEPS = main.hpp menu.hpp game.hpp normalGame.hpp timedGame.hpp multiGame.hpp snake.hpp fruit.hpp wall.hpp score.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o menu.o game.o normalGame.o timedGame.o multiGame.o snake.o fruit.o wall.o score.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~