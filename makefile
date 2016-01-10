#######################
# BULDING THE LIBRARY #
#######################

all: lib example

lib = libsweetexpressions.so
src = src/sweetexpressions/datatypes.c src/sweetexpressions/parser.c \
	  src/sweetexpressions/charclasses.c src/sweetexpressions.c \
	  src/sweetexpressions/stream.c
libobjs = $(src:.c=.o)

CFLAGS = -g -Wall -std=c99 -fPIC
LDFLAGS = -shared

lib: $(lib)

# link the libs from the object files
$(lib): $(libobjs)
	gcc $(LDFLAGS) -o $@ $^

src/%.o : src/%.c src/%.h
	gcc $(CFLAGS) -c $< -o $@

###############################
# BUILDING THE PARSER EXAMPLE #
###############################

example_exec = example/sweetparse
example_src = example/sweetparse.c
example_obj = $(example_src:.c=.o)
example_cflags = -g -Wall -std=c99 -Isrc
example_ldflags = -L. -lsweetexpressions

example: $(example_exec)

$(example_obj): $(example_src) $(lib)
	gcc $(example_cflags) -c $^ -o $@

$(example_exec): $(example_obj)
	gcc $(example_ldflags) $^ -o $@

################
# MISC UTILITY #
################

# clean up the example and lib thigs
clean:
	rm -f $(libobjs) $(lib) $(example_obj) $(example_exec)

# debug helper to print makefile variables
print-%:
	@echo '$*=$($*)'

###########
# INSTALL #
###########

installdir = /usr/lib
headerdir = /usr/include
subheaderdir = $(headerdir)/sweetexpressions

coreheader = src/sweetexpressions.h
subheaders = src/sweetexpressions/*.h

.PHONY: install
install: $(lib)
	cp $(lib) $(installdir)
	mkdir -p $(headerdir)
	cp $(coreheader) $(headerdir)
	mkdir -p $(subheaderdir)
	cp -r $(subheaders) $(subheaderdir)

###########
# TESTING #
###########

.PHONY:  tests/*.input
.SILENT: tests/*.input
test: tests/*.input

tests/*.swexp.input:
	$(example_exec) $@ | diff - $(@:.swexp.input=.swexp.output) 
	if [ $$? = 0 ]; then echo "test '$@' passed"; else echo "test failed"; fi

