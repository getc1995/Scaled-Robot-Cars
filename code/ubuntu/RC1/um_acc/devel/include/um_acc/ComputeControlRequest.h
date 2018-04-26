// Generated by gencpp from file um_acc/ComputeControlRequest.msg
// DO NOT EDIT!


#ifndef UM_ACC_MESSAGE_COMPUTECONTROLREQUEST_H
#define UM_ACC_MESSAGE_COMPUTECONTROLREQUEST_H


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
struct ComputeControlRequest_
{
  typedef ComputeControlRequest_<ContainerAllocator> Type;

  ComputeControlRequest_()
    : v(0.0)
    , h(0.0)
    , vl(0.0)  {
    }
  ComputeControlRequest_(const ContainerAllocator& _alloc)
    : v(0.0)
    , h(0.0)
    , vl(0.0)  {
    }



   typedef double _v_type;
  _v_type v;

   typedef double _h_type;
  _h_type h;

   typedef double _vl_type;
  _vl_type vl;




  typedef boost::shared_ptr< ::um_acc::ComputeControlRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::um_acc::ComputeControlRequest_<ContainerAllocator> const> ConstPtr;

}; // struct ComputeControlRequest_

typedef ::um_acc::ComputeControlRequest_<std::allocator<void> > ComputeControlRequest;

typedef boost::shared_ptr< ::um_acc::ComputeControlRequest > ComputeControlRequestPtr;
typedef boost::shared_ptr< ::um_acc::ComputeControlRequest const> ComputeControlRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::um_acc::ComputeControlRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::um_acc::ComputeControlRequest_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::um_acc::ComputeControlRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::um_acc::ComputeControlRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::um_acc::ComputeControlRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "334d257651a8f411a4db23bc120d1b4e";
  }

  static const char* value(const ::um_acc::ComputeControlRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x334d257651a8f411ULL;
  static const uint64_t static_value2 = 0xa4db23bc120d1b4eULL;
};

template<class ContainerAllocator>
struct DataType< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "um_acc/ComputeControlRequest";
  }

  static const char* value(const ::um_acc::ComputeControlRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 v\n\
float64 h\n\
float64 vl\n\
";
  }

  static const char* value(const ::um_acc::ComputeControlRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.v);
      stream.next(m.h);
      stream.next(m.vl);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct ComputeControlRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::um_acc::ComputeControlRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::um_acc::ComputeControlRequest_<ContainerAllocator>& v)
  {
    s << indent << "v: ";
    Printer<double>::stream(s, indent + "  ", v.v);
    s << indent << "h: ";
    Printer<double>::stream(s, indent + "  ", v.h);
    s << indent << "vl: ";
    Printer<double>::stream(s, indent + "  ", v.vl);
  }
};

} // namespace message_operations
} // namespace ros

#endif // UM_ACC_MESSAGE_COMPUTECONTROLREQUEST_H