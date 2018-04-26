// Generated by gencpp from file um_acc/ComputeControl.msg
// DO NOT EDIT!


#ifndef UM_ACC_MESSAGE_COMPUTECONTROL_H
#define UM_ACC_MESSAGE_COMPUTECONTROL_H

#include <ros/service_traits.h>


#include <um_acc/ComputeControlRequest.h>
#include <um_acc/ComputeControlResponse.h>


namespace um_acc
{

struct ComputeControl
{

typedef ComputeControlRequest Request;
typedef ComputeControlResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct ComputeControl
} // namespace um_acc


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::um_acc::ComputeControl > {
  static const char* value()
  {
    return "99cbc1e3badf94dc8e3459eb691661ab";
  }

  static const char* value(const ::um_acc::ComputeControl&) { return value(); }
};

template<>
struct DataType< ::um_acc::ComputeControl > {
  static const char* value()
  {
    return "um_acc/ComputeControl";
  }

  static const char* value(const ::um_acc::ComputeControl&) { return value(); }
};


// service_traits::MD5Sum< ::um_acc::ComputeControlRequest> should match 
// service_traits::MD5Sum< ::um_acc::ComputeControl > 
template<>
struct MD5Sum< ::um_acc::ComputeControlRequest>
{
  static const char* value()
  {
    return MD5Sum< ::um_acc::ComputeControl >::value();
  }
  static const char* value(const ::um_acc::ComputeControlRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::um_acc::ComputeControlRequest> should match 
// service_traits::DataType< ::um_acc::ComputeControl > 
template<>
struct DataType< ::um_acc::ComputeControlRequest>
{
  static const char* value()
  {
    return DataType< ::um_acc::ComputeControl >::value();
  }
  static const char* value(const ::um_acc::ComputeControlRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::um_acc::ComputeControlResponse> should match 
// service_traits::MD5Sum< ::um_acc::ComputeControl > 
template<>
struct MD5Sum< ::um_acc::ComputeControlResponse>
{
  static const char* value()
  {
    return MD5Sum< ::um_acc::ComputeControl >::value();
  }
  static const char* value(const ::um_acc::ComputeControlResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::um_acc::ComputeControlResponse> should match 
// service_traits::DataType< ::um_acc::ComputeControl > 
template<>
struct DataType< ::um_acc::ComputeControlResponse>
{
  static const char* value()
  {
    return DataType< ::um_acc::ComputeControl >::value();
  }
  static const char* value(const ::um_acc::ComputeControlResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // UM_ACC_MESSAGE_COMPUTECONTROL_H