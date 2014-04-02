NAME = miro

all: $(NAME)

include Makedefs
SOURCES -=  parse.cpp lexer.cpp
OBJS -=  parse.o lexer.o
# 
# lexer.cpp: lexer.lex
# 	$(ECHO) "Flex-ing lexer.lex"
# 	$(FLEX) -o$@ lexer.lex
# 
# parse.cpp: parse.y
# 	$(ECHO) "Bison-ing parse.y"
# 	$(BISON) -d -o $@ parse.y
# 	@if [ -f parse.hpp ]; then \
# 		mv parse.hpp parse.cpp.h; \
# 	fi
# 	@if [ -f parse.tab.hpp ]; then \
# 		mv parse.tab.hpp parse.cpp.h; \
# 	fi
# 	@if [ -f parse.tab.h ]; then \
# 		mv parse.tab.h parse.cpp.h; \
# 	fi

-include .deps/*.d

$(NAME): $(OBJS)
	$(ECHO) "Linking $@..."
	$(CC) -o $@ *.o $(LIBS) $(LIBDIRS) #$(OBJS) 
	$(ECHO) "Built $@!"
	
