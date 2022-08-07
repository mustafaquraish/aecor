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

all: aecor aecor_sh

$(DEPFILES):
	@mkdir -p "$(@D)"

aecor: $(OBJS) | $(BIN)
	@echo "- Building final executable $@"
	@clang++ $(CFLAGS) $^ -o $@ -lm

.PHONY: aecor_sh
aecor_sh:
	@echo "- Building self-host compiler"
	@./aecor compiler/main.ae && mv ./out ./aecor_sh

build/%.o : src/%.cc build/deps/%.d | build/deps
	@echo "- Compiling $@"
	@mkdir -p "$(@D)"
	@clang++ $(DEPFLAGS) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p $@

build/deps:
	@mkdir -p $@

clean:
	rm -rf build aecor

include $(wildcard $(DEPFILES))
