##
# Project Aecor
#
# @file
# @version 0.1

SRCS      = $(wildcard src/*.cc)
DEPFILES := $(patsubst src/%.cc,build/deps/%.d,$(SRCS))
OBJS      = $(patsubst src/%.cc,build/%.o,$(SRCS))
DEPFLAGS  = -MT $@ -MMD -MP -MF build/deps/$*.d
CFLAGS    = -ggdb3 -I src/ -std=c++20

.PHONY: all run clean

all: compiler

$(DEPFILES):
	@mkdir -p "$(@D)"

compiler: $(OBJS) | $(BIN)
	@echo "- Building final executable $@"
	@clang++ $(CFLAGS) $^ -o $@ -lm

build/%.o : src/%.cc build/deps/%.d | build/deps
	@echo "- Compiling $@"
	@mkdir -p "$(@D)"
	@clang++ $(DEPFLAGS) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p $@

build/deps:
	@mkdir -p $@

clean:
	rm -rf build compiler

include $(wildcard $(DEPFILES))
