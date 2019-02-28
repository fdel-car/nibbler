TARGET := nibbler
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
DYLIBS_DIR := ./dylibs
CC := clang++
OS := $(shell uname)
CFLAGS := -Wall -Wextra -Werror -std=c++11
DYLIBS_FLAGS := -shared -fPIC
ignore-warnings : CFLAGS := -w
# LIBS :=
HEADERS := -I./includes/ -I./libs/includes/

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

GLFW_DIR := ./srcs/GLFW
SFML_DIR := ./srcs/SFML
SDL_DIR := ./srcs/SDL

all: $(OBJS_DIR) $(DYLIBS_DIR) $(TARGET)

ignore-warnings: all

$(DYLIBS_DIR):
	@mkdir -p $(DYLIBS_DIR)
	@# GLFW dylib
	@$(CC) $(CFLAGS) -c ./libs/srcs/glad/glad.cpp -o $(OBJS_DIR)/glad/glad.o $(HEADERS)
	@$(CC) $(CFLAGS) -c $(GLFW_DIR)/GLFWDisplay.cpp -o $(OBJS_DIR)/GLFW/GLFWDisplay.o $(HEADERS) `pkg-config --cflags glfw3`
	@$(CC) $(DYLIBS_FLAGS) $(CFLAGS) $(OBJS_DIR)/GLFW/GLFWDisplay.o $(OBJS_DIR)/glad/glad.o -o $(DYLIBS_DIR)/GLFWDisplay.so `pkg-config --libs glfw3`
	@# SFML dylib
	@# SDL dylib
	@echo "$(GREEN)Successfully compiled the dynamic libraries.$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/glad
	@mkdir -p $(OBJS_DIR)/GLFW

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
