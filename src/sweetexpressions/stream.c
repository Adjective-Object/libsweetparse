#include <stdio.h>
#include <stdlib.h>

#include "stream.h"

stream * sopen_file(const char * path) {
    stream * s = malloc(sizeof(stream));
    s->file = fopen(path, "r");
    s->type = FROM_FILE;

    s->origin = NULL;
    s->current = NULL;
    s->buflen = 0;

    return s;
}

stream * sopen_mem(const char * corpus, size_t buffer_len) {
    stream * s = malloc(sizeof(stream));

    s->origin = corpus;
    s->current = corpus;
    s->buflen = buffer_len;

    s->file = NULL;

    return s;
}

void sclose(stream * s) {
    switch(s->type) {
        case FROM_FILE:
            fclose(s->file);
            break;
        case FROM_MEMORY:
            break;
        default:
            printf("unhandled stream type");
            exit(1);
    }
    free(s);
}

int sgetc(stream * s) {
    switch(s->type) {
        case FROM_FILE:
            return fgetc(s->file);
        case FROM_MEMORY:
            if ((unsigned long) (s->current - s->origin)
                    < s->buflen) {
                int c = (int) *(s->current);
                s->current++;
                return c;
            } else {
                return EOF;
            }
        default:
            printf("unhandled stream type");
            exit(1);
    }
}

int sseek(stream *s, long offset, int whence) {
    // file stuff
    if(s->type == FROM_FILE)
        return fseek(s->file, offset, whence);

    // memory stuff
    switch(whence) {
        case SEEK_SET:
            if (offset < 0) return -1;
            s->current = s->origin + offset;
            break;
        case SEEK_CUR:
            if (s->current + offset < s->origin ||
                    s->current + offset > s->origin + s->buflen)
                return -1;
            s->current += offset;
            break; 
        case SEEK_END:
            if (offset > 0) return -1;
            s->current = s->origin + s->buflen + offset;
            break;
    }

    return s->current - s->origin;
}


