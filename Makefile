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
CFLAGS=-I$(INCLUDE_DIR) -Iglew.h -Iglfw3.h -std=c++20 -lGLEW -lGLU -lGL -lglfw -Wall -Wextra
DEBUG_FLAGS=-g -O0 -DDEBUG
RELEASE_FLAGS=-O3 -DNDEBUG

all: debug

rebuild_dbg: clean_dbg debug

rebuild_rel: clean_rel release

debug: prep_dbg $(DEBUG_EXE)

release: prep_rel $(RELEASE_EXE)

# debug
$(DEBUG_EXE): $(DEBUG_OBJS)
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^
	echo "debug build completed!"

$(DEBUG_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/globals.hpp $(INCLUDE_DIR)/utils.hpp \
	$(INCLUDE_DIR)/renderer.hpp $(INCLUDE_DIR)/window.hpp $(INCLUDE_DIR)/geometryObject.hpp \
	$(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/vec.hpp $(INCLUDE_DIR)/animation.hpp  \
	$(INCLUDE_DIR)/camera.hpp $(INCLUDE_DIR)/texture.hpp $(INCLUDE_DIR)/meshes.hpp \
	$(INCLUDE_DIR)/userDefinedObjects.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/globals.o: $(SRC_DIR)/globals.cpp $(INCLUDE_DIR)/globals.hpp $(INCLUDE_DIR)/camera.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/renderer.o: $(SRC_DIR)/renderer.cpp $(INCLUDE_DIR)/renderer.hpp $(INCLUDE_DIR)/utils.hpp \
	$(INCLUDE_DIR)/geometryObject.hpp $(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/sharedTypes.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/window.o: $(SRC_DIR)/window.cpp $(INCLUDE_DIR)/window.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/utils.o: $(SRC_DIR)/utils.cpp $(SRC_DIR)/globals.cpp $(INCLUDE_DIR)/utils.hpp $(INCLUDE_DIR)/camera.hpp $(INCLUDE_DIR)/globals.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/texture.o: $(SRC_DIR)/texture.cpp $(INCLUDE_DIR)/texture.hpp $(INCLUDE_DIR)/renderer.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/geometryObject.o: $(SRC_DIR)/geometryObject.cpp $(INCLUDE_DIR)/geometryObject.hpp $(INCLUDE_DIR)/renderer.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/camera.o: $(SRC_DIR)/camera.cpp $(INCLUDE_DIR)/camera.hpp $(INCLUDE_DIR)/vec.hpp $(INCLUDE_DIR)/math.hpp \
	$(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/globals.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/meshes.o: $(SRC_DIR)/meshes.cpp $(INCLUDE_DIR)/meshes.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

$(DEBUG_DIR)/userDefinedObjects.o: $(SRC_DIR)/userDefinedObjects.cpp $(INCLUDE_DIR)/userDefinedObjects.hpp
	$(CXX) $(CFLAGS) $(DEBUG_FLAGS) -o $@ -c $<

# release
$(RELEASE_EXE): $(RELEASE_OBJS)
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^
	echo "release build completed!"

$(RELEASE_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/globals.hpp $(INCLUDE_DIR)/utils.hpp \
	$(INCLUDE_DIR)/renderer.hpp $(INCLUDE_DIR)/window.hpp $(INCLUDE_DIR)/geometryObject.hpp \
	$(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/vec.hpp $(INCLUDE_DIR)/animation.hpp  \
	$(INCLUDE_DIR)/camera.hpp $(INCLUDE_DIR)/texture.hpp $(INCLUDE_DIR)/meshes.hpp \
	$(INCLUDE_DIR)/userDefinedObjects.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/globals.o: $(SRC_DIR)/globals.cpp $(INCLUDE_DIR)/globals.hpp $(INCLUDE_DIR)/camera.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/renderer.o: $(SRC_DIR)/renderer.cpp $(INCLUDE_DIR)/renderer.hpp $(INCLUDE_DIR)/utils.hpp \
	$(INCLUDE_DIR)/geometryObject.hpp $(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/sharedTypes.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/window.o: $(SRC_DIR)/window.cpp $(INCLUDE_DIR)/window.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/utils.o: $(SRC_DIR)/utils.cpp $(INCLUDE_DIR)/utils.hpp $(INCLUDE_DIR)/camera.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/texture.o: $(SRC_DIR)/texture.cpp $(INCLUDE_DIR)/texture.hpp $(INCLUDE_DIR)/renderer.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/geometryObject.o: $(SRC_DIR)/geometryObject.cpp $(INCLUDE_DIR)/geometryObject.hpp $(INCLUDE_DIR)/renderer.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/camera.o: $(SRC_DIR)/camera.cpp $(INCLUDE_DIR)/camera.hpp $(INCLUDE_DIR)/vec.hpp $(INCLUDE_DIR)/math.hpp \
	$(INCLUDE_DIR)/matrix.hpp $(INCLUDE_DIR)/globals.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/meshes.o: $(SRC_DIR)/meshes.cpp $(INCLUDE_DIR)/meshes.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

$(RELEASE_DIR)/userDefinedObjects.o: $(SRC_DIR)/userDefinedObjects.cpp $(INCLUDE_DIR)/userDefinedObjects.hpp
	$(CXX) $(CFLAGS) $(RELEASE_FLAGS) -o $@ -c $<

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
