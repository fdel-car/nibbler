TARGET := nibbler
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
DYLIBS_DIR := ./dylibs
CC := clang++
OS := $(shell uname)
CFLAGS := -Wall -Wextra -Werror -std=c++11
DYLIBS_FLAGS := -shared -fPIC
ignore-warnings : CFLAGS := -w -std=c++11
# LIBS :=
HEADERS := -I./includes/ -I./libs/includes/ `pkg-config --cflags glm`

# Colors
RESET := \033[0m
CYAN := \033[36;01m
GREEN := \033[32;01m
ERROR := \033[31;01m
WARN := \033[33;01m
# Formatting
CLEAR_LINE := \033[2K
MOVE_CURSOR_UP := \033[1A

SRCS := $(shell find $(SRCS_DIR) -maxdepth 1 -name "*.cpp")
OBJS := $(patsubst $(SRCS_DIR)%.cpp,$(OBJS_DIR)%.o,$(SRCS))

GLFW_SRCS := $(shell find $(SRCS_DIR)/GLFW -maxdepth 1 -name "*.cpp")
GLFW_OBJS := $(patsubst $(SRCS_DIR)/GLFW/%.cpp,$(OBJS_DIR)/GLFW/%.o,$(GLFW_SRCS))

SFML_SRCS := $(shell find $(SRCS_DIR)/SFML -maxdepth 1 -name "*.cpp")
SFML_OBJS := $(patsubst $(SRCS_DIR)/SFML/%.cpp,$(OBJS_DIR)/SFML/%.o,$(SFML_SRCS))

SDL_SRCS := $(shell find $(SRCS_DIR)/SDL -maxdepth 1 -name "*.cpp")
SDL_OBJS := $(patsubst $(SRCS_DIR)/SDL/%.cpp,$(OBJS_DIR)/SDL/%.o,$(SDL_SRCS))

all: $(OBJS_DIR) $(DYLIBS_DIR) $(TARGET)

ignore-warnings: all

$(DYLIBS_DIR): $(GLFW_OBJS) $(SFML_OBJS) $(SDL_OBJS)
	@mkdir -p $(DYLIBS_DIR)
	@# GLFW dylib
	@$(CC) $(CFLAGS) -c ./libs/srcs/glad/glad.cpp -o $(OBJS_DIR)/glad/glad.o $(HEADERS)
	@$(CC) $(DYLIBS_FLAGS) $(CFLAGS) $(GLFW_OBJS) $(OBJS_DIR)/glad/glad.o -o $(DYLIBS_DIR)/GLFWDisplay.so `pkg-config --libs glfw3`
	@# SFML dylib
	@$(CC) $(DYLIBS_FLAGS) $(CFLAGS) $(SFML_OBJS) -o $(DYLIBS_DIR)/SFMLDisplay.so `pkg-config --libs sfml-window sfml-graphics`
	@# SDL dylib
	@$(CC) $(DYLIBS_FLAGS) $(CFLAGS) $(SDL_OBJS) -o $(DYLIBS_DIR)/SDLDisplay.so -framework SDL2
	@echo "$(GREEN)Successfully compiled the dynamic libraries.$(RESET)"

$(OBJS_DIR)/GLFW/%.o: $(SRCS_DIR)/GLFW/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS) `pkg-config --cflags glfw3`

$(OBJS_DIR)/SFML/%.o: $(SRCS_DIR)/SFML/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS) `pkg-config --cflags sfml-window sfml-graphics`

$(OBJS_DIR)/SDL/%.o: $(SRCS_DIR)/SDL/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS) -F/Library/Frameworks

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/glad
	@mkdir -p $(OBJS_DIR)/GLFW
	@mkdir -p $(OBJS_DIR)/SFML
	@mkdir -p $(OBJS_DIR)/SDL

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^ # $(LIBS)
	@echo "$(CYAN)Successfully compiled $(TARGET).$(RESET)"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $^ -o $@ $(HEADERS)

install:
	./install.sh

uninstall:
	./uninstall.sh

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -rf $(DYLIBS_DIR)
	@rm -f $(TARGET)

re: fclean
	@make all

.PHONY: all clean fclean re ignore-warnings install uninstall
