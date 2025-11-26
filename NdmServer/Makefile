TARGET_LIB := ndmserver.a

BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.cxx')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CXXFLAGS := $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.cxx.o: %.cxx
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR) || true
-include $(DEPS)
