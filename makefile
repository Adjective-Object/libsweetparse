#######################
# BULDING THE LIBRARY #
#######################

all: lib example

lib = libsweetexpressions.so
src = src/datatypes.c
libobjs = $(src:.c=.o)

LIBS =
CFLAGS = -g -Wall -std=c99 -fPIC

lib: $(lib)

# build the libs from the shared object files
$(lib): $(libobjs)
	gcc -shared -o $@ -fPIC $^

src/%.o : src/%.c src/%.h
	gcc $(CFLAGS) -c $< -o $@

###############################
# BUILDING THE PARSER EXAMPLE #
###############################

example_exec = sweetparse
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
