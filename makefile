# Source file list:
SRCLIST := sdltest.c screen.c
# Source dir:
SRCDIR  := src

# Output file name:
OUTFILE := sdltest
# Output dir:
OUTDIR  := bin

# Include dir:
INCDIR  := include

# Common libraries:
LIB     := -lSDL2 -lm
# Lib dir:
LIBDIR  := lib
# Windows-only libraries:
LIBWIN  := -lmingw32 -lSDL2main $(LIB)
# Note that the lib directory is necessary for
# compiling on/for Windows only, since the
# Linux lib directory should be /usr/local/lib

# Other compiler flags:
FLAGS   := -std=c99

# SDL2.dll's location (for fix-win):
SDLDLL  := lib/SDL2.dll

# Compiler for linux
CC      := gcc
# MinGW gcc command
CCWIN   := mingw32-gcc

# Concatenate output file and dir:
OUTFILE := $(OUTDIR)/$(OUTFILE)
# Prefix all source file names with the source dir name:
SRCLIST := $(addprefix $(SRCDIR)/,$(SRCLIST))

clean:
	@echo "Cleaning..."
	@if [ -d $(OUTDIR) ]; then \
		rm -fr $(OUTDIR); \
	fi

fix:
	@echo "Fixing..."
	@mkdir $(OUTDIR)

build:
	@echo "Building..."
	@$(CC) $(SRCLIST) \
		   -o $(OUTFILE) \
		   -I $(INCDIR) \
		   $(LIB) \
		   $(FLAGS)
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
	
# For Windows:
	
fix-win:
	@echo "Fixing for Windows..."
	@mkdir $(OUTDIR)
	@cp $(SDLDLL) $(OUTDIR)

build-win:
	@echo "Building for Windows..."
	@$(CCWIN) $(SRCLIST) \
			  -o $(OUTFILE).exe \
			  -I $(INCDIR) \
			  -L $(LIBDIR) $(LIBWIN) \
			  $(FLAGS) 
	@if [ -f $(OUTFILE).exe ]; then \
		echo "Windows build succeeded, now get that filth out of here."; \
	fi
	  
all-win: clean fix-win build-win
	
run-win:
	@if [ -f $(OUTFILE).exe ]; then \
		$(OUTFILE).exe; \
	else \
		echo "Unable to run $(OUTFILE).exe."; \
	fi
	
allr-win: all-win run-win
