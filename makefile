CXX = g++
CXX_FLAGS = -std=c++17
CXX_FLAGS += -Wuninitialized -Wall -Wextra -Werror -pedantic -Wfatal-errors # source code quality.
CXX_FLAGS += -O3 -ffast-math # non-debug version.
#CXX_FLAGS += -O0 -g # debug version.
CXX_FLAGS += -I./ # include paths.

LINK_FLAGS = # -lstdc++fs # filesystem

SRCS = lkl.cc tour.cc \
	kmove.cc \
	hill_climber.cc \
    point_quadtree/node.cc \
    point_quadtree/point_quadtree.cc \
    point_quadtree/point_inserter.cc \
    cycle_check.cc \
    cycle.cc \
    feasibility/segment.cc \
    feasibility/multi_segment.cc

%.o: %.cc; $(CXX) $(CXX_FLAGS) -o $@ -c $<

OBJS = $(SRCS:.cc=.o)

all: $(OBJS); $(CXX) $^ $(LINK_FLAGS) -o lkl.out

clean: ; rm -rf lkl.out $(OBJS) *.dSYM
