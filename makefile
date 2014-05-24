SRCDIR  := src
SRCLIST := sdltest.c screen.c

OUTDIR  := bin
OUTFILE := sdltest

LIB     := -lSDL2 -lm

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
	@if [ -f $(OUTFILE) ]; then \
		echo "Build succeeded."; \
	fi
	  
all: clean fix build

run:
	@if [ -f $(OUTFILE) ]; then \
		$(OUTFILE); \
	else \
		echo "Unable to run $(OUTFILE)."; \
	fi

allr: all run

