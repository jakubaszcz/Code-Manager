SRC =	Main.cpp	\
        src/application/Application.cpp \
        src/data/Data.cpp	\
        external/glad/src/glad.c

NAME = CodeManager
CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 -Iexternal/glad/include -Iexternal/nlohmann
LDFLAGS = -lglfw -lGL -lGLU -lglut

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all
