#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "macros.h"

#include <stdbool.h>   // true/false macro for bools

#include "sha1.h"


#ifdef WIN32
    #include <Windows.h>
    #include <ntddstor.h> // device i/o stuff
    #include <ntddscsi.h> // SCSI_PASS_THROUGH_DIRECT

    char winbuffer[2048];
    HANDLE hDevice = INVALID_HANDLE_VALUE;
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

#define	MAX_FILENAMES 100000

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
const char *headerversion = "v1.0.7";
const char *curluseragent = "abgx360 v1.0.7 (\"ABGX360_OS\")";
unsigned long currentversion = 0x010007L;  // MSB (1.2.3 = 0x010203)

// this will be replaced with the value from abgx360.dat if it exists
unsigned long latestversion = 0L;

// update this value before release (unrecognized pfi/video on a game authored before this date is labeled almost
// certainly corrupt, otherwise it might be a brand new wave)
// this will be replaced with the value from abgx360.dat if it exists
unsigned long long lastknownwave = 0x01CC6C27EBA6C000LL;  // 2011-09-06

// update this value if additional stealth sectors are needed in our ISOs so that older versions
// of abgx360 will not blank them out if fixing video padding is enabled (works for XGD2 or XGD3)
// this will be replaced with the value from abgx360.dat if it exists
unsigned long number_of_stealth_sectors = 16L;  // increase if needed, never decrease (13 sectors for topology data plus 1 sector each for PFI, DMI, SS)

// update this value if stealth sectors or data ever needs to be placed in the 16 sectors between an XGD3 SS and the game partition
// xgd3_stealth_padding_bitfield is 16 bits, one for each of the 16 sectors
// a bit value of 1 indicates a padding sector, 0 indicates a data sector
// Example: 0x7EFE = 0111111011111110 means that only sectors 2-7 and 9-15 will be checked for blank padding if check/fix padding is enabled
// all bits are currently set to 1 (1111111111111111 = 0xFFFF) which means that all 16 sectors will be checked for blank padding
// this will be replaced with the value from abgx360.dat if it exists
unsigned short xgd3_stealth_padding_bitfield = 0xFFFF;

// local directories
char homedir[2048];
#ifdef WIN32
    const char *abgxdir =        "\\abgx360\\";
    const char *stealthdir =     "StealthFiles\\";
    const char *userstealthdir = "UserStealthFiles\\";
    const char *imagedir =       "Images\\";
#else
    const char *abgxdir =        "/.abgx360/";
    const char *stealthdir =     "StealthFiles/";
    const char *userstealthdir = "UserStealthFiles/";
    const char *imagedir =       "Images/";
#endif

// load replacements from abgx360.ini if it exists (make sure to update checkini() if these addresses are changed)
const char *webinidir =            "http://abgx360.hadzz.com/verified/";                       // dir that contains verified ini files
const char *webunverifiedinidir =  "http://abgx360.hadzz.com/unverified/";                     // dir that contains unverified ini files
const char *webcsv =               "http://abgx360.hadzz.com/GameNameLookup.csv";              // http path to GameNameLookup.csv
const char *webdat =               "http://abgx360.hadzz.com/abgx360.dat";                     // http path to abgx360.dat
const char *webtopology =          "http://abgx360.hadzz.com/topology.php";                    // http path to topology.php
const char *webstealthdir =        "http://abgx360.hadzz.com/StealthFiles/";                   // dir that contains SS/DMI/PFI/Video stealth files
const char *autouploadwebaddress = "http://abgx360.hadzz.com/Control/AutoUpload.php";          // form for submitting AutoUploads

//char *ap25autouploadwebaddress = "http://abgx360.net/Apps/Control/AP25AutoUpload.php";  // form for submitting AP25 AutoUploads
//char *webdae = "http://abgx360.net/Apps/Stealth360/dae.bin";                            // http path to dae.bin

struct waveentry {unsigned long crc; uchar sha1[20]; char *description; bool hosted;} waveentry;
struct waveentry currentpfientries[NUM_CURRENTPFIENTRIES];
struct waveentry *mostrecentpfientries;
struct waveentry *datfilepfientries;
unsigned long num_pfientries;
struct waveentry currentvideoentries[NUM_CURRENTVIDEOENTRIES];
struct waveentry *mostrecentvideoentries;
struct waveentry *datfilevideoentries;
unsigned long num_videoentries;
unsigned long xbox1pficrc;
uchar xbox1pfisha1[20];
unsigned long xbox1videocrc;
uchar xbox1videosha1[20];

struct mediaidshort {unsigned char mediaid[4];} mediaidshort;
struct mediaidshort currentap25mediaids[NUM_CURRENTAP25MEDIAIDS];
struct mediaidshort *mostrecentap25mediaids;
struct mediaidshort *datfileap25mediaids;
unsigned long num_ap25mediaids;

struct mediaidshort currentap25titleids[NUM_CURRENTAP25TITLEIDS];
struct mediaidshort *mostrecentap25titleids;
struct mediaidshort *datfileap25titleids;
unsigned long num_ap25titleids;

struct mediaidshort currentap25discprofileids[NUM_CURRENTAP25DISCPROFILEIDS];
struct mediaidshort *mostrecentap25discprofileids;
struct mediaidshort *datfileap25discprofileids;
unsigned long num_ap25discprofileids;

//struct flagverdescription {unsigned char flags; unsigned short version; char *description;};
//struct flagverdescription currentdaeversions[NUM_CURRENTDAEVERSIONS];
//struct flagverdescription *mostrecentdaeversions;
//struct flagverdescription *datfiledaeversions;
//unsigned long num_daeversions;

int mediumangledev_value = 3, highangledev_value = 9, fixangledev_value = 3;

bool verbose = true, stealthcheck = true, autofix = true, autofixuncertain = true, verify = true, onlineupdate = true, csvupdate = false;
bool checkdvdfile = true, checkpadding = false, padL0 = false, fixdeviation = true, fixDRT = true, increasescreenbuffersize = true;
bool autofixalways = false, autoupload = false, keeporiginaliso = false, dontparsefs = false;
bool extraverbose = false, debug = false, debugfs = false;
bool noheader = false, justheader = false, justfooter = false;
bool minimal = false, html = false, stripcolors = false, script = false, justhelp = false;
bool terminal = false, stayoffline = false, showsstable = false;
bool pauseshell = false, maximize = false, showfiles = false;
bool fixangle359 = false, showfulltable = false;
bool homeless = false, makedatfile = false;
bool patchvalidfilesonly = true, patchifstealthpasses = false, manualpatch = false, manualextract = false;
bool rebuildlowspace = false, norebuild = false, truncatefile = false, checkcorruption = false, foldermode = false;
bool matchonly = false, testing = false, testingdvd = false;
bool localonly = false, recursesubdirs = false, clobber = false;
bool showachievements = false, hidesecretachievements = false, showavatarawards = false, unicode = false, imagedirmissing = false;
bool skiplayerboundaryinfo = false, devkey = false, trustssv2angles = true, useinstalldir = false;
struct badshit {unsigned char c[21], d[21], data[21]; int count; char* explanation;} badshit;
char unrecognizedRTarray[21];
// don't forget to add new args to the list before stat()
int truncatearg = 0, userregionarg = 0, folderarg = 0, matcharg = 0, specialarg = 0, readretryarg = 0;
int patchvideoarg = 0, patchpfiarg = 0, patchdmiarg = 0, patchssarg = 0;
int extractvideoarg = 0, extractpfiarg = 0, extractdmiarg = 0, extractssarg = 0;
int autouploaduserarg = 0, autouploadpassarg = 0, fixangledevarg = 0, connectiontimeoutarg = 0, dvdtimeoutarg = 0;
int dvdarg = 0, userlangarg = 0, origarg = 0, speedarg = 0;
//int riparg = 0, ripdestarg = 0;
long connectiontimeout = 20, dvdtimeout = 20, userlang = 0;
float speed = 0.0;
unsigned long curlprogressstartmsecs, userregion = 0L;

char buffer[2048], buffer2[2048];
char inifilename[24], xexinifilename[17], gamename[151];
char sense[20], specialerror[200];
char installdirvideofilepath[2048] = {0};
unsigned char ubuffer[2048], ss[2048], fixed_ss[2048], cdb[12];
unsigned char bigbuffer[BIGBUF_SIZE];
unsigned long filecount = 0L;
unsigned long seek;

const char *green = "\033[1;32;40m", *yellow = "\033[1;33;40m", *red = "\033[1;31;40m", *cyan = "\033[1;36;40m", *blue = "\033[1;34;40m";
const char *darkblue = "\033[0;34;40m", *white = "\033[1;37;40m", *arrow = "\033[1;34;40m", *box = "\033[1;34;40m", *normal = "\033[0;37;40m";
const char *wtfhexcolor = "\033[1;31;40m", *wtfcharcolor = "\033[1;37;41m", *reset = "\033[0m", *brown = "\033[0;33;40m", *filename = "\033[0;37;44m";
const char *blackonyellow = "\033[0;30;43m", *blackonred = "\033[0;30;41m";
#ifdef __APPLE__
    const char *hexoffsetcolor = "\033[0;37;40m", *darkgray = "\033[0;37;40m";  // can't do dark gray apparently (shows completely black) so just use normal gray
#else
    const char *hexoffsetcolor = "\033[1;30;40m", *darkgray = "\033[1;30;40m";
#endif
const char *newline = "\n", *quotation = "\"", *ampersand = "&", *lessthan = "<", *greaterthan = ">", *numbersign = "#";
const char *sp0 = "\0", *sp1 = " ", *sp2 = "  ", *sp3 = "   ", *sp4 = "    ", *sp5 = "     ";
const char *sp6 = "      ", *sp7 = "       ", *sp8 = "        ", *sp9 = "         ";
const char *sp10 = "          ", *sp11 = "           ", *sp12 = "            ", *sp18 = "                  ";
const char *sp20 = "                    ", *sp21 = "                     ", *sp28 = "                            ";


unsigned long fix_ss_crc32;
long long fpfilesize;
int isofilearg, readretries = 20, charsprinted;
unsigned long readerrorstotal, readerrorsrecovered, writeerrorstotal, writeerrorsrecovered;
int readerrorcharsprinted;
int returnvalue;
long longreturnvalue;
unsigned long sizeoverbuffer, bufferremainder;

char *isofilename = NULL;

// global vars that might need to be reset after every fileloop
bool writefile = true;
bool checkgamecrcalways = false, checkgamecrcnever = false, gamecrcfailed = false, verifyfailed = false;
bool stealthfailed = false, stealthuncertain = false, xbox1iso = false;
bool ss_stealthfailed = false, dmi_stealthfailed = false, pfi_stealthfailed = false, video_stealthfailed = false;
bool dmi_stealthuncertain = false, ss_stealthuncertain = false, pfi_stealthuncertain = false, video_stealthuncertain = false;
bool xex_foundmediaid = false, xex_founddiscprofileid = false, foundtitleid = false, foundregioncode = false, foundgamename = false;
bool ss_foundtimestamp = false, usercancelledgamecrc = false;
bool pfi_alreadydumped = false, pfi_foundsectorstotal = false, pfiexception = false;
bool wtfhex = false, checkssbin = false;
bool justastealthfile = false, isotoosmall = false;
bool drtfucked = false, fixedss = false, fixedtopology = false; //fixedap25 = false;
bool printstderr = false, rebuildfailed = false, curlheaderprinted = false;
int unrecognizedRTcount = 0;
//int videowave = 0, pfiwave = 0, truepfiwave = 0;
unsigned long long video = 0, ss_authored = 0;
unsigned long game_crc32 = 0, xex_crc32 = 0, ss_crc32 = 0, ss_rawcrc32 = 0, ss_staticcrc32 = 0, dmi_crc32 = 0, pfi_crc32 = 0;
unsigned int ss_num_angles, ss_num_targets, ss_angleaddresses[4], ss_targets[4];
unsigned long video_crc32 = 0, videoL0_crc32 = 0, videoL1_crc32 = 0;
//uchar xex_sha1[20] = {0};
uchar pfi_sha1[20] = {0};
//video_sha1[20] = {0};
int ini_dmi_count = 0;
unsigned long ini_ss = 0, ini_pfi = 0, ini_video = 0, ini_rawss = 0, ini_v0 = 0, ini_v1 = 0, ini_game = 0, ini_xexhash = 0;
unsigned long ini_dmi[30] = {0};
int corruptionoffsetcount = 0;
int buffermaxdir = MIN_DIR_SECTORS;
int bufferlevels = MIN_DIR_LEVELS;
char *fsbuffer;
char dirprefix[2048] = {0};
unsigned long long totalbytes = 0;
unsigned long totalfiles = 0, totaldirectories = 0;
long long L0capacity = -1;
int level = 0;
bool parsingfsfailed = false;
bool extractimages = false, embedimages = false, noxexiniavailable = false, offlinewarningprinted = false;
bool ssv2 = false, iso_has_ssv2 = false;
bool verify_found_bad_pfi_or_video = false;
bool game_has_ap25 = false, xgd3 = false, topology_was_verified = false;
bool drive_speed_needs_to_be_reset = false;
unsigned int ss_replay_table_offset = 0, ss_replay_table_length = 0;
long layerbreak = -1;

#ifdef __cplusplus
}
#endif