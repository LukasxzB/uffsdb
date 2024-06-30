#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int copyFile(char *src, char *dest)
{
  int src_fd, dest_fd;
  char buffer[4096];
  ssize_t bytes;

  src_fd = open(src, O_RDONLY);
  if (src_fd == -1)
    return 0;

  dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dest_fd == -1)
  {
    close(src_fd);
    return 0;
  }

  while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0)
    if (write(dest_fd, buffer, bytes) != bytes)
    {
      close(src_fd);
      close(dest_fd);
      return 0;
    }

  close(src_fd);
  close(dest_fd);
  return 1;
}

int copyDirectory(char *src, char *dest)
{
  struct dirent *entry;
  DIR *dp;
  char src_path[1024], dest_path[1024];

  dp = opendir(src);
  if (dp == NULL)
    return 0;

  if (mkdir(dest, 0755) == -1)
  {
    closedir(dp);
    return 0;
  }

  while ((entry = readdir(dp)))
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
    snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

    struct stat statbuf;
    if (stat(src_path, &statbuf) == -1)
      continue;

    if (S_ISDIR(statbuf.st_mode))
    {
      int res = copyDirectory(src_path, dest_path);
      if (res == 0)
        return 0;

      continue;
    }

    int res = copyFile(src_path, dest_path);
    if (res == 0)
      return 0;
  }

  closedir(dp);
  return 1;
}

int directoryExists(char *path)
{
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;

  return S_ISDIR(statbuf.st_mode);
}

int deleteDirectory(char *path)
{
  struct dirent *entry;
  DIR *dp;
  char pathBuff[1024];

  dp = opendir(path);
  if (dp == NULL)
    return 0;

  while ((entry = readdir(dp)))
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    snprintf(pathBuff, sizeof(pathBuff), "%s/%s", path, entry->d_name);

    struct stat statbuf;
    if (stat(pathBuff, &statbuf) == -1)
      continue;

    if (S_ISDIR(statbuf.st_mode))
    {
      deleteDirectory(pathBuff);
    }
    else if (unlink(pathBuff) == -1)
    {
      return 0;
    }
  }

  closedir(dp);

  if (rmdir(path) == -1)
    return 0;

  return 1;
}

int renameDirectory(char *oldPath, char *newPath)
{
  if (rename(oldPath, newPath) != 0)
    return 0;

  return 1;
}