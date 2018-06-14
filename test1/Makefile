CXX = g++
CC  = gcc

LDFLAGS += -lsqlite3

src := test1.cpp 
#src := main.cpp 

objects=$(patsubst %.cpp,%.o, $(src))
target = sqlite3_test

.PHONY:all
all:${target}

${target}:$(objects)
	$(Q)$(CXX) $^ ${LDFLAGS} -o $@
	
%.o:%.cpp
	$(Q)$(CXX) -c $^ ${CFLAGS} ${CFLAGS_FULL} -o $@

.PHONY:clean
clean:
	@rm -f  ${target}
	@find -name "*.o" -execdir rm {} \;

	

