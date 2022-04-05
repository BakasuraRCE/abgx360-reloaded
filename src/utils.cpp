#include "utils.h"
#include "global.h"

extern "C" {
    // abgx360.c
    void color(const char *color);
    void WinError(const char *action, const char *textcolor);
    int sendcdb(UCHAR direction, unsigned char *dataBuffer, unsigned long dataBufferSize, unsigned char *cdb, int cdbLen, bool checkreturnlength);
    char* cdberror(char *sense);
    void hexdump(unsigned char* ptr, int stealthtype, int bytes);
    bool isanxbox360rippingdrive(char *drivename, bool ap25supportneeded);
}

int opendeviceandgetname(char *drive, bool requestexclusiveaccess, bool errorsareok) {
    int i, j, n;
    char devicenamebuffer[64] = {0};
    #ifdef WIN32
        // drive should have already been validated as a drive letter A-Z (or a-z)
        char *stringforwinerror = new char[26 + strlen(drive)]; // char stringforwinerror[26 + strlen(drive)];
        char *rootpath = new char[strlen(drive) + 3]; // char rootpath[strlen(drive) + 3];
        char *pathforcreatefile = new char[strlen(drive) + 6]; // char pathforcreatefile[strlen(drive) + 6];
        sprintf(rootpath, "%s:\\", drive);
        sprintf(pathforcreatefile, "\\\\.\\%s:", drive);
        const char *drivetypelist[7] = {
            "The drive type cannot be determined.",
            "The root path is invalid; for example, there is no volume mounted at the path.",
            "The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader.",
            "The drive has fixed media; for example, a hard drive or flash drive.",
            "The drive is a remote (network) drive.",
            "The drive is a CD-ROM drive.",
            "The drive is a RAM disk."
        };
        // check the drive type
        unsigned int drivetype = GetDriveType(rootpath);
        if (drivetype != DRIVE_CDROM) {
            if (errorsareok) color(yellow);
            else color(red);
            printf("ERROR: %s is not recognized as a multimedia drive!", rootpath);
            if (drivetype < 7 && drivetype != 5) printf(" (%s)", drivetypelist[drivetype]);
            printf("%s", newline);
            color(normal);
            if (debug) printf("GetDriveType(%s) = %d%s", rootpath, GetDriveType(rootpath), newline);
          return 1;
        }
        // open the device
        if (requestexclusiveaccess)
             hDevice = CreateFile(pathforcreatefile,
                                  GENERIC_READ | GENERIC_WRITE,
                                  0,
                                  NULL, OPEN_EXISTING, 0, NULL);
        else hDevice = CreateFile(pathforcreatefile,
                                  GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  NULL, OPEN_EXISTING, 0, NULL);
        if (hDevice == INVALID_HANDLE_VALUE) {
            sprintf(stringforwinerror, "CreateFile for drive (%s:)", drive);
            if (errorsareok) WinError(stringforwinerror, yellow);
            else WinError(stringforwinerror, red);
            if (debug) printf("drive: %s, rootpath: %s, pathforcreatefile: %s%s", drive, rootpath, pathforcreatefile, newline);
          return 1;
        }
    #else
        // open the device
        #if defined(__FreeBSD__)
            cam_dev = cam_open_device(drive, O_RDWR);
            if (cam_dev != NULL && cam_dev->fd != -1) fd = cam_dev->fd;
            else {
                if (errorsareok) color(yellow);
                else color(red);
                if (debug) {
                    if (cam_dev == NULL) printf("cam_dev == NULL%s", newline);
                    else if (cam_dev->fd == -1) printf("cam_dev->fd == -1%s", newline);
                }
                printf("ERROR: Failed to open device (O_RDWR): %s (%s)%s", drive, strerror(errno), newline);
                color(normal);
              return 1;
            }
            if (requestexclusiveaccess) {
                // todo: make sure this is valid and works on FreeBSD
                struct flock fl;
                fl.l_type   = F_WRLCK;  // type of lock: F_RDLCK, F_WRLCK, F_UNLCK
                fl.l_whence = SEEK_SET; // how to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END
                fl.l_start  = 0;        // starting offset for lock from l_whence
                fl.l_len    = 0;        // number of bytes to lock, 0 = to EOF
                fl.l_pid    = getpid(); // lock owner
                if (fcntl(fd, F_SETLK, &fl) == -1) {
                    if (errorsareok) color(yellow);
                    else color(red);
                    printf("ERROR: Failed to open device for exclusive access: %s (%s)%s", drive, strerror(errno), newline);
                    color(normal);
                    close(fd);
                  return 1;
                }
            }
        #else
            if (requestexclusiveaccess) {
                // don't think this is actually exclusive but Ubuntu 8.10 VM (didn't try any others) will not let me open using O_RDWR on /dev/cdrom
                // (errno says read-only filesystem) so i can't use F_WRLCK (exclusive) on a file that is not open for writing
                struct flock fl;
                fl.l_type   = F_RDLCK;  // type of lock: F_RDLCK, F_WRLCK, F_UNLCK
                fl.l_whence = SEEK_SET; // how to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END
                fl.l_start  = 0;        // starting offset for lock from l_whence
                fl.l_len    = 0;        // number of bytes to lock, 0 = to EOF
                fl.l_pid    = getpid(); // lock owner
                fd = open(drive, O_RDONLY);
                if (fd == -1) {
                    if (errorsareok) color(yellow);
                    else color(red);
                    printf("ERROR: Failed to open device: %s (%s)%s", drive, strerror(errno), newline);
                    color(normal);
                  return 1;
                }
                if (fcntl(fd, F_SETLK, &fl) == -1) {
                    if (errorsareok) color(yellow);
                    else color(red);
                    printf("ERROR: Failed to open device for exclusive access: %s (%s)%s", drive, strerror(errno), newline);
                    color(normal);
                    close(fd);
                  return 1;
                }
            }
            else {
                fd = open(drive, O_RDONLY);
                if (fd == -1) {
                    if (errorsareok) color(yellow);
                    else color(red);
                    printf("ERROR: Failed to open device: %s (%s)%s", drive, strerror(errno), newline);
                    color(normal);
                  return 1;
                }
            }
        #endif
    #endif
    
    // get the device name
    unsigned char inqbuffer[96] = {0};
    memset(cdb, 0, 12);
    cdb[0] = 0x12;  // INQUIRY command
    cdb[4] = 0x60;  // allocation length LSB (0x0060 = 96) - maximum number of bytes that may be returned by the drive
    
    if (sendcdb(DATA_IN, inqbuffer, 96, cdb, 12, false)) {  // false means it won't check that number of bytes returned == 96
        for (i=0;i<readretries;i++) {
            if (sendcdb(DATA_IN, inqbuffer, 96, cdb, 12, false) == 0) {
                // recovered error
                goto inquiryrecovered;
            }
        }
        // unrecovered error
        color(yellow);
        #ifdef WIN32
            printf("Failed to get device parameters from drive %s: (%s)%s", drive, cdberror(sense), newline);
        #else
            printf("Failed to get device parameters from %s (%s)%s", drive, cdberror(sense), newline);
        #endif
        color(normal);
    }
    else {
        inquiryrecovered:
        if (debug || testingdvd) {
            printf("inquiry buffer:%s", newline);
            hexdump(inqbuffer, 0, 96);
        }
        #ifndef WIN32
            // check the device type
            unsigned char peripheraldevicetype = inqbuffer[0] & 0x1F;
            if (peripheraldevicetype != 0x05) {
                if (errorsareok) color(yellow);
                else color(red);
                printf("ERROR: %s is not recognized as a multimedia device!", drive);
                if      (peripheraldevicetype == 0x00) printf(" (It's a direct-access block device; for example, magnetic disk.)");
                else if (peripheraldevicetype == 0x01) printf(" (It's a sequential-access device; for example, magnetic tape.)");
                else if (peripheraldevicetype == 0x02) printf(" (It's a printer device.)");
                else if (peripheraldevicetype == 0x03) printf(" (It's a processor device.)");
                else if (peripheraldevicetype == 0x04) printf(" (It's a write-once device; for example, some optical disks.)");
                else if (peripheraldevicetype == 0x06) printf(" (It's a scanner device.)");
                else if (peripheraldevicetype == 0x07) printf(" (It's an optical memory device; for example, some optical disks.)");
                else if (peripheraldevicetype == 0x08) printf(" (It's a medium changer device; for example, a jukebox.)");
                else if (peripheraldevicetype == 0x09) printf(" (It's a communications device.)");
                else if (peripheraldevicetype == 0x0A || peripheraldevicetype == 0x0B) printf(" (It's an obsolete device type.)");
                else if (peripheraldevicetype == 0x0C) printf(" (It's a storage array controller device; for example, RAID.)");
                else if (peripheraldevicetype == 0x0D) printf(" (It's an enclosure services device.)");
                else if (peripheraldevicetype == 0x0E) printf(" (It's a simplified direct-access device; for example, magnetic disk.)");
                else if (peripheraldevicetype == 0x0F) printf(" (It's an optical card reader/writer device.)");
                else if (peripheraldevicetype == 0x10) printf(" (It's a bridge controller.)");
                else if (peripheraldevicetype == 0x11) printf(" (It's an object-based storage device.)");
                else if (peripheraldevicetype == 0x12) printf(" (It's an automation/drive interface.)");
                else if (peripheraldevicetype >= 0x13 && peripheraldevicetype <= 0x1E) printf(" (It's a reserved device type.)");
                else if (peripheraldevicetype == 0x1F) printf(" (It's an unknown device type.)");
                printf("%s", newline);
                color(normal);
              return 1;
            }
        #endif
        i = 0;
        // vendor id 8-15
        for (j=8;j<16;j++) {
            if (inqbuffer[j] > 0x1F) {
                devicenamebuffer[i] = inqbuffer[j];
                i++;
            }
        }
        if (i > 0) for (n=i-1;n>=0;n--) {
            if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
            else break;
        }
        devicenamebuffer[i] = 0x20;
        i++;
        // product id 16-31
        for (j=16;j<32;j++) {
            if (inqbuffer[j] > 0x1F) {
                devicenamebuffer[i] = inqbuffer[j];
                i++;
            }
        }
        if (i > 0) for (n=i-1;n>=0;n--) {
            if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
            else break;
        }
        devicenamebuffer[i] = 0x20;
        i++;
        // revision level 32-35
        for (j=32;j<36;j++) {
            if (inqbuffer[j] > 0x1F) {
                devicenamebuffer[i] = inqbuffer[j];
                i++;
            }
        }
        if (i > 0) for (n=i-1;n>=0;n--) {
            if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
            else break;
        }
        devicenamebuffer[i] = 0x20;
        i++;
        // vendor specific additional info 36-55
        if (isanxbox360rippingdrive(devicenamebuffer, false)) {
            if (memcmp(devicenamebuffer, "PLDS DG-16D2S ", 14) == 0) {
                // get more data for fw and hardware version (i.e. PLDS DG-16D2S 74850C A0A1)
                i--;
                for (j=36;j<42;j++) {
                    if (j == 38) {
                        devicenamebuffer[i] = 0x20;
                        i++;
                    }
                    if (inqbuffer[j] > 0x1F) {
                        devicenamebuffer[i] = inqbuffer[j];
                        i++;
                    }
                }
                if (i > 0) for (n=i-1;n>=0;n--) {
                    if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
                    else break;
                }
                devicenamebuffer[i] = 0x20;
                i++;
            }
            else if (memcmp(devicenamebuffer, "SATA DVD-ROM 6243 ", 18) == 0) {
                // get more data for fw version (i.e. SATA DVD-ROM 62430C)
                i--;
                for (j=36;j<40;j++) {
                    if (inqbuffer[j] > 0x1F) {
                        devicenamebuffer[i] = inqbuffer[j];
                        i++;
                    }
                }
                if (i > 0) for (n=i-1;n>=0;n--) {
                    if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
                    else break;
                }
                devicenamebuffer[i] = 0x20;
                i++;
            }
            // try to get ixtreme version info
            unsigned char ixbuffer[0x30] = {0};
            memset(cdb, 0, 12);
            cdb[0] = 0x12;
            cdb[4] = 0x24;
            cdb[9] = 0x49;
            cdb[10] = 0x58;
            cdb[11] = 0x01;
            if (sendcdb(DATA_IN, ixbuffer, 0x30, cdb, 12, false)) {  // false means it won't check that number of bytes returned == 96
                for (i=0;i<readretries;i++) {
                    if (sendcdb(DATA_IN, ixbuffer, 0x30, cdb, 12, false) == 0) {
                        // recovered error
                        goto ixrecovered;
                    }
                }
                // unrecovered error
                if (debug || testingdvd) {
                    #ifdef WIN32
                        printf("DG-16D2S/VAD6038/TS-H943A failed to do ix01 cmd for drive %s: (%s)%s", drive, cdberror(sense), newline);
                    #else
                        printf("DG-16D2S/VAD6038/TS-H943A failed to do ix01 cmd for %s (%s)%s", drive, cdberror(sense), newline);
                    #endif
                }
            }
            else {
                ixrecovered:
                if (debug || testingdvd) {
                    printf("ix buffer:%s", newline);
                    hexdump(ixbuffer, 0, 0x30);
                }
                if (html) {
                    devicenamebuffer[i]   = '&';
                    devicenamebuffer[i+1] = 'l';
                    devicenamebuffer[i+2] = 't';
                    devicenamebuffer[i+3] = ';';
                    i += 4;
                }
                else {
                    devicenamebuffer[i] = '<';
                    i++;
                }
                for (j=16;j<48;j++) {
                    if (ixbuffer[j] > 0x1F) {
                        devicenamebuffer[i] = ixbuffer[j];
                        i++;
                    }
                }
                if (i > 0) for (n=i-1;n>=0;n--) {
                    if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
                    else break;
                }
                if (html) {
                    devicenamebuffer[i]   = '&';
                    devicenamebuffer[i+1] = 'g';
                    devicenamebuffer[i+2] = 't';
                    devicenamebuffer[i+3] = ';';
                    i += 4;
                }
                else {
                    devicenamebuffer[i] = '>';
                    i++;
                }
            }
        }
        else {
            if (html) {
                devicenamebuffer[i]   = '&';
                devicenamebuffer[i+1] = 'l';
                devicenamebuffer[i+2] = 't';
                devicenamebuffer[i+3] = ';';
                i += 4;
            }
            else {
                devicenamebuffer[i] = '<';
                i++;
            }
            for (j=36;j<56;j++) {
                if (inqbuffer[j] > 0x1F) {
                    devicenamebuffer[i] = inqbuffer[j];
                    i++;
                }
            }
            if (i > 0) for (n=i-1;n>=0;n--) {
                if (devicenamebuffer[n] == 0x20) i--;  // backspace over trailing spaces
                else break;
            }
            if (html) {
                devicenamebuffer[i]   = '&';
                devicenamebuffer[i+1] = 'g';
                devicenamebuffer[i+2] = 't';
                devicenamebuffer[i+3] = ';';
                i += 4;
            }
            else {
                devicenamebuffer[i] = '>';
                i++;
            }
        }
        
        #ifdef WIN32
            // get bus type
            ULONG returnedbytes;
            STORAGE_PROPERTY_QUERY query;
            PSTORAGE_ADAPTER_DESCRIPTOR adapterDesc;
            UCHAR outBuf[1024];
            memset(outBuf, 0, 1024);
            query.PropertyId = StorageAdapterProperty;
            query.QueryType = PropertyStandardQuery;
            if (DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query,
                                sizeof(STORAGE_PROPERTY_QUERY), &outBuf, 1024, &returnedbytes, NULL)) {
                adapterDesc = (PSTORAGE_ADAPTER_DESCRIPTOR) outBuf;
                devicenamebuffer[i] = 0x20;
                devicenamebuffer[i+1] = '[';
                i += 2;
                switch((UCHAR) adapterDesc->BusType) {
                    case 0x01: memcpy(devicenamebuffer+i, "SCSI", 4);         i += 4;  break;
                    case 0x02: memcpy(devicenamebuffer+i, "ATAPI", 5);        i += 5;  break;
                    case 0x03: memcpy(devicenamebuffer+i, "ATA", 3);          i += 3;  break;
                    case 0x04: memcpy(devicenamebuffer+i, "IEEE 1394", 9);    i += 9;  break;
                    case 0x05: memcpy(devicenamebuffer+i, "SSA", 3);          i += 3;  break;
                    case 0x06: memcpy(devicenamebuffer+i, "Fibre", 5);        i += 5;  break;
                    case 0x07: memcpy(devicenamebuffer+i, "USB", 3);          i += 3;  break;
                    case 0x08: memcpy(devicenamebuffer+i, "RAID", 4);         i += 4;  break;
                    case 0x09: memcpy(devicenamebuffer+i, "iSCSI", 5);        i += 5;  break;
                    case 0x0A: memcpy(devicenamebuffer+i, "SAS", 3);          i += 3;  break;
                    case 0x0B: memcpy(devicenamebuffer+i, "SATA", 4);         i += 4;  break;
                    case 0x0C: memcpy(devicenamebuffer+i, "SD", 2);           i += 2;  break;
                    case 0x0D: memcpy(devicenamebuffer+i, "MMC", 3);          i += 3;  break;
                    case 0x0E: memcpy(devicenamebuffer+i, "Virtual", 7);      i += 7;  break;
                    case 0x0F: memcpy(devicenamebuffer+i, "FB Virtual", 10);  i += 10; break;
                    default:   memcpy(devicenamebuffer+i, "Unknown Bus", 11); i += 11; break;
                }
                devicenamebuffer[i] = ']';
                i++;
            }
        #endif
        
        devicenamebuffer[i] = 0x00;  // terminating null
    }
    
    if (strlen(devicenamebuffer)) {
        isofilename = (char*)calloc(4 + strlen(devicenamebuffer) + strlen(drive) + 1, sizeof(char));
        if (isofilename == NULL) {
            color(red);
            printf("ERROR: Memory allocation for device name failed! Game over man... Game over!%s", newline);
            color(normal);
          exit(1);
        }
        #ifdef WIN32
            sprintf(isofilename, "%s (%s:)", devicenamebuffer, drive);
        #else
            sprintf(isofilename, "%s (%s)", devicenamebuffer, drive);
        #endif
    }
    else {
        isofilename = (char*)calloc(7 + strlen(drive) + 1, sizeof(char));
        if (isofilename == NULL) {
            color(red);
            printf("ERROR: Memory allocation for device name failed! Game over man... Game over!%s", newline);
            color(normal);
          exit(1);
        }
        #ifdef WIN32
            sprintf(isofilename, "Drive %s:", drive);
        #else
            sprintf(isofilename, "%s", drive);
        #endif
    }
  return 0;
}

int processdirectory(char *rootdir, char **filenames, bool *blockdevice, bool matchonly, int matchfiles, char **matchfilelist, char *s) {
    printf("CRITICAL: Must refactor this methodi n Python");
    return 1;
    /*
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
  */
}
