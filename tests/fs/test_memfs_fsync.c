/*
 * Copyright 2015 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main() {
  int fd;

  // We first make sure the file doesn't currently exist.
  // We then write a file, call fsync, and close the file,
  // to make sure synchronous calls to resume does not throw.

  struct stat st;

  // a file whose contents are just 'az'
  if ((stat("/wakaka.txt", &st) != -1) || (errno != ENOENT))
    return -1000 - errno;

  fd = open("/wakaka.txt", O_RDWR | O_CREAT, 0666);
  assert(fd >= 0);
  if (fd == -1)
    return -2000 - errno;

  if (write(fd,"az",2) != 2)
    return -3000 - errno;

  if (fsync(fd) != 0)
    return -4000 - errno;

  if (close(fd) != 0)
    return -5000 - errno;

  return 0;
}
