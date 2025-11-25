# TARGET_EXEC := server
#
# BUILD_DIR := ./build
# SRC_DIRS := ./src
# CXXFLAGS=-g -pthread -I/sw/include/root
# NDM_SERVER := ./NdmServer/build/ndmserver.a
#
# SRCS := $(shell find $(SRC_DIRS) -name '*.cxx')
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# DEPS := $(OBJS:.o=.d)
# INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# CXXFLAGS := $(INC_FLAGS) -MMD -MP
#
# $(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
# 	$(CXX) $(OBJS)  -o $@ $(LDFLAGS)
#
# $(BUILD_DIR)/%.cxx.o: %.cxx
# 	mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@
#
# .PHONY: clean
# clean:
# 	rm -f main.o main
#
#
TARGET_EXEC := server
BUILD_DIR := ./build
SRC_DIRS := ./src
NDM_SERVER_DIR := ./NdmServer
NDM_SERVER_LIB := $(NDM_SERVER_DIR)/build/ndmserver.a
NDM_SERVER_INC := $(NDM_SERVER_DIR)/src

SRCS := $(shell find $(SRC_DIRS) -name '*.cxx')
OBJS := $(SRCS:%.cxx=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CXXFLAGS := -I$(NDM_SERVER_INC) $(INC_FLAGS) -MMD -MP -std=c++20
LDFLAGS := -lm -lpthread  -lboost_program_options

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) | $(NDM_SERVER_LIB)
	$(CXX) $(OBJS) $(NDM_SERVER_LIB) -o $@ $(LDFLAGS)

$(NDM_SERVER_LIB):
	$(MAKE) -C $(NDM_SERVER_DIR)

$(BUILD_DIR)/%.o: %.cxx
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	$(MAKE) -C $(NDM_SERVER_DIR) clean
	rm -rf $(BUILD_DIR)
