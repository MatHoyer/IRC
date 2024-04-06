NAME        :=	ircserv
CXX         :=	c++
CXXFLAGS    :=	-Wall -Wextra -Werror  -MMD -std=c++98 -pedantic-errors -g3

BUILD_DIR   :=	build

SRCS        :=	src/main.cpp \
								src/signals.cpp \
								src/core/Server.cpp \
								src/core/Logger.cpp \
								src/client/Client.cpp \
								src/client/ClientManager.cpp \
								src/client/Bot.cpp \
								src/client/alreadyKnowData.cpp \
								src/buffer/Buffer.cpp \
								src/message/Message.cpp \
								src/channel/Channel.cpp \
								src/channel/ChannelManager.cpp \
								src/commands/CommandManager.cpp \
								src/commands/doBotThings.cpp \
								src/replies/ircRPL.cpp \
								src/replies/ircERR.cpp \
								src/utils/utils.cpp \
								src/commands/connection/cap.cpp \
								src/commands/connection/pass.cpp \
								src/commands/connection/user.cpp \
								src/commands/connection/nick.cpp \
								src/commands/connection/ping.cpp \
								src/commands/connection/pong.cpp \
								src/commands/connection/quit.cpp \
								src/commands/connection/error.cpp \
								src/commands/server/motd.cpp \
								src/commands/channel/join.cpp \
								src/commands/channel/part.cpp \
								src/commands/privmsg.cpp \
								src/commands/channel/topic.cpp \
								src/commands/channel/invite.cpp \
								src/commands/channel/kick.cpp \
								src/commands/server/mode.cpp

OBJS        := $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
