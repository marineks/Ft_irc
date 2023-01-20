NAME		= ircserv

SRCS		= main.cpp Client.cpp ManageServer.cpp Server.cpp parsing.cpp Channel.cpp utils.cpp \
				commands/invite.cpp	\
				commands/join.cpp 	\
				commands/list.cpp 	\
				commands/mode.cpp 	\
				commands/modeUtils.cpp		\
				commands/nick.cpp	\
				commands/oper.cpp	\
				commands/pass.cpp	\
				commands/ping.cpp 	\
				commands/topic.cpp	\

DIR_SRCS	= srcs/

DIR_OBJS	= objs/

OBJS		= ${SRCS:%.cpp=${DIR_OBJS}%.o}

CXX			= c++

DEP			= ${OBJS:%.o=%.d}

CPPFLAGS	= -Wall -Wextra -Wunused -MMD -MP -g3 -std=c++98 -c -I includes/ 

RM 			= rm -f

all:	${NAME}

${NAME} : ${OBJS}
	${CXX} $^ -o $@

${OBJS} : ${DIR_OBJS}%.o: ${DIR_SRCS}%.cpp
	mkdir -p ${@D}
	${CXX} ${CPPFLAGS} $< -o $@
-include ${DEP}

clean:
	${RM} -r ${DIR_OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re