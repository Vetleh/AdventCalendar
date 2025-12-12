CC      := clang
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O3
# CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O3 -g \
#           -fsanitize=address,undefined -fno-omit-frame-pointer
UTILDIR := utilities

UTIL_SRC := $(wildcard $(UTILDIR)/*.c)
UTIL_HDR := $(wildcard $(UTILDIR)/*.h)

PARTS := pt1 pt2

DAY_PT1_SRC := $(wildcard day*/pt1.c)
DAY_PT2_SRC := $(wildcard day*/pt2.c)

RAW_DAYS := \
  $(patsubst day%/pt1.c,%,$(DAY_PT1_SRC)) \
  $(patsubst day%/pt2.c,%,$(DAY_PT2_SRC))

DAYS := $(shell printf "%s\n" $(RAW_DAYS) | sort -nu)

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

RUNS ?= 10

timeall:
	@set -euo pipefail; \
	out="README.md"; \
	runs="$(RUNS)"; \
	[ -n "$$runs" ] || runs=10; \
	if [ -z "$(strip $(DAYS))" ]; then \
		echo "ERROR: DAYS is empty (no day*/pt1.c or day*/pt2.c found)" >&2; \
		exit 1; \
	fi; \
	{ \
		echo "# Timings"; \
		echo ""; \
		echo "Runs: $$runs"; \
		echo ""; \
		echo "| Day | P1 avg (µs) | P1 stdev (µs) | P1 peak RSS (KB) | P2 avg (µs) | P2 stdev (µs) | P2 peak RSS (KB) |"; \
		echo "|---:|---:|---:|---:|---:|---:|---:|"; \
	} > "$$out"; \
	\
	now_us() { perl -MTime::HiRes -e 'printf "%.0f\n", Time::HiRes::time()*1000000'; }; \
	debug() { [ "$(DEBUG)" = "1" ] && echo "DEBUG: $$*" >&2 || true; }; \
	\
	TIMEBIN="/usr/bin/time"; \
	if ! command -v "$$TIMEBIN" >/dev/null 2>&1; then \
		if command -v gtime >/dev/null 2>&1; then TIMEBIN="gtime"; else TIMEBIN=""; fi; \
	fi; \
	time_flavor="none"; \
	if [ -n "$$TIMEBIN" ]; then \
		if "$$TIMEBIN" -l true >/dev/null 2>&1; then time_flavor="mac"; else time_flavor="gnu"; fi; \
	fi; \
	\
	run_once() { \
		local start end dur rss_kb time_err; \
		start="$$(now_us)"; \
		if [ "$$time_flavor" = "mac" ]; then \
			time_err=$$({ "$$TIMEBIN" -l "$$@" >/dev/null; } 2>&1); \
			rss_kb=$$(awk '/maximum resident set size/ {printf "%.0f\n", $$1/1024}' <<<"$$time_err" 2>/dev/null || echo 0); \
		elif [ "$$time_flavor" = "gnu" ]; then \
			time_err=$$({ "$$TIMEBIN" -v "$$@" >/dev/null; } 2>&1); \
			rss_kb=$$(awk -F': *' '/Maximum resident set size/ {print $$2}' <<<"$$time_err" 2>/dev/null || echo 0); \
		else \
			"$$@" >/dev/null; \
			rss_kb=0; \
		fi; \
		end="$$(now_us)"; \
		dur=$$(( end - start )); \
		printf "%s %s\n" "$$dur" "$$rss_kb"; \
	}; \
	\
	mean()  { awk '{s+=$$1;n++} END{printf "%.2f\n", (n? s/n : 0)}'; }; \
	stdev() { awk '{x=$$1;s+=x;ss+=x*x;n++} END{if(n<=1){print "0.00"; exit} v=(ss-(s*s)/n)/(n-1); if(v<0)v=0; printf "%.2f\n", sqrt(v)}'; }; \
	maxv()  { awk 'BEGIN{m=0} {if($$1>m)m=$$1} END{print m}'; }; \
	\
	for d in $(DAYS); do \
		debug "build day=$$d"; \
		$(MAKE) "$$d" >/dev/null; \
		cd "day$$d"; \
		\
		pt1_times=(); pt1_rss=(); \
		for ((i=0;i<runs;i++)); do \
			read -r t r < <(run_once ./pt1); \
			pt1_times+=("$$t"); pt1_rss+=("$$r"); \
		done; \
		pt1_avg=$$(printf "%s\n" "$${pt1_times[@]}" | mean); \
		pt1_sd=$$(printf "%s\n" "$${pt1_times[@]}" | stdev); \
		pt1_peak=$$(printf "%s\n" "$${pt1_rss[@]}" | maxv); \
		\
		pt2_times=(); pt2_rss=(); \
		for ((i=0;i<runs;i++)); do \
			read -r t r < <(run_once ./pt2); \
			pt2_times+=("$$t"); pt2_rss+=("$$r"); \
		done; \
		pt2_avg=$$(printf "%s\n" "$${pt2_times[@]}" | mean); \
		pt2_sd=$$(printf "%s\n" "$${pt2_times[@]}" | stdev); \
		pt2_peak=$$(printf "%s\n" "$${pt2_rss[@]}" | maxv); \
		\
		cd ..; \
		printf "| day%s | %s | %s | %s | %s | %s | %s |\n" "$$d" "$$pt1_avg" "$$pt1_sd" "$$pt1_peak" "$$pt2_avg" "$$pt2_sd" "$$pt2_peak" >> "$$out"; \
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
