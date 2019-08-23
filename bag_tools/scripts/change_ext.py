#!env python

# Usage
# change_ext DIRECTORY_WHERE_IMAGES_ARE current_extension desired_extension

import os
import sys
import string

if len(sys.argv) < 4:
	print "[error] dir or extension not provided\n"
	print "Usage:"
	print "change_ext DIRECTORY_WHERE_IMAGES_ARE current_extension desired_extension"
	exit(-1)
	
number=1
dir = sys.argv[1]
curr_ext = sys.argv[2]
des_ext = sys.argv[3]

print "dir: "+dir

for filename in sorted(os.listdir(dir)):
	if filename.endswith("."+curr_ext):
		print filename[:-len(curr_ext)]+des_ext
		os.rename(dir+"/"+filename,dir+"/"+filename[:-len(curr_ext)]+des_ext)
		
