
# Top-level Makefile
CC      = gcc
CFLAGS  = -Wall -g
SRC     = main.c \
          machine.c \
          debugScreen.c \
          asm.c
TARGET  = stags
LIBS    = -lncurses

SUBDIR  = maika
# where the submodule will produce the file
SUB_OUT = $(SUBDIR)/$(notdir $(OUTPUT))
# top-level view of generated file (default to maika/output.c)
OUTPUT ?= $(SUBDIR)/output.c

# GENERATE=0 -> only run generator if file missing
# GENERATE=1 -> force generator
GENERATE ?= 0
# pass extra args to submodule if needed (not used here because we use INPUT/OUTPUT)
GEN_ARGS ?=

all: $(TARGET)

$(TARGET): $(SRC) $(OUTPUT)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LIBS)

# Force generation
ifeq ($(GENERATE),1)
$(OUTPUT):
	@echo "Forcing generation in $(SUBDIR)..."
	$(MAKE) -C $(SUBDIR) generate INPUT="$(INPUT)" OUTPUT="$(notdir $(OUTPUT))" GEN_ARGS="$(GEN_ARGS)"
else
# Only generate if missing
$(OUTPUT):
	@if [ -f $(OUTPUT) ]; then \
	  echo "Generated file exists: $(OUTPUT) (skipping)"; \
	else \
	  echo "Generated file missing — invoking generator in $(SUBDIR)..."; \
	  $(MAKE) -C $(SUBDIR) generate INPUT="$(INPUT)" OUTPUT="$(notdir $(OUTPUT))" GEN_ARGS="$(GEN_ARGS)"; \
	fi
endif

.PHONY: generate regenerate run-submodule clean

# Explicit generate target (creates $(OUTPUT) in top-level by moving it)
generate:
	$(MAKE) -C $(SUBDIR) generate INPUT="$(INPUT)" OUTPUT="$(notdir $(OUTPUT))" GEN_ARGS="$(GEN_ARGS)"
	@mv $(SUBDIR)/$(notdir $(OUTPUT)) $(OUTPUT) || true

regenerate:
	$(MAKE) -C $(SUBDIR) clean
	$(MAKE) -C $(SUBDIR) generate INPUT="$(INPUT)" OUTPUT="$(notdir $(OUTPUT))" GEN_ARGS="$(GEN_ARGS)"
	@mv $(SUBDIR)/$(notdir $(OUTPUT)) $(OUTPUT) || true

# run-submodule will run generator but leave output in submodule folder
run-submodule:
	$(MAKE) -C $(SUBDIR) generate INPUT="$(INPUT)" OUTPUT="$(notdir $(OUTPUT))" GEN_ARGS="$(GEN_ARGS)"

clean:
	$(MAKE) -C $(SUBDIR) clean || true
	-rm -f $(TARGET) $(OUTPUT)




