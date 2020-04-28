CXX = clang++
TARGET = interpreter
SRCS = lexer.cpp parser.cpp driver.cpp
OBJS := $(SRCS:.cpp=.o)
#INCLUDES = -I../include -I../tools
LIBS = `llvm-config --cxxflags`
CXXFLAGS = -g -O3 $(LIBS)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)