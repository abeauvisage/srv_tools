#include <ros/ros.h>

#include <boost/format.hpp>

#include <sensor_msgs/Imu.h>

#include <image_proc/processor.h>

#include "bag_tools/imu_bag_processor.h"

#include <memory>
#include <iostream>
#include <fstream>

/**
 * Saves inertial data
 */
class ImuSaver
{
public:
  ImuSaver(const std::string& filename) : filename_(filename),num_saved_(0){
  
      std::ofstream imuFile(filename_,std::ios_base::trunc);
      if (!imuFile.is_open())
      {
        ROS_ERROR_STREAM("ERROR saving in " << filename_);
      }
      else
        imuFile << "#timestamp,orientation,angular_velocity,linear_acceleration" << std::endl;
      imuFile.close();
  }

  /*ImuSaver(const ImuSaver& saver): num_saved_(saver.num_saved_){
      imuFile_ = &(saver.imuFile_);
  }*/

  void save(const sensor_msgs::ImuConstPtr &imuData)
  {
    std::ofstream imuFile(filename_,std::ios_base::app);
    if (!imuFile.is_open())
    {
      std::cerr << "ERROR ImuFile not opened" << std::endl;
      return;
    }
    if (!imuData)
    {
      std::cerr << "ERROR wrong ImuData" << std::endl;
      return;
    }else{
        imuFile << imuData->header.stamp.toNSec() << ","
                << imuData->orientation.x << "," << imuData->orientation.y << "," << imuData->orientation.z << "," << imuData->orientation.w
                << "," << imuData->angular_velocity.x << "," << imuData->angular_velocity.y << "," << imuData->angular_velocity.z
                << "," << imuData->linear_acceleration.x << "," << imuData->linear_acceleration.y << "," << imuData->linear_acceleration.z << std::endl;
        num_saved_++;
    }
    imuFile.close();
  }
private:

  std::string filename_;
  int num_saved_;
};

int main(int argc, char** argv)
{
  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " OUT_DIR FILETYPE IMU_TOPIC BAGFILE [BAGFILE...]" << std::endl;
    std::cout << "  Example: " << argv[0] << " /tmp csv /stereo_down/left/image_raw bag1.bag bag2.bag" << std::endl;
    return 0;
  }

  std::string out_dir(argv[1]);
  std::string filetype(argv[2]);
  std::string imu_topic(argv[3]);

  ros::Time::init();

  ImuSaver saver(out_dir+"/"+"imu_data."+filetype);
  bag_tools::ImuBagProcessor processor(imu_topic);
  processor.registerCallback(boost::bind(&ImuSaver::save, saver, _1));
  std::cout << "registered" << std::endl;
  
  for (int i = 4; i < argc; ++i)
    processor.processBag(argv[i]);

  return 0;
}

