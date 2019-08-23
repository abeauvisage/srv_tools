#!env python

# Usage
# rename_png DIRECTORY_WHERE_IMAGES_ARE camera_name

import os
import sys
import string

if len(sys.argv) < 3:
	print "[error] dir or cam name not provided"
	exit(-1)
	
number=1
dir = sys.argv[1] 
cam_name = sys.argv[2] #eg. cam0_image
print "dir: "+dir
outFile = open(dir+"/image_data.csv","w+")

for filename in sorted(os.listdir(dir)):
	if filename.startswith("image") and filename.endswith(".png"):
		outFile.write(filename[5:11]+","+filename[12:31]+"\n")
		print cam_name+str(int(filename[5:11])).zfill(5)+".png"
		os.rename(dir+"/"+filename,dir+"/"+cam_name+str(int(filename[5:11])).zfill(5)+".png")
		
