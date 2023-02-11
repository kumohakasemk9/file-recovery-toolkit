file-recovery-toolkit
====
Ext4 dumpstar diver program for recovering deleted files. (Searches in marked-for-unused or marked-for-deleted area)  

filesearcher.c - Added Sep 20 2021   
Finds data pattern in specified file and tells the location in the file, if found.   

Filesearcher command usage
=====
./filesearcher filename pattern   
filename: The filename you want to search for pattern.   
pattern:  Target file pattern. If specified pattern found in specified file, the program   
          will tell you address of first occurance, and dumps data around the pattern.   
          Pattern format will be sXXXXXX or bXXYYXXYY. sXXXXXX means string "XXXXXX"   
          bXXYYXXYY means binary data 0xXX 0xYY 0xXX 0xYY.   
         
License
====
(C) 2022 kumohakase   
You can share, modify this code without deleting this section.
In commercial usage, gimme 10% of earning. Please consider supporting me using kofi.com https://ko-fi.com/kumohakase
