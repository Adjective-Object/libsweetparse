program = sweetparse
srcs = src/sweetparse.c src/datatypes.c
objs = $(srcs:.c=.o)

LIBS =
CFLAGS = -g -Wall -std=c99

print-%:
	@echo '$*=$($*)'

all: $(program)

$(program): $(objs)
	g++ $(LDFLAGS) $(objs) $(LIBS) -o $(program)

clean:
	rm -f $(objs) $(program)

src/$(program).o: src/$(program).c
	gcc $(CFLAGS) -c $< -o $@

%.o : %.c %.h
	gcc $(CFLAGS) -c $< -o $@

