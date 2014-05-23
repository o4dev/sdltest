SRCDIR  := src
SRCLIST := sdltest.c screen.c

OUTDIR  := bin
OUTFILE := sdltest

LIB     := -lSDL2

SWITCH  := -std=c99

OUTFILE := $(OUTDIR)/$(OUTFILE)
SRCLIST := $(addprefix $(SRCDIR)/,$(SRCLIST))

clean:
	@echo "Cleaning..."
	@rm -fr $(OUTDIR)

fix:
	@echo "Fixing..."
	@mkdir $(OUTDIR)

build:
	@echo "Building..."
	@gcc $(SRCLIST) \
	     -o $(OUTFILE) \
	     $(LIB) \
	     $(SWITCH)
	  
all: clean fix build

run:
	@if [ -f $(OUTFILE) ]; then \
		$(OUTFILE); \
	else \
		echo "No output file -- build failed?"; \
	fi

allr: all run

