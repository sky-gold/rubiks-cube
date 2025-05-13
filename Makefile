# Variables
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests

# Targets
.PHONY: build test run clean debug_build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .. && make

debug_build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

test: build
	@$(BUILD_DIR)/$(TEST_DIR)/unit_tests

run: build
	@$(BUILD_DIR)/$(SRC_DIR)/rubiks_cube_solver

clean:
	@rm -rf $(BUILD_DIR)