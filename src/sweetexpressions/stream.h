#ifndef SWEETEXPR_STREAM
#define SWEETEXPR_STREAM

#include <stdio.h>

typedef enum STREAM_TYPE { __SWEXP_FROM_FILE, __SWEXP_FROM_MEMORY } STREAM_TYPE;

typedef struct stream {
  STREAM_TYPE type;
  FILE *file;
  const char *origin;
  const char *current;
  size_t buflen;
} stream;

/**
 * Allocates and returns a new stream object for reading from a data
 * source.
 *
 * target: either a pointer to a string that will be consumerd or
 *      a path to a file, depending on the value of `type`
 * type: the type of the stream.
 *      If it is __SWEXP_FROM_FILE, then `target` will be the path to a file.
 *      If it is __SWEXP_FROM_MEMORYm then `target` will be the string served.
 **/
stream *sopen_file(const char *path);
stream *sopen_mem(const char *corpus, size_t buffer_len);

/**
 * cleans up the internals of a stream
 *
 * s: the stream to be cleaned up and deallocated
 **/
void sclose(stream *s);

/**
 * Get a character from a stream
 * Returns the value of the character that was fetched, or
 * returns <EOF> on error.
 **/
int sgetc(stream *s);

/**
 * Sets the position indicator in the stream. The new position
 * is obtained by adding `offset` bytes to the position specified
 * by `whence`.
 *
 * whence = SEEK_SET sets the position with relation to origin
 * whence = SEEK_CUR sets the position relative to the current pos *
 * whence = SEEK_END sets the position to the end of the file
 **/
int sseek(stream *s, long offset, int whence);

#endif
