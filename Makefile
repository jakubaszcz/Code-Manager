SRC =	Main.cpp	\
        src/application/Application.cpp \
        src/data/Data.cpp	\
        src/software/Software.cpp	\
        src/audio/Audio.cpp	\
        src/graphic/WorkflowGraphic.cpp	\
        src/graphic/workflow/WorkflowHeaderGraphic.cpp	\
        src/graphic/workflow/WorkflowBodyGraphic.cpp	\
        src/graphic/workflow/tabs/WorkflowBodyWorkflowTabGraphic.cpp	\
        src/graphic/workflow/tabs/WorkflowBodyCommandTabGraphic.cpp	\
        src/graphic/workflow/tabs/WorkflowBodyApplicationTabGraphic.cpp	\
        src/graphic/MenuGraphic.cpp	\
        src/graphic/SettingsGraphic.cpp	\
        src/graphic/popup/IPopup.cpp	\
        src/graphic/popup/NamePopup.cpp	\
        src/graphic/popup/TerminalPopup.cpp
NAME = CodeManager
CC = g++
CFLAGS = -Wall -Wextra -pedantic -O2 `pkg-config --cflags Qt6Widgets` -fPIC
LDFLAGS = `pkg-config --libs Qt6Widgets`

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all