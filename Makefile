CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2
UTILDIR := utilities

UTIL_SRC := $(wildcard $(UTILDIR)/*.c)
UTIL_HDR := $(wildcard $(UTILDIR)/*.h)

PARTS := pt1 pt2

DAY_PT1_SRC := $(wildcard day*/pt1.c)
DAY_PT2_SRC := $(wildcard day*/pt2.c)

# Union of days that have pt1.c or pt2.c
DAYS := $(sort \
	$(patsubst day%/pt1.c,%,$(DAY_PT1_SRC)) \
	$(patsubst day%/pt2.c,%,$(DAY_PT2_SRC)) \
)

SAMPLY ?= samply
SAMPLY_REC_OPTS ?=

SHELL := /bin/bash

.PHONY: timeall
.PHONY: all list clean run samply $(DAYS)

all: list

list:
	@echo "Available days: $(DAYS)"
	@echo "Build with:    make <day>"
	@echo "Run with:      make run <day>"
	@echo "Profile with:  make samply <day> [pt1|pt2]"

$(DAYS): %: day%/pt1 day%/pt2
	@:

day%/pt1: day%/pt1.c $(UTIL_SRC) $(UTIL_HDR)
	$(CC) $(CFLAGS) -I $(UTILDIR) $< $(UTIL_SRC) -o $@

day%/pt2: day%/pt2.c $(UTIL_SRC) $(UTIL_HDR)
	$(CC) $(CFLAGS) -I $(UTILDIR) $< $(UTIL_SRC) -o $@

run: $(filter-out run,$(MAKECMDGOALS))
	@d="$(firstword $(filter-out run,$(MAKECMDGOALS)))"; \
	if [ -z "$$d" ]; then \
		echo "Usage: make run <day>"; \
		exit 2; \
	fi; \
	$(MAKE) $$d >/dev/null; \
	cd day$$d && ./pt1 && ./pt2

clean:
	@for d in $(DAYS); do \
		rm -f day$$d/pt1 day$$d/pt2; \
	done

timeall:
	@set -eu; \
	out="README.md"; \
	{ \
		echo "# Timings"; \
		echo ""; \
		echo "| Day | Part 1 (ms) | Part 2 (ms) |"; \
		echo "|---:|---:|---:|"; \
	} > "$$out"; \
	\
	now_ms() { perl -MTime::HiRes -e 'printf "%.0f\n", Time::HiRes::time()*1000'; }; \
	debug() { if [ "$(DEBUG)" = "1" ]; then echo "DEBUG: $$*" >&2; fi; }; \
	\
	for d in $(DAYS); do \
		debug "loop day=$$d"; \
		$(MAKE) "$$d" >/dev/null; \
		\
		cd "day$$d"; \
		\
		start_pt1=$$(now_ms); \
		./pt1 >/dev/null; \
		end_pt1=$$(now_ms); \
		pt1_ms=$$(( end_pt1 - start_pt1 )); \
		debug "day $$d pt1 start=$$start_pt1 end=$$end_pt1 ms=$$pt1_ms"; \
		\
		start_pt2=$$(now_ms); \
		./pt2 >/dev/null; \
		end_pt2=$$(now_ms); \
		pt2_ms=$$(( end_pt2 - start_pt2 )); \
		debug "day $$d pt2 start=$$start_pt2 end=$$end_pt2 ms=$$pt2_ms"; \
		\
		cd ..; \
		\
		printf "| day%s | %s | %s |\n" "$$d" "$$pt1_ms" "$$pt2_ms" >> "$$out"; \
	done; \
	\
	echo "" >> "$$out"; \
	echo "Wrote timings to $$out"

samply: $(filter-out samply,$(MAKECMDGOALS))
	@args=($(filter-out samply,$(MAKECMDGOALS))); \
	d="$${args[0]}"; \
	p="$${args[1]}"; \
	if [ -z "$$d" ]; then \
		echo "Usage: make samply <day> [pt1|pt2]"; \
		exit 2; \
	fi; \
	if [ -z "$$p" ]; then p="pt1"; fi; \
	if [ "$$p" != "pt1" ] && [ "$$p" != "pt2" ]; then \
		echo "Usage: make samply <day> [pt1|pt2]"; \
		exit 2; \
	fi; \
	if ! command -v "$(SAMPLY)" >/dev/null 2>&1; then \
		echo "Error: 'samply' not found. Install with: brew install samply"; \
		exit 127; \
	fi; \
	$(MAKE) day$$d/$$p >/dev/null; \
	cd day$$d && $(SAMPLY) record $(SAMPLY_REC_OPTS) -- ./$$p
