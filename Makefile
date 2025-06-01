SRC_DIR=src
BUILD_DIR=build
DEBUG_DIR=$(BUILD_DIR)/debug
RELEASE_DIR=$(BUILD_DIR)/release
INCLUDE_DIR=include
SRCS=$(notdir $(wildcard $(SRC_DIR)/*.cpp))
OBJS=$(SRCS:.cpp=.o)
DEPS=$(OBJS:.o=.d)
DEBUG_OBJS=$(addprefix $(DEBUG_DIR)/, $(OBJS))
RELEASE_OBJS=$(addprefix $(RELEASE_DIR)/, $(OBJS))
DEBUG_DEPS=$(addprefix $(DEBUG_DIR)/, $(DEPS))
RELEASE_DEPS=$(addprefix $(RELEASE_DIR)/, $(DEPS))
EXE=app
DEBUG_EXE=$(DEBUG_DIR)/$(EXE)
RELEASE_EXE=$(RELEASE_DIR)/$(EXE)
CXX=clang++
CXXFLAGS=-I$(INCLUDE_DIR) -Iglew.h -Iglfw3.h -std=c++20 -lGLEW -lGLU -lGL -lglfw -Wall -Wextra
DEBUG_FLAGS=-g -O0 -DDEBUG
RELEASE_FLAGS=-O3 -DNDEBUG

$(info NEW = $(SRCS))

all: debug

rebuild_dbg: clean_dbg debug

rebuild_rel: clean_rel release

debug: prep_dbg $(DEBUG_EXE)

release: prep_rel $(RELEASE_EXE)

-include $(DEBUG_DEPS)
-include $(RELEASE_DEPS)

# debug
$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $^
	echo "debug build completed!"

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD $(DEBUG_FLAGS) -o $@ -c $<

# release
$(RELEASE_EXE): $(RELEASE_OBJS)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $@ $^
	echo "release build completed!"

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD $(RELEASE_FLAGS) -o $@ -c $<

# util
prep_dbg:
	mkdir -p $(BUILD_DIR) $(DEBUG_DIR)

prep_rel:
	mkdir -p $(BUILD_DIR) $(RELEASE_DIR)

clean_dbg:
	rm -f $(DEBUG_DIR)/$(EXE) $(DEBUG_DIR)/*.o $(DEBUG_DIR)/*.d

clean_rel:
	rm -f $(RELEASE_DIR)/$(EXE) $(RELEASE_DIR)/*.o $(RELEASE_DIR)/*.d

run_dbg:
	$(DEBUG_EXE)

run_rel:
	$(RELEASE_EXE)
