CC := gcc
CFLAGS := -std=c99 -Wall -ledit
OUTDIR := out

.PHONY: all
all: $(OUTDIR)/repl

$(OUTDIR)/%: %.c | $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OUTDIR):
	mkdir -p $@


.PHONY: clean
clean:
	rm -rf $(OUTDIR)
