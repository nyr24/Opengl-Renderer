SRC_DIR=src
BUILD_DIR=build
DEBUG_DIR=$(BUILD_DIR)/debug
RELEASE_DIR=$(BUILD_DIR)/release
INCLUDE_DIR=include
SRCS=main.cpp renderer.cpp utils.cpp window.cpp geometryObject.cpp texture.cpp globals.cpp camera.cpp meshes.cpp userDefinedObjects.cpp
OBJS=$(SRCS:.cpp=.o)
DEBUG_OBJS=$(addprefix $(DEBUG_DIR)/, $(OBJS))
RELEASE_OBJS=$(addprefix $(RELEASE_DIR)/, $(OBJS))
EXE=app
DEBUG_EXE=$(DEBUG_DIR)/$(EXE)
RELEASE_EXE=$(RELEASE_DIR)/$(EXE)
CXX=clang++
CXXFLAGS=-I$(INCLUDE_DIR) -I/usr/include/GLFW -I/usr/include/GL -Iglew.h -Iglfw3.h -std=c++20 -lGLEW -lGLU -lGL -lglfw -Wall -Wextra
DEBUG_FLAGS=-g -O0 -DDEBUG
RELEASE_FLAGS=-O3 -DNDEBUG

all: debug

rebuild_dbg: clean_dbg debug

rebuild_rel: clean_rel release

debug: prep_dbg $(DEBUG_EXE)

release: prep_rel $(RELEASE_EXE)

# debug
$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ $^
	echo "debug build completed!"

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

# release
$(RELEASE_EXE): $(RELEASE_OBJS)
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^
	echo "release build completed!"

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

# util
prep_dbg:
	mkdir -p $(BUILD_DIR) $(DEBUG_DIR)

prep_rel:
	mkdir -p $(BUILD_DIR) $(RELEASE_DIR)

clean_dbg:
	rm -f $(DEBUG_DIR)/*.o $(DEBUG_EXE)

clean_rel:
	rm -f $(RELEASE_DIR)/*.o $(RELEASE_EXE)

run_dbg:
	$(DEBUG_EXE)

run_rel:
	$(RELEASE_EXE)
