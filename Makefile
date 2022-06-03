TARGET_EXEC ?= main

BUILD_DIR ?= ./build
CC=g++ -g -std=c++11

SRCS := $(shell find -name '*.cpp' )
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CPPFLAGS ?= -std=c++11
LDFLAGS ?= -g

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	./$(TARGET_EXEC)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_BUILD) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_BUILD ?= mkdir -p
