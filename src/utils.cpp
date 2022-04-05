#include "utils.h"

extern "C" {
    // abgx360.c
}

int processdirectory(char *rootdir, char **filenames, bool *blockdevice, bool matchonly, int matchfiles, char **matchfilelist, char *s) {
#ifdef WIN32
    printf("CRITICAL: Must refactor this method in Python");
    return 1;
#else
    int i, len_ent;
    int len_root = strlen(rootdir) + 1;  // leave room for possible slash
    char *path = new char[len_root+1]; // char path[len_root+1];  // leave room for terminating null
    strcpy(path, rootdir);
    if (path[strlen(path)-1] != '/' && path[strlen(path)-1] != '\\') {
        #ifdef WIN32
            strcat(path, "\\");
        #else
            strcat(path, "/");
        #endif
    }
    DIR *dp;
    struct dirent *ep;
    dp = opendir(path);
    if (debug) printf("processing directory: %s%s", path, newline);
    if (dp != NULL) {
        while ((ep = readdir(dp))) {
            len_ent = strlen(ep->d_name);
            if ((s = (char *) malloc(len_root + len_ent + 1)) == NULL ) {
                color(red);
                printf("ERROR: memory allocation for a filename failed! Game over man... Game over!%s", newline);
                color(normal);
              exit(1);
            }
            if (filecount == MAX_FILENAMES) {
                color(red);
                printf("ERROR: More than %d matching files!%s", MAX_FILENAMES, newline);
                color(normal);
              return 1;
            }
            strcpy(s, path);
        	strcat(s, ep->d_name);
            if (stat(s, &buf) == -1 ) {
                color(yellow);
                printf("ERROR: stat failed for %s%s%s (%s)%s", quotation, s, quotation, strerror(errno), newline);
                color(normal);
              continue;
            }
            if (buf.st_mode & S_IFDIR) {  // directory
                if (recursesubdirs) {
                    if ( (len_ent == 1 && strcmp(ep->d_name, ".") == 0) ||
                         (len_ent == 2 && strcmp(ep->d_name, "..") == 0) ) continue;
                    if (processdirectory(s, filenames, blockdevice, matchonly, matchfiles, matchfilelist, s)) return 1;
                }
            }
            else if (buf.st_mode & (S_IFREG | S_IFBLK)) {  // regular files or block devices
                if (matchonly) {
                    for (i=0;i<matchfiles;i++) {
                        if (fnmatch(matchfilelist[i], ep->d_name, FNM_CASEFOLD) == 0) goto foundmatch;
                    }
                  continue;
                }
                foundmatch:
        	    filenames[filecount] = s;
        	    if (buf.st_mode & S_IFBLK) blockdevice[filecount] = true;
                if (debug) {
                    printf("%ld: %s", filecount, filenames[filecount]);
                    if (blockdevice[filecount]) printf(" (block device)%s", newline);
                    else printf("%s", newline);
                }
                filecount++;
            }
        }
        closedir(dp);
    }
    else {
        color(yellow);
        printf("ERROR: Couldn't open directory %s%s%s (%s)%s", quotation, path, quotation, strerror(errno), newline);
        color(normal);
    }
  return 0;
#endif
}
