#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>   // true/false macro for bools
#include <stddef.h>    // for offsetof

#if defined(_WIN32) || defined(__CLION_IDE__)
#include <Windows.h>
#include <ntddstor.h> // device i/o stuff
#include <ntddscsi.h> // SCSI_PASS_THROUGH_DIRECT

extern char winbuffer[2048];
extern HANDLE hDevice;
#define mkdir(a, b) _mkdir(a)
//#define strcasecmp(a,b) _stricmp(a,b)
//#define strncasecmp(a,b,c) _strnicmp(a,b,c)
//#define fseeko(a,b,c) myfseeko64(a,b,c)
//#define ftello(a) ftello64(a)
#define DATA_NONE SCSI_IOCTL_DATA_UNSPECIFIED
#define DATA_IN   SCSI_IOCTL_DATA_IN
#define DATA_OUT  SCSI_IOCTL_DATA_OUT
#define LL "I64"
#endif

#define PFI_HEX 1
#define DMI_HEX 2

#define WEB_INIDIR           1
#define WEB_CSV              2
#define WEB_DAT              3
#define WEB_TOPOLOGY         4
#define WEB_STEALTHDIR       5
#define WEB_AUTOUPLOAD       6
#define WEB_UNVERIFIEDINIDIR 7
//#define WEB_AP25AUTOUPLOAD   8
//#define WEB_DAE              9


#define XEX_INI                  1
#define SSXEX_INI                2
#define SSXEX_INI_FROM_XEX_INI   3
#define UNVERIFIED_INI           4
#define SS_FILE                  5
#define SS_FILE_OK_IF_MISSING    6
#define STEALTH_FILE             7
#define GIANT_VIDEO_FILE         8
#define SMALL_VIDEO_FILE         9
#define TOP_BIN_FILE            10
#define TOP_HASH_FILE           11

#define    MAX_FILENAMES 100000

// MAX_DIR_LEVELS * MAX_DIR_SECTORS * 2048 will be the maximum size of fsbuffer during execution
#define MIN_DIR_SECTORS 20
#define MAX_DIR_SECTORS 300  // needs to be an even multiple of MIN_DIR_SECTORS; largest observed was cod4 (53)
#define MIN_DIR_LEVELS  10
#define MAX_DIR_LEVELS  150  // needs to be an even multiple of MIN_DIR_LEVELS; largest observed was dark messiah (22)

#define WOW_THATS_A_LOT_OF_RAM 134217728  // 128 MB

// values for extended c/r
#define NUM_SS_CR_SAMPLES   51  // should be an odd number, may cause poor formatting of bar graphs if greater than 59
//#define NUM_AP25_CR_SAMPLES 31  // should be an odd number, may cause poor formatting of bar graphs if greater than 57
//#define AP25_HIGH_ANGLE     9   // don't change this
//#define AP25_MEDIUM_ANGLE   5
#define SS_TOLERANCE        15
#define SS_HIGH_ANGLE       9
#define SS_MEDIUM_ANGLE     5

//#define DAE_HEADER_SIZE 336
//#define DAE_SEARCH_MID  0
//#define DAE_SEARCH_DID  1
//#define DAE_SEARCH_TID  2

#define BIGBUF_SIZE 32768  // 32 KB, changing this could cause some problems

#define NUM_CURRENTPFIENTRIES   15  // update this when adding new pfi entries
#define NUM_CURRENTVIDEOENTRIES 21 // update this when adding new video entries

#define NUM_CURRENTAP25MEDIAIDS       25  // update this when adding new AP25 media ids for discs that have no AP25 flag in the default.xex
#define NUM_CURRENTAP25TITLEIDS        2  // update this when adding new AP25 title ids for discs that have no AP25 flag in the default.xex
#define NUM_CURRENTAP25DISCPROFILEIDS 11  // update this when adding new AP25 disc profile ids
//#define NUM_CURRENTDAEVERSIONS        5   // update this when adding new DAE versions and descriptions

#define TOPOLOGY_SIZE 26624

// update version values here
extern const char *headerversion;
extern const char *curluseragent;
extern unsigned long currentversion;  // MSB (1.2.3 = 0x010203)

// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long latestversion;

// update this value before release (unrecognized pfi/video on a game authored before this date is labeled almost
// certainly corrupt, otherwise it might be a brand new wave)
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long long lastknownwave;  // 2011-09-06

// update this value if additional stealth sectors are needed in our ISOs so that older versions
// of abgx360 will not blank them out if fixing video padding is enabled (works for XGD2 or XGD3)
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned long number_of_stealth_sectors;  // increase if needed, never decrease (13 sectors for topology data plus 1 sector each for PFI, DMI, SS)

// update this value if stealth sectors or data ever needs to be placed in the 16 sectors between an XGD3 SS and the game partition
// xgd3_stealth_padding_bitfield is 16 bits, one for each of the 16 sectors
// a bit value of 1 indicates a padding sector, 0 indicates a data sector
// Example: 0x7EFE = 0111111011111110 means that only sectors 2-7 and 9-15 will be checked for blank padding if check/fix padding is enabled
// all bits are currently set to 1 (1111111111111111 = 0xFFFF) which means that all 16 sectors will be checked for blank padding
// this will be replaced with the value from abgx360.dat if it exists
extern unsigned short xgd3_stealth_padding_bitfield;

// local directories
extern char homedir[2048];
extern const char *abgxdir;
extern const char *stealthdir;
extern const char *userstealthdir;
extern const char *imagedir;


// load replacements from abgx360.ini if it exists (make sure to update checkini() if these addresses are changed)
extern const char *webinidir;                       // dir that contains verified ini files
extern const char *webunverifiedinidir;                     // dir that contains unverified ini files
extern const char *webcsv;              // http path to GameNameLookup.csv
extern const char *webdat;                     // http path to abgx360.dat
extern const char *webtopology;                    // http path to topology.php
extern const char *webstealthdir;                   // dir that contains SS/DMI/PFI/Video stealth files
extern const char *autouploadwebaddress;          // form for submitting AutoUploads

//char *ap25autouploadwebaddress;  // form for submitting AP25 AutoUploads
//char *webdae;                            // http path to dae.bin

extern struct mediaidshort { unsigned char mediaid[4]; } mediaidshort;
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

extern int mediumangledev_value, highangledev_value, fixangledev_value;

extern bool verbose, stealthcheck, autofix, autofixuncertain, verify, onlineupdate, csvupdate;
extern bool checkdvdfile, checkpadding, padL0, fixdeviation, fixDRT, increasescreenbuffersize;
extern bool autofixalways, autoupload, keeporiginaliso, dontparsefs;
extern bool extraverbose, debug, debugfs;
extern bool noheader, justheader, justfooter;
extern bool minimal, html, stripcolors, script, justhelp;
extern bool terminal, stayoffline, showsstable;
extern bool pauseshell, maximize, showfiles;
extern bool fixangle359, showfulltable;
extern bool homeless, makedatfile;
extern bool patchvalidfilesonly, patchifstealthpasses, manualpatch, manualextract;
extern bool rebuildlowspace, norebuild, truncatefile, checkcorruption, foldermode;
extern bool matchonly, testing, testingdvd;
extern bool localonly, recursesubdirs, clobber;
extern bool showachievements, hidesecretachievements, showavatarawards, unicode, imagedirmissing;
extern bool skiplayerboundaryinfo, devkey, trustssv2angles, useinstalldir;
extern struct badshit { unsigned char c[21], d[21], data[21]; int count; char *explanation; } badshit;
extern char unrecognizedRTarray[21];
// don't forget to add new args to the list before stat()
extern int truncatearg, userregionarg, folderarg, matcharg, specialarg, readretryarg;
extern int patchvideoarg, patchpfiarg, patchdmiarg, patchssarg;
extern int extractvideoarg, extractpfiarg, extractdmiarg, extractssarg;
extern int autouploaduserarg, autouploadpassarg, fixangledevarg, connectiontimeoutarg, dvdtimeoutarg;
extern int dvdarg, userlangarg, origarg, speedarg;
//int riparg, ripdestarg;
extern long connectiontimeout, dvdtimeout, userlang;
extern float speed;
extern unsigned long curlprogressstartmsecs, userregion;

extern char buffer[2048], buffer2[2048];
extern char inifilename[24], xexinifilename[17], gamename[151];
extern char sense[20], specialerror[200];
extern char installdirvideofilepath[2048];
extern unsigned char ubuffer[2048], ss[2048], fixed_ss[2048], cdb[12];
extern unsigned char bigbuffer[BIGBUF_SIZE];
extern unsigned long filecount;
extern unsigned long seek;

extern const char *green, *yellow, *red, *cyan, *blue;
extern const char *darkblue, *white, *arrow, *box, *normal;
extern const char *wtfhexcolor, *wtfcharcolor, *reset, *brown, *filename;
extern const char *blackonyellow, *blackonred;
extern const char *hexoffsetcolor, *darkgray;
extern const char *newline, *quotation, *ampersand, *lessthan, *greaterthan, *numbersign;
extern const char *sp0, *sp1, *sp2, *sp3, *sp4, *sp5;
extern const char *sp6, *sp7, *sp8, *sp9;
extern const char *sp10, *sp11, *sp12, *sp18;
extern const char *sp20, *sp21, *sp28;

extern unsigned long fix_ss_crc32;
extern long long fpfilesize;
extern int isofilearg, readretries, charsprinted;
extern unsigned long readerrorstotal, readerrorsrecovered, writeerrorstotal, writeerrorsrecovered;
extern int readerrorcharsprinted;
extern int returnvalue;
extern long longreturnvalue;
extern unsigned long sizeoverbuffer, bufferremainder;

extern char *isofilename;

// global vars that might need to be reset after every fileloop
extern bool writefile;
extern bool checkgamecrcalways, checkgamecrcnever, gamecrcfailed, verifyfailed;
extern bool stealthfailed, stealthuncertain, xbox1iso;
extern bool ss_stealthfailed, dmi_stealthfailed, pfi_stealthfailed, video_stealthfailed;
extern bool dmi_stealthuncertain, ss_stealthuncertain, pfi_stealthuncertain, video_stealthuncertain;
extern bool xex_foundmediaid, xex_founddiscprofileid, foundtitleid, foundregioncode, foundgamename;
extern bool ss_foundtimestamp, usercancelledgamecrc;
extern bool pfi_alreadydumped, pfi_foundsectorstotal, pfiexception;
extern bool wtfhex, checkssbin;
extern bool justastealthfile, isotoosmall;
extern bool drtfucked, fixedss, fixedtopology; //fixedap25;
extern bool printstderr, rebuildfailed, curlheaderprinted;
extern int unrecognizedRTcount;
//int videowave, pfiwave, truepfiwave;
extern unsigned long long video, ss_authored;
extern unsigned long game_crc32, xex_crc32, ss_crc32, ss_rawcrc32, ss_staticcrc32, dmi_crc32, pfi_crc32;
extern unsigned int ss_num_angles, ss_num_targets, ss_angleaddresses[4], ss_targets[4];
extern unsigned long video_crc32, videoL0_crc32, videoL1_crc32;

extern int ini_dmi_count;
extern unsigned long ini_ss, ini_pfi, ini_video, ini_rawss, ini_v0, ini_v1, ini_game, ini_xexhash;
extern unsigned long ini_dmi[30];
extern int corruptionoffsetcount;
extern int buffermaxdir;
extern int bufferlevels;
extern char *fsbuffer;
extern char dirprefix[2048];
extern unsigned long long totalbytes;
extern unsigned long totalfiles, totaldirectories;
extern long long L0capacity;
extern int level;
extern bool parsingfsfailed;
extern bool extractimages, embedimages, noxexiniavailable, offlinewarningprinted;
extern bool ssv2, iso_has_ssv2;
extern bool verify_found_bad_pfi_or_video;
extern bool game_has_ap25, xgd3, topology_was_verified;
extern bool drive_speed_needs_to_be_reset;
extern unsigned int ss_replay_table_offset, ss_replay_table_length;
extern long layerbreak;

#ifdef __cplusplus
}
#endif