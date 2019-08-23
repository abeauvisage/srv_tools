#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <boost/foreach.hpp>
#include <boost/progress.hpp>

#include <sensor_msgs/Imu.h>

namespace bag_tools
{

class ImuBagProcessor
{

public:

  typedef boost::function<void (const sensor_msgs::ImuConstPtr&)> CallbackType;

  ImuBagProcessor(const std::string& imu_topic) :
    imu_topic_(imu_topic)
  {
    ros::Time::init();
  }

  void registerCallback(const CallbackType& callback)
  {
    callback_ = callback;
  }
  /**
   * Processes given bagfile, calls registered callback function when
   * a message is found.
   */
  void processBag(const std::string &filename)
  {
    // imu topics to load
    std::vector<std::string> topics;
    topics.push_back(imu_topic_);

    std::cout << "Starting processing " << filename << ", ";
    rosbag::Bag bag;
    bag.open(filename, rosbag::bagmode::Read);
    rosbag::View view(bag, rosbag::TopicQuery(topics));

    int num_messages = view.size();
    std::cout << num_messages << " messages to process." << std::endl;

    // Load all messages
    boost::progress_display show_progress(num_messages);
    BOOST_FOREACH(rosbag::MessageInstance const m, view)
    {
      if (m.getTopic() == imu_topic_)
      {
        sensor_msgs::Imu::ConstPtr imu_msg = m.instantiate<sensor_msgs::Imu>();
        if (imu_msg != NULL && callback_.empty() == false)
        {
          callback_(imu_msg);
        }
      }
      ++show_progress;
    }
    bag.close();
    std::cout << "Finished processing " << filename << std::endl;
  }

private:

  std::string imu_topic_;
  CallbackType callback_;

};

} // end of namespace

