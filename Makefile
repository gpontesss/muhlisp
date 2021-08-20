CC := gcc
OUTDIR := out

.PHONY: all
all: $(OUTDIR)/hello_world

$(OUTDIR)/%: %.c | $(OUTDIR)
	$(CC) $^ -o $@

$(OUTDIR):
	mkdir -p $@


.PHONY: clean
clean:
	rm -rf $(OUTDIR)
