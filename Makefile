BUILD_DIR  ?= build
BUILD_TYPE ?= Debug
GENERATOR  ?= Ninja
SAN        ?= ON

P ?=
S ?=
N ?=
T ?= *

CMAKE_FLAGS = -S . -B $(BUILD_DIR) -G $(GENERATOR) \
              -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
              -DLC_ENABLE_SANITIZERS=$(SAN)

PROBLEM_DIR = $(firstword $(wildcard problems/$(P)*))
BIN         = $(BUILD_DIR)/$(notdir $(PROBLEM_DIR))

.PHONY: help all configure build rebuild test t list case clean distclean new release nosan reconfigure ccdb submit watch

help:
	@printf "Targets:\n"
	@printf "  make all              Build and run all tests\n"
	@printf "  make build            Build everything\n"
	@printf "  make test             Run all tests\n"
	@printf "  make test P=0001      Run tests for one problem (regex match)\n"
	@printf "  make t P=0001         Alias for test P=...\n"
	@printf "  make list P=0001      List doctest cases in a problem\n"
	@printf "  make case P=0001 T='*basic*'   Run a single doctest case\n"
	@printf "  make submit P=0001    Copy 'class Solution { ... };' to clipboard (pbcopy)\n"
	@printf "  make watch P=0001     Rebuild + retest on save (requires entr)\n"
	@printf "  make new N=15 S=3sum  Scaffold problems/0015-3sum/\n"
	@printf "  make rebuild          Clean build artifacts and rebuild\n"
	@printf "  make clean            cmake --build --target clean\n"
	@printf "  make distclean        Remove $(BUILD_DIR)/ entirely\n"
	@printf "  make reconfigure      Force re-run of cmake configure\n"
	@printf "  make release          Configure as Release (BUILD_TYPE=Release)\n"
	@printf "  make nosan            Configure with SAN=OFF\n"
	@printf "  make ccdb             Symlink compile_commands.json to repo root\n"
	@printf "\nVariables: P=<problem-prefix>  S=<slug>  N=<number>  T=<doctest-glob>\n"

$(BUILD_DIR)/CMakeCache.txt:
	cmake $(CMAKE_FLAGS)

configure: $(BUILD_DIR)/CMakeCache.txt

reconfigure:
	cmake $(CMAKE_FLAGS)

build: configure
	cmake --build $(BUILD_DIR)

all: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

rebuild:
	cmake --build $(BUILD_DIR) --clean-first

test: build
ifeq ($(strip $(P)),)
	ctest --test-dir $(BUILD_DIR) --output-on-failure
else
	ctest --test-dir $(BUILD_DIR) -R $(P) --output-on-failure
endif

t: test

list: build
	@test -n "$(P)" || { echo "usage: make list P=<problem-prefix>"; exit 2; }
	@test -n "$(PROBLEM_DIR)" || { echo "no problem matching '$(P)*' under problems/"; exit 2; }
	./$(BIN) --list-test-cases

case: build
	@test -n "$(P)" || { echo "usage: make case P=<problem-prefix> T='*case-glob*'"; exit 2; }
	@test -n "$(PROBLEM_DIR)" || { echo "no problem matching '$(P)*' under problems/"; exit 2; }
	./$(BIN) --test-case="$(T)"

new:
	@test -n "$(N)" || { echo "usage: make new N=<number> S=<slug>"; exit 2; }
	@test -n "$(S)" || { echo "usage: make new N=<number> S=<slug>"; exit 2; }
	./scripts/new.sh $(N) $(S)

clean:
	@test -d $(BUILD_DIR) && cmake --build $(BUILD_DIR) --target clean || true

distclean:
	rm -rf $(BUILD_DIR)

release:
	$(MAKE) reconfigure BUILD_TYPE=Release SAN=OFF

nosan:
	$(MAKE) reconfigure SAN=OFF

ccdb: configure
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

submit:
	@test -n "$(P)" || { echo "usage: make submit P=<problem-prefix>"; exit 2; }
	@test -n "$(PROBLEM_DIR)" || { echo "no problem matching '$(P)*' under problems/"; exit 2; }
	@command -v pbcopy >/dev/null || { echo "pbcopy not found (macOS required)"; exit 2; }
	@awk '/^class Solution/{p=1} p{print} p && /^};/{exit}' $(PROBLEM_DIR)/solution.hpp | pbcopy
	@echo "copied class Solution from $(PROBLEM_DIR)/solution.hpp to clipboard"

watch:
	@test -n "$(P)" || { echo "usage: make watch P=<problem-prefix>"; exit 2; }
	@test -n "$(PROBLEM_DIR)" || { echo "no problem matching '$(P)*' under problems/"; exit 2; }
	@command -v entr >/dev/null || { echo "entr not found; install with: brew install entr"; exit 2; }
	@echo "watching $(PROBLEM_DIR)/ and common/ -- ctrl-c to stop"
	@find $(PROBLEM_DIR) common \( -name '*.hpp' -o -name '*.cpp' \) | entr -c $(MAKE) --no-print-directory test P=$(P)
