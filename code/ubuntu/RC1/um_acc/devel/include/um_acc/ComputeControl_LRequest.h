// Generated by gencpp from file um_acc/ComputeControl_LRequest.msg
// DO NOT EDIT!


#ifndef UM_ACC_MESSAGE_COMPUTECONTROL_LREQUEST_H
#define UM_ACC_MESSAGE_COMPUTECONTROL_LREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace um_acc
{
template <class ContainerAllocator>
struct ComputeControl_LRequest_
{
  typedef ComputeControl_LRequest_<ContainerAllocator> Type;

  ComputeControl_LRequest_()
    : y(0.0)
    , v(0.0)
    , phi(0.0)
    , r(0.0)  {
    }
  ComputeControl_LRequest_(const ContainerAllocator& _alloc)
    : y(0.0)
    , v(0.0)
    , phi(0.0)
    , r(0.0)  {
    }



   typedef double _y_type;
  _y_type y;

   typedef double _v_type;
  _v_type v;

   typedef double _phi_type;
  _phi_type phi;

   typedef double _r_type;
  _r_type r;




  typedef boost::shared_ptr< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> const> ConstPtr;

}; // struct ComputeControl_LRequest_

typedef ::um_acc::ComputeControl_LRequest_<std::allocator<void> > ComputeControl_LRequest;

typedef boost::shared_ptr< ::um_acc::ComputeControl_LRequest > ComputeControl_LRequestPtr;
typedef boost::shared_ptr< ::um_acc::ComputeControl_LRequest const> ComputeControl_LRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::um_acc::ComputeControl_LRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace um_acc

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'um_acc': ['/home/ubuntu/rccar_catkin_ws/src/um_acc/msg', '/home/ubuntu/rccar_catkin_ws/src/um_acc/msg'], 'std_msgs': ['/opt/ros/jade/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4e4939b536e65d0f1c171d857d476271";
  }

  static const char* value(const ::um_acc::ComputeControl_LRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4e4939b536e65d0fULL;
  static const uint64_t static_value2 = 0x1c171d857d476271ULL;
};

template<class ContainerAllocator>
struct DataType< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "um_acc/ComputeControl_LRequest";
  }

  static const char* value(const ::um_acc::ComputeControl_LRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 y\n\
float64 v\n\
float64 phi\n\
float64 r\n\
";
  }

  static const char* value(const ::um_acc::ComputeControl_LRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.y);
      stream.next(m.v);
      stream.next(m.phi);
      stream.next(m.r);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct ComputeControl_LRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::um_acc::ComputeControl_LRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::um_acc::ComputeControl_LRequest_<ContainerAllocator>& v)
  {
    s << indent << "y: ";
    Printer<double>::stream(s, indent + "  ", v.y);
    s << indent << "v: ";
    Printer<double>::stream(s, indent + "  ", v.v);
    s << indent << "phi: ";
    Printer<double>::stream(s, indent + "  ", v.phi);
    s << indent << "r: ";
    Printer<double>::stream(s, indent + "  ", v.r);
  }
};

} // namespace message_operations
} // namespace ros

#endif // UM_ACC_MESSAGE_COMPUTECONTROL_LREQUEST_H
