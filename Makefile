INC_DIR		= 	srcs
SRC_DIR		= 	srcs

SRC_NAMES	=	ctx_create.c \
				ctx_delete.c \
				ctx_transfer.c \
				ctx_wild_manip.c \
				global.c \
				ptr_utils.c \
				static.c
SRC_PATH	=	$(addprefix $(SRC_DIR)/ft_gc_, $(SRC_NAMES))

OBJS		=	$(SRC_PATH:.c=.o)

NAME		=	libftgc.a
LIB_DIR		=	lib
LIB_HEADER	=	ft_gc.h

CC			=	@clang
CFLAGS		=	-Wall -Wextra -Werror

AR			=	@ar -rc
RL			=	@ranlib

RM			=	@rm -f

EXAMPLE_DIR	=	examples
EX1_SRC		=	$(EXAMPLE_DIR)/ex01_introduction.c
EX1_BIN		=	$(EXAMPLE_DIR)/_ex01
EX2_SRC		=	$(EXAMPLE_DIR)/ex02_context.c
EX2_BIN		=	$(EXAMPLE_DIR)/_ex02

.c.o:
			$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
			@echo "...Compiled source code to object files"
			@mkdir -p $(LIB_DIR)
			$(AR) $(LIB_DIR)/$(NAME) $(OBJS)
			$(RL) $(LIB_DIR)/$(NAME)
			@echo "...Static library linked from object files"
			@cp $(INC_DIR)/$(LIB_HEADER) $(LIB_DIR)/$(LIB_HEADER)
			@printf "\033[92m\033[1mHeader and library placed in folder $(LIB_DIR)\033[0m\n"

examples:	$(NAME)
			$(CC) $(CFLAGS) $(EX1_SRC) -I $(LIB_DIR) -L $(LIB_DIR) -lftgc -o $(EX1_BIN)
			$(CC) $(CFLAGS) $(EX2_SRC) -I $(LIB_DIR) -L $(LIB_DIR) -lftgc -o $(EX2_BIN)
			@printf "\033[92m\033[1mExample binaries placed in folder $(EXAMPLE_DIR)\033[0m\n"

all:		$(NAME)

clean:
			$(RM) $(OBJS)
			@echo "Object files cleaned"

fclean:		clean
			$(RM) $(LIB_DIR)/$(NAME) $(LIB_DIR)/$(LIB_HEADER) $(EX1_BIN) $(EX2_BIN)
			@rmdir --ignore-fail-on-non-empty $(LIB_DIR)
			@echo "All binaries cleaned"

re:			fclean all

.PHONY:		all example clean fclean re
