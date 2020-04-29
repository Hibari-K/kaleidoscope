CXX = clang++
TARGET = interpreter
SRCS = lexer.cpp parser.cpp driver.cpp
OBJS := $(SRCS:.cpp=.o)
#INCLUDES = -I../include -I../tools
LIBS = `llvm-config --cxxflags --ldflags --system-libs --libs core`
CXXFLAGS = -g -O3 $(LIBS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)