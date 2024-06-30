#define FDIREDCTORYUTILS 1

int copyDirectory(char *source, char *dest);
int copyFile(char *source, char *dest);
int deleteDirectory(char *path);
int directoryExists(char *path);
int renameDirectory(char *oldPath, char *newPath);