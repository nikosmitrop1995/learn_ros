/* This code comes from:
*   https://raw.githubusercontent.com/ros2/examples/foxy/rclcpp/topics/minimal_publisher/member_function.cpp
*   Based on this tutorial:
*   https://docs.ros.org/en/foxy/Tutorials/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html
*/

/* The top of the code includes the standard C++ headers you will be using.
*  After the standard C++ headers is the rclcpp/rclcpp.hpp include 
*   which allows you to use the most common pieces of the ROS 2 system. 
*  Last is std_msgs/msg/string.hpp, which includes the built-in message type you will use to publish data.

*  These lines represent the node’s dependencies.
*  Recall that dependencies have to be added to package.xml and CMakeLists.txt,
*   which you’ll do in the next section.
*/

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
*  member function as a callback from the timer.
*  Line 32 creates the node class MinimalPublisher by inheriting from rclcpp::Node.
*  Every this-> in the code is referring to the node.
*/
class MinimalPublisher : public rclcpp::Node //Class Inherits from Node class
{
    /* The public constructor names the node talker and initializes count_ to 0.
    */
    public:
        MinimalPublisher(): // Constructor
            Node("talker_node"), count_(0) //Node is called talker and count_ initializes to 0
            {
                /* Inside the constructor, the publisher is initialized with the String message type, the topic name topic, and the required queue size to limit messages in the event of a backup. 
                *  Next, timer_ is initialized, which causes the timer_callback function to be executed twice a second.
                */
                publisher_ = this->create_publisher<std_msgs::msg::String>("talker",10);
                timer_ = this->create_wall_timer(
                    500ms,std::bind(&MinimalPublisher::timer_callback, this));
            }
    private:
        void timer_callback()
        {
            auto message = std_msgs::msg::String();
            message.data = "Hello, world! " + std::to_string(count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'",message.data.c_str());
            publisher_->publish(message);
        }
        size_t count_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}