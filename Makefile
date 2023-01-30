NAME		= ircserv

SRCS		= main.cpp ManageServer.cpp ManageServerUtils.cpp parsing.cpp utils.cpp \
				class/Client.cpp	\
				class/Channel.cpp	\
				class/Server.cpp	\
				commands/join.cpp 	\
				commands/ping.cpp 	\
				commands/privmsg.cpp	\
				commands/topic.cpp	\
				commands/list.cpp 	\
				commands/mode.cpp	\
				commands/invite.cpp	\
				commands/nick.cpp	\
				commands/notice.cpp	\
				commands/pass.cpp	\
				commands/names.cpp	\
				commands/part.cpp	\
				commands/quit.cpp   \
				commands/kick.cpp 	\
				commands/oper.cpp	\
				commands/user.cpp 	\
				commands/kill.cpp	\
				commands/motd.cpp

DIR_SRCS	= srcs/

DIR_OBJS	= objs/

OBJS		= ${SRCS:%.cpp=${DIR_OBJS}%.o}

CXX			= c++

DEP			= ${OBJS:%.o=%.d}

CPPFLAGS	= -Wall -Wextra -Werror -MMD -MP -g3 -D_GLIBCXX_DEBUG -std=c++98 -c -I includes/ 

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