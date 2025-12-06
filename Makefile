CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2
UTILDIR := utilities

UTIL_SRC := $(wildcard $(UTILDIR)/*.c)
UTIL_HDR := $(wildcard $(UTILDIR)/*.h)

DAY_MAINS := $(wildcard day*/main.c)
DAYS := $(patsubst day%/main.c,%,$(DAY_MAINS))

SAMPLY ?= samply
SAMPLY_REC_OPTS ?=

SHELL := /bin/bash

.PHONY: timeall

.PHONY: all list clean run $(DAYS)

all: list

list:
	@echo "Available days: $(DAYS)"
	@echo "Build with: make <day>"
	@echo "Run with:   make run <day>"
	@echo "Profile with: make samply <day>"

$(DAYS): %: day%/main
	@:

day%/main: day%/main.c $(UTIL_SRC) $(UTIL_HDR)
	$(CC) $(CFLAGS) -I $(UTILDIR) $< $(UTIL_SRC) -o $@

run: $(filter-out run,$(MAKECMDGOALS))
	@d="$(firstword $(filter-out run,$(MAKECMDGOALS)))"; \
	if [ -z "$$d" ]; then \
		echo "Usage: make run <day>"; \
		exit 2; \
	fi; \
	cd day$$d && ./main

clean:
	@for d in $(DAYS); do \
		rm -f day$$d/main; \
	done

timeall:
	@out="timings.txt"; : > "$$out"; \
	days=($(DAYS)); \
	for d in "$${days[@]}"; do \
		$(MAKE) $$d >/dev/null; \
		( \
			cd day$$d; \
			TIMEFORMAT='%3R'; \
			printf "day%s " "$$d" >> ../$$out; \
			{ time ./main >/dev/null; } 2>> ../$$out; \
		); \
	done; \
	echo "Wrote timings to $$out"

samply: $(filter-out samply,$(MAKECMDGOALS))
	@d="$(firstword $(filter-out samply,$(MAKECMDGOALS)))"; \
	if [ -z "$$d" ]; then \
		echo "Usage: make samply <day>"; \
		exit 2; \
	fi; \
	if ! command -v "$(SAMPLY)" >/dev/null 2>&1; then \
		echo "Error: 'samply' not found. Install with: brew install samply"; \
		exit 127; \
	fi; \
	cd day$$d && $(SAMPLY) record $(SAMPLY_REC_OPTS) -- ./main