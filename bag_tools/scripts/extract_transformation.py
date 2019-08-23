#!/usr/bin/python

PKG = 'bag_tools' # this package name

import roslib; roslib.load_manifest(PKG)
import rospy
import rosbag
from tf_bag import BagTfTransformer
import os
import sys
import argparse

def extract_transform(inbag,outdir,frames):
  rospy.loginfo(' Processing input bagfile: %s', inbag)
  rospy.loginfo(' Writing to output dir: %s', outdir)
  rospy.loginfo(' Extracting tansformation: %s', frames)

  file = open(outdir+"/tf_data.csv","w+")
  file.write("#transformation between "+frames[0]+" and "+frames[1]+"\n")
  file.write("#timestamp,q_x,q_y,q_z,q_w,x,y,z\n")
  bag = rosbag.Bag(inbag,'r')
  bag_transformer = BagTfTransformer(bag)
  
  if len(frames) < 2:
  	print "[Error] not enough frames"
  	exit(-1)
  	
  transform_time = bag_transformer.waitForTransform(frames[0], frames[1])
  rospy.loginfo('first tansformation: %s', transform_time)
  for i in range(1,5000):
  	try:
	  	translation, quaternion = bag_transformer.lookupTransform(frames[0], frames[1], transform_time)
	  	file.write(str(transform_time)+","+str(quaternion[0])+","+str(quaternion[1])+","+str(quaternion[2])+","+str(quaternion[3])+","+str(translation[0])+","+str(translation[1])+","+str(translation[2])+"\n")
  		transform_time = bag_transformer.waitForTransform(frames[0], frames[1],transform_time)
	  	rospy.loginfo('tansformation found: %s', transform_time)
	except Exception, e:
		break
		
  rospy.loginfo('Closing output bagfile and exit...')
  bag.close();
  file.close()

if __name__ == "__main__":
  rospy.init_node('extract_transformation')
  parser = argparse.ArgumentParser(
      description='Extracts transformations from a bagfile into a file.')
  parser.add_argument('inbag', help='input bagfile')
  parser.add_argument('outdir', help='output directory')
  parser.add_argument('frames', nargs='+', help='frames corresponding to transformation to extract')
  args = parser.parse_args()

  try:
    extract_transform(args.inbag,args.outdir,args.frames)
    
  except Exception, e:
    import traceback
    traceback.print_exc()

