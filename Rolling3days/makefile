EXT = cpp
CXX = g++
EXEC = run

#CXXFLAGS = -W -Wall -DIL_STD -DNDEBUG -I. -I/home/sergeb/PROJET/LIB/PARAM/C++ -I/home/sergeb/LIB/CHRONO -g
CXXFLAGS = -W -Wall -DIL_STD -DNDEBUG -I. -g -O3 -std=c++11
#LDFLAGS = -L. -lilocplex -lcplex -lconcert -lm -lpthread -g
LDFLAGS = -L. -lilocplex -lcplex -lconcert -lm -lpthread -ldl -g -O3 -std=c++11

SRC = $(wildcard *.$(EXT))
OBJ = $(SRC:.$(EXT)=.o)
OBJDIR = .

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.$(EXT) *.h
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	@rm -rf *.o
	@rm -f $(EXEC)
