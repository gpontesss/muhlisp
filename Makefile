CC := gcc
CFLAGS := -std=c99 -Wall -ledit -lm
DEPS := mpc.c parsing.c val.c
OUTDIR := out

.PHONY: all
all: $(OUTDIR)/muhlisp

$(OUTDIR)/%: $(DEPS) %.c | $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OUTDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(OUTDIR)
