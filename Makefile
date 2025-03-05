NAME		=	webserv
SRC			=	src/main.cpp src/Server.cpp src/RequestHandler.cpp src/ConfigParser.cpp src/LocationConfig.cpp \
				src/Client.cpp src/CGIHandler.cpp src/Logger.cpp src/Request.cpp src/Response.cpp src/Router.cpp
OBJ			=	$(SRC:.cpp=.o)
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
