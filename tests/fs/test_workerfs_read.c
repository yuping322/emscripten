/*
 * Copyright 2015 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define SECRET_LEN 10

int main() {
  int fd;
  int fd2;
  struct stat st;
  struct stat st2;
  char buf[100];
  char secret2[] = SECRET2;
  int len2 = SECRET_LEN / 2;

  if (stat("/work/notexist.txt", &st) != -1 || errno != ENOENT) {
    assert(false);
  }

  if (stat("/work/blob.txt", &st) != 0) {
    assert(false);
  }

  fd = open("/work/blob.txt", O_RDWR, 0666);
  if (fd == -1) {
    assert(false);
  }

  if (read(fd, buf, 1000) != SECRET_LEN ||
      strncmp(buf, SECRET, SECRET_LEN) != 0) {
    assert(false);
  }

  fd2 = open("/work/file.txt", O_RDONLY, 0666);
  assert(fd2 != -1);

  if (lseek(fd2, len2, SEEK_SET) != len2) {
    assert(false);
  }

  if (read(fd2, buf, len2) != len2 ||
      strncmp(buf, secret2 + len2, len2) != 0) {
    assert(false);
  }

  stat("/work/file.txt", &st);
  chmod("/work/file.txt", 0640);
  stat("/work/file.txt", &st2);
  if (st.st_mode != (0777 | S_IFREG) || st2.st_mode != (0640 | S_IFREG)) {
    assert(false);
  }

  DIR *pDir = opendir("/work/");
  assert(pDir);

  int blobFileExists = 0;
  int fileTxtExists = 0;
  struct dirent *pDirent;
  while ((pDirent = readdir(pDir)) != NULL) {
    if (strcmp(pDirent->d_name, "blob.txt") == 0) {
       blobFileExists = 1;
    }
    if (strcmp(pDirent->d_name, "file.txt") == 0) {
       fileTxtExists = 1;
    }

  }

  assert(blobFileExists);
  assert(fileTxtExists);
  return 0;
}
