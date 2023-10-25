NAME			=	deferred_shading

# **************************** COMPILER SETTINGS ***************************** #

CC				=	gcc
# CFLAGS			=	-Wall -Wextra -Werror -MMD -MP
CPPFLAGS		=	-I./include
LDFLAGS			=	-framework OpenGL -framework Appkit -framework IOKit
LDLIBS			=	

# ********************************* LIBRAY *********************************** #

# glfw
GLFW_PATH		=	./glfw-3.3.8
CPPFLAGS		+=	-I./${GLFW_PATH}/include
LDFLAGS			+=	-L./${GLFW_PATH}/build/src
LDLIBS			+=	-lglfw3

# ****************************** SOURCE FILES ******************************** #

SRCS			=	\
					$(wildcard src/*.c) \
					$(wildcard src/*/*.c)
OBJS			=	$(SRCS:.c=.o)
DEPS			=	$(SRCS:.c=.d)

# ********************************** RULES ********************************** #

all:
	make $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)

fclean:
	rm -f $(NAME) $(OBJS) $(DEPS)

re: 
	make fclean
	make all

-include $(DEPS)
.PHONY: all clean fclean re