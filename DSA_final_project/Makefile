CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
SRCS     = main.cpp StoreApp.cpp Utils.cpp Catalog.cpp ProductBST.cpp \
           ShoppingCart.cpp BrandRing.cpp OrderQueue.cpp OrderStack.cpp \
           Sorting.cpp Searching.cpp
OBJS     = $(SRCS:.cpp=.o)
TARGET   = gadgetstore

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
