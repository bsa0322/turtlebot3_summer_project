// Generated by gencpp from file deep_sort_pytorch/Bound.msg
// DO NOT EDIT!


#ifndef DEEP_SORT_PYTORCH_MESSAGE_BOUND_H
#define DEEP_SORT_PYTORCH_MESSAGE_BOUND_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace deep_sort_pytorch
{
template <class ContainerAllocator>
struct Bound_
{
  typedef Bound_<ContainerAllocator> Type;

  Bound_()
    : x1(0.0)
    , x2(0.0)
    , y1(0.0)
    , y2(0.0)  {
    }
  Bound_(const ContainerAllocator& _alloc)
    : x1(0.0)
    , x2(0.0)
    , y1(0.0)
    , y2(0.0)  {
  (void)_alloc;
    }



   typedef double _x1_type;
  _x1_type x1;

   typedef double _x2_type;
  _x2_type x2;

   typedef double _y1_type;
  _y1_type y1;

   typedef double _y2_type;
  _y2_type y2;





  typedef boost::shared_ptr< ::deep_sort_pytorch::Bound_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::deep_sort_pytorch::Bound_<ContainerAllocator> const> ConstPtr;

}; // struct Bound_

typedef ::deep_sort_pytorch::Bound_<std::allocator<void> > Bound;

typedef boost::shared_ptr< ::deep_sort_pytorch::Bound > BoundPtr;
typedef boost::shared_ptr< ::deep_sort_pytorch::Bound const> BoundConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::deep_sort_pytorch::Bound_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::deep_sort_pytorch::Bound_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::deep_sort_pytorch::Bound_<ContainerAllocator1> & lhs, const ::deep_sort_pytorch::Bound_<ContainerAllocator2> & rhs)
{
  return lhs.x1 == rhs.x1 &&
    lhs.x2 == rhs.x2 &&
    lhs.y1 == rhs.y1 &&
    lhs.y2 == rhs.y2;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::deep_sort_pytorch::Bound_<ContainerAllocator1> & lhs, const ::deep_sort_pytorch::Bound_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace deep_sort_pytorch

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::deep_sort_pytorch::Bound_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::deep_sort_pytorch::Bound_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::deep_sort_pytorch::Bound_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
{
  static const char* value()
  {
    return "1a0d13b1a36f23172ad0536b31e11c13";
  }

  static const char* value(const ::deep_sort_pytorch::Bound_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x1a0d13b1a36f2317ULL;
  static const uint64_t static_value2 = 0x2ad0536b31e11c13ULL;
};

template<class ContainerAllocator>
struct DataType< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
{
  static const char* value()
  {
    return "deep_sort_pytorch/Bound";
  }

  static const char* value(const ::deep_sort_pytorch::Bound_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 x1\n"
"float64 x2\n"
"float64 y1\n"
"float64 y2\n"
;
  }

  static const char* value(const ::deep_sort_pytorch::Bound_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.x1);
      stream.next(m.x2);
      stream.next(m.y1);
      stream.next(m.y2);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Bound_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::deep_sort_pytorch::Bound_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::deep_sort_pytorch::Bound_<ContainerAllocator>& v)
  {
    s << indent << "x1: ";
    Printer<double>::stream(s, indent + "  ", v.x1);
    s << indent << "x2: ";
    Printer<double>::stream(s, indent + "  ", v.x2);
    s << indent << "y1: ";
    Printer<double>::stream(s, indent + "  ", v.y1);
    s << indent << "y2: ";
    Printer<double>::stream(s, indent + "  ", v.y2);
  }
};

} // namespace message_operations
} // namespace ros

#endif // DEEP_SORT_PYTORCH_MESSAGE_BOUND_H
