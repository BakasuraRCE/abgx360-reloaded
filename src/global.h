#pragma once

#include <stdbool.h>   // true/false macro for bools

#include "sha1.h"


#ifdef WIN32
    #include <Windows.h>
    #include <ntddstor.h> // device i/o stuff
    #include <ntddscsi.h> // SCSI_PASS_THROUGH_DIRECT

    extern char winbuffer[2048];
    extern HANDLE hDevice = INVALID_HANDLE_VALUE;
    #define mkdir(a,b) _mkdir(a)
    //#define strcasecmp(a,b) _stricmp(a,b)
    //#define strncasecmp(a,b,c) _strnicmp(a,b,c)
    //#define fseeko(a,b,c) myfseeko64(a,b,c)
    //#define ftello(a) ftello64(a)
    #define DATA_NONE SCSI_IOCTL_DATA_UNSPECIFIED
    #define DATA_IN   SCSI_IOCTL_DATA_IN
    #define DATA_OUT  SCSI_IOCTL_DATA_OUT
    #define LL "I64"
#endif

#define BIGBUF_SIZE 32768  // 32 KB, changing this could cause some problems

#define NUM_CURRENTPFIENTRIES   15  // update this when adding new pfi entries
#define NUM_CURRENTVIDEOENTRIES 21 // update this when adding new video entries

#define NUM_CURRENTAP25MEDIAIDS       25  // update this when adding new AP25 media ids for discs that have no AP25 flag in the default.xex
#define NUM_CURRENTAP25TITLEIDS        2  // update this when adding new AP25 title ids for discs that have no AP25 flag in the default.xex
#define NUM_CURRENTAP25DISCPROFILEIDS 11  // update this when adding new AP25 disc profile ids
//#define NUM_CURRENTDAEVERSIONS        5   // update this when adding new DAE versions and descriptions

#define TOPOLOGY_SIZE 26624

// update version values here
extern const char *headerversion = "v1.0.7";
extern const char *curluseragent = "abgx360 v1.0.7 (\"ABGX360_OS\")";
extern unsigned long currentversion = 0x010007L;  // MSB (1.2.3 = 0x010203)

// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long latestversion = 0L;

// update this value before release (unrecognized pfi/video on a game authored before this date is labeled almost
// certainly corrupt, otherwise it might be a brand new wave)
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long long lastknownwave = 0x01CC6C27EBA6C000LL;  // 2011-09-06

// update this value if additional stealth sectors are needed in our ISOs so that older versions
// of abgx360 will not blank them out if fixing video padding is enabled (works for XGD2 or XGD3)
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long number_of_stealth_sectors = 16L;  // increase if needed, never decrease (13 sectors for topology data plus 1 sector each for PFI, DMI, SS)

// update this value if stealth sectors or data ever needs to be placed in the 16 sectors between an XGD3 SS and the game partition
// xgd3_stealth_padding_bitfield is 16 bits, one for each of the 16 sectors
// a bit value of 1 indicates a padding sector, 0 indicates a data sector
// Example: 0x7EFE = 0111111011111110 means that only sectors 2-7 and 9-15 will be checked for blank padding if check/fix padding is enabled
// all bits are currently set to 1 (1111111111111111 = 0xFFFF) which means that all 16 sectors will be checked for blank padding
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned short xgd3_stealth_padding_bitfield = 0xFFFF;

// local directories
extern char homedir[2048];
#ifdef WIN32
    extern const char *abgxdir =        "\\abgx360\\";
    extern const char *stealthdir =     "StealthFiles\\";
    extern const char *userstealthdir = "UserStealthFiles\\";
    extern const char *imagedir =       "Images\\";
#else
    extern const char *abgxdir =        "/.abgx360/";
    extern const char *stealthdir =     "StealthFiles/";
    extern const char *userstealthdir = "UserStealthFiles/";
    extern const char *imagedir =       "Images/";
#endif

// load replacements from abgx360.ini if it exists (make sure to update checkini() if these addresses are changed)
extern const char *webinidir =            "http://abgx360.hadzz.com/verified/";                       // dir that contains verified ini files
extern const char *webunverifiedinidir =  "http://abgx360.hadzz.com/unverified/";                     // dir that contains unverified ini files
extern const char *webcsv =               "http://abgx360.hadzz.com/GameNameLookup.csv";              // http path to GameNameLookup.csv
extern const char *webdat =               "http://abgx360.hadzz.com/abgx360.dat";                     // http path to abgx360.dat
extern const char *webtopology =          "http://abgx360.hadzz.com/topology.php";                    // http path to topology.php
extern const char *webstealthdir =        "http://abgx360.hadzz.com/StealthFiles/";                   // dir that contains SS/DMI/PFI/Video stealth files
extern const char *autouploadwebaddress = "http://abgx360.hadzz.com/Control/AutoUpload.php";          // form for submitting AutoUploads

//char *ap25autouploadwebaddress = "http://abgx360.net/Apps/Control/AP25AutoUpload.php";  // form for submitting AP25 AutoUploads
//char *webdae = "http://abgx360.net/Apps/Stealth360/dae.bin";                            // http path to dae.bin

extern struct waveentry {unsigned long crc; uchar sha1[20]; char *description; bool hosted;} waveentry;
extern struct waveentry currentpfientries[NUM_CURRENTPFIENTRIES];
extern struct waveentry *mostrecentpfientries;
extern struct waveentry *datfilepfientries;
extern unsigned long num_pfientries;
extern struct waveentry currentvideoentries[NUM_CURRENTVIDEOENTRIES];
extern struct waveentry *mostrecentvideoentries;
extern struct waveentry *datfilevideoentries;
extern unsigned long num_videoentries;
extern unsigned long xbox1pficrc;
extern uchar xbox1pfisha1[20];
extern unsigned long xbox1videocrc;
extern uchar xbox1videosha1[20];

extern struct mediaidshort {unsigned char mediaid[4];} mediaidshort;
extern struct mediaidshort currentap25mediaids[NUM_CURRENTAP25MEDIAIDS];
extern struct mediaidshort *mostrecentap25mediaids;
extern struct mediaidshort *datfileap25mediaids;
extern unsigned long num_ap25mediaids;

extern struct mediaidshort currentap25titleids[NUM_CURRENTAP25TITLEIDS];
extern struct mediaidshort *mostrecentap25titleids;
extern struct mediaidshort *datfileap25titleids;
extern unsigned long num_ap25titleids;

extern struct mediaidshort currentap25discprofileids[NUM_CURRENTAP25DISCPROFILEIDS];
extern struct mediaidshort *mostrecentap25discprofileids;
extern struct mediaidshort *datfileap25discprofileids;
extern unsigned long num_ap25discprofileids;

//struct flagverdescription {unsigned char flags; unsigned short version; char *description;};
//struct flagverdescription currentdaeversions[NUM_CURRENTDAEVERSIONS];
//struct flagverdescription *mostrecentdaeversions;
//struct flagverdescription *datfiledaeversions;
//unsigned long num_daeversions;

extern int mediumangledev_value = 3, highangledev_value = 9, fixangledev_value = 3;

extern bool verbose = true, stealthcheck = true, autofix = true, autofixuncertain = true, verify = true, onlineupdate = true, csvupdate = false;
extern bool checkdvdfile = true, checkpadding = false, padL0 = false, fixdeviation = true, fixDRT = true, increasescreenbuffersize = true;
extern bool autofixalways = false, autoupload = false, keeporiginaliso = false, dontparsefs = false;
extern bool extraverbose = false, debug = false, debugfs = false;
extern bool noheader = false, justheader = false, justfooter = false;
extern bool minimal = false, html = false, stripcolors = false, script = false, justhelp = false;
extern bool terminal = false, stayoffline = false, showsstable = false;
extern bool pauseshell = false, maximize = false, showfiles = false;
extern bool fixangle359 = false, showfulltable = false;
extern bool homeless = false, makedatfile = false;
extern bool patchvalidfilesonly = true, patchifstealthpasses = false, manualpatch = false, manualextract = false;
extern bool rebuildlowspace = false, norebuild = false, truncatefile = false, checkcorruption = false, foldermode = false;
extern bool matchonly = false, testing = false, testingdvd = false;
extern bool localonly = false, recursesubdirs = false, clobber = false;
extern bool showachievements = false, hidesecretachievements = false, showavatarawards = false, unicode = false, imagedirmissing = false;
extern bool skiplayerboundaryinfo = false, devkey = false, trustssv2angles = true, useinstalldir = false;
extern struct badshit {unsigned char c[21], d[21], data[21]; int count; char* explanation;} badshit;
extern char unrecognizedRTarray[21];
// don't forget to add new args to the list before stat()
extern int truncatearg = 0, userregionarg = 0, folderarg = 0, matcharg = 0, specialarg = 0, readretryarg = 0;
extern int patchvideoarg = 0, patchpfiarg = 0, patchdmiarg = 0, patchssarg = 0;
extern int extractvideoarg = 0, extractpfiarg = 0, extractdmiarg = 0, extractssarg = 0;
extern int autouploaduserarg = 0, autouploadpassarg = 0, fixangledevarg = 0, connectiontimeoutarg = 0, dvdtimeoutarg = 0;
extern int dvdarg = 0, userlangarg = 0, origarg = 0, speedarg = 0;
//int riparg = 0, ripdestarg = 0;
extern long connectiontimeout = 20, dvdtimeout = 20, userlang = 0;
extern float speed = 0.0;
extern unsigned long curlprogressstartmsecs, userregion = 0L;

extern char buffer[2048], buffer2[2048];
extern char inifilename[24], xexinifilename[17], gamename[151];
extern char sense[20], specialerror[200];
extern char installdirvideofilepath[2048] = {0};
extern unsigned char ubuffer[2048], ss[2048], fixed_ss[2048], cdb[12];
extern unsigned char bigbuffer[BIGBUF_SIZE];
extern unsigned long filecount = 0L;
extern unsigned long seek;

extern const char *green = "\033[1;32;40m", *yellow = "\033[1;33;40m", *red = "\033[1;31;40m", *cyan = "\033[1;36;40m", *blue = "\033[1;34;40m";
extern const char *darkblue = "\033[0;34;40m", *white = "\033[1;37;40m", *arrow = "\033[1;34;40m", *box = "\033[1;34;40m", *normal = "\033[0;37;40m";
extern const char *wtfhexcolor = "\033[1;31;40m", *wtfcharcolor = "\033[1;37;41m", *reset = "\033[0m", *brown = "\033[0;33;40m", *filename = "\033[0;37;44m";
extern const char *blackonyellow = "\033[0;30;43m", *blackonred = "\033[0;30;41m";
#ifdef __APPLE__
    extern const char *hexoffsetcolor = "\033[0;37;40m", *darkgray = "\033[0;37;40m";  // can't do dark gray apparently (shows completely black) so just use normal gray
#else
    extern const char *hexoffsetcolor = "\033[1;30;40m", *darkgray = "\033[1;30;40m";
#endif
extern const char *newline = "\n", *quotation = "\"", *ampersand = "&", *lessthan = "<", *greaterthan = ">", *numbersign = "#";
extern const char *sp0 = "\0", *sp1 = " ", *sp2 = "  ", *sp3 = "   ", *sp4 = "    ", *sp5 = "     ";
extern const char *sp6 = "      ", *sp7 = "       ", *sp8 = "        ", *sp9 = "         ";
extern const char *sp10 = "          ", *sp11 = "           ", *sp12 = "            ", *sp18 = "                  ";
extern const char *sp20 = "                    ", *sp21 = "                     ", *sp28 = "                            ";


extern unsigned long fix_ss_crc32;
extern long long fpfilesize;
extern int isofilearg, readretries = 20, charsprinted;
extern unsigned long readerrorstotal, readerrorsrecovered, writeerrorstotal, writeerrorsrecovered;
extern int readerrorcharsprinted;
extern int returnvalue;
extern long longreturnvalue;
extern unsigned long sizeoverbuffer, bufferremainder;

extern char *isofilename = NULL;