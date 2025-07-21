SRC =	Main.cpp	\
        src/application/Application.cpp \
        src/data/Data.cpp	\

NAME = CodeManager
CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 `pkg-config --cflags Qt6Widgets`
LDFLAGS = `pkg-config --libs Qt6Widgets`

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all
