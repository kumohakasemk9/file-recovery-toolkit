# file-recovery-toolkit

filesearcher.c - Added Sep 20 2021
Finds data pattern in specified file and tells the location in the file, if found.
Compile args: gcc filesearcher.c
No required library nor compile flags
Command usage: ./filesearcher filename pattern
filename: The filename you want to search for pattern.
pattern:  Target file pattern. If specified pattern found in specified file, the program
          will tell you address of first occurance, and dumps data around the pattern.
          Pattern format will be sXXXXXX or bXXYYXXYY. sXXXXXX means string "XXXXXX"
          bXXYYXXYY means binary data 0xXX 0xYY 0xXX 0xYY.
