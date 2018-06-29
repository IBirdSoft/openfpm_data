// Copyright 2002 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Boost.MultiArray Library
//  Authors: Ronald Garcia
//           Jeremy Siek
//           Andrew Lumsdaine
//  See http://www.boost.org/libs/multi_array for documentation.

#ifndef SUBARRAY_RG071801_OPENFPM_HPP
#define SUBARRAY_RG071801_OPENFPM_HPP

//
// subarray.hpp - used to implement standard operator[] on
// multi_arrays
//

#include "boost/multi_array/concept_checks.hpp"
#include "boost/limits.hpp"
#include "boost/type.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
#include "util/boost/boost_multi_array_base_openfpm.hpp"
#include "util/cuda_util.hpp"

namespace boost {
namespace detail {
namespace multi_array {

//
// const_sub_array
//    multi_array's proxy class to allow multiple overloads of
//    operator[] in order to provide a clean multi-dimensional array 
//    interface.
template <typename T, std::size_t NumDims, typename TPtr>
class const_sub_array_openfpm :
  public boost::detail::multi_array::multi_array_impl_base_openfpm<T,NumDims>
{
  typedef boost::detail::multi_array::multi_array_impl_base_openfpm<T,NumDims> super_type;
public: 
  typedef typename super_type::value_type value_type;
  typedef typename super_type::const_reference const_reference;
  typedef typename super_type::const_iterator const_iterator;
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;
  typedef typename super_type::element element;
  typedef typename super_type::size_type size_type;
  typedef typename super_type::difference_type difference_type;
  typedef typename super_type::index index;
  typedef typename super_type::extent_range extent_range;

  // template typedefs
  template <std::size_t NDims>
  struct const_array_view {
    typedef boost::detail::multi_array::const_multi_array_view_openfpm<T,NDims> type;
  };

  template <std::size_t NDims>
  struct array_view {
    typedef boost::detail::multi_array::multi_array_view_openfpm<T,NDims> type;
  };

  // Allow default copy constructor as well.

  template <typename OPtr>
  const_sub_array_openfpm (const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) :
    base_(rhs.base_), extents_(rhs.extents_), strides_(rhs.strides_),
    index_base_(rhs.index_base_) {
  }

  // const_sub_array always returns const types, regardless of its own
  // constness.
  __device__ __host__  const_reference operator[](index idx) const {
    return super_type::access(boost::type<const_reference>(),
                              idx,base_,shape(),strides(),index_bases());
  }
  
  template <typename IndexList>
  const element& operator()(const IndexList& indices) const {
    boost::function_requires<
      CollectionConcept<IndexList> >();
    return super_type::access_element(boost::type<const element&>(),
                                      indices,origin(),
                                      shape(),strides(),index_bases());
  }

  // see generate_array_view in base.hpp
  template <int NDims>
  __device__ __host__   typename const_array_view<NDims>::type
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices)
    const {
    typedef typename const_array_view<NDims>::type return_type;
    return
      super_type::generate_array_view(boost::type<return_type>(),
                                      indices,
                                      shape(),
                                      strides(),
                                      index_bases(),
                                      base_);
  }

  template <typename OPtr>
  bool operator<(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    return std::lexicographical_compare(begin(),end(),rhs.begin(),rhs.end());
  }

  template <typename OPtr>
  bool operator==(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    if(std::equal(shape(),shape()+num_dimensions(),rhs.shape()))
      return std::equal(begin(),end(),rhs.begin());
    else return false;
  }

  template <typename OPtr>
  bool operator!=(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    return !(*this == rhs);
  }

  template <typename OPtr>
  bool operator>(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    return rhs < *this;
  }

  template <typename OPtr>
  bool operator<=(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    return !(*this > rhs);
  }

  template <typename OPtr>
  bool operator>=(const const_sub_array_openfpm<T,NumDims,OPtr>& rhs) const {
    return !(*this < rhs);
  }

  const_iterator begin() const {
    return const_iterator(*index_bases(),origin(),
                          shape(),strides(),index_bases());
  }

  const_iterator end() const {
    return const_iterator(*index_bases()+(index)*shape(),origin(),
                          shape(),strides(),index_bases());
  }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  TPtr origin() const { return base_; }
  __host__ __device__ size_type size() const { return extents_[0]; }
  size_type max_size() const { return num_elements(); }
  bool empty() const { return size() == 0; }
  size_type num_dimensions() const { return NumDims; }
  __host__ __device__ const size_type* shape() const { return extents_; }
  __host__ __device__ const index* strides() const { return strides_; }
  __host__ __device__ const index* index_bases() const { return index_base_; }

  size_type num_elements() const { 
    return std::accumulate(shape(),shape() + num_dimensions(),
                           size_type(1), std::multiplies<size_type>());
  }


#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
protected:
  template <typename,std::size_t> friend class value_accessor_n_openfpm;
  template <typename,std::size_t,typename> friend class const_sub_array_openfpm;
#else    
public:  // Should be protected
#endif

  __device__ __host__ const_sub_array_openfpm (TPtr base,
                 const size_type* extents,
                 const index* strides,
                 const index* index_base) :
    base_(base), extents_(extents), strides_(strides),
    index_base_(index_base) {
  }

  TPtr base_;
  const size_type* extents_;
  const index* strides_;
  const index* index_base_;
private:
  // const_sub_array cannot be assigned to (no deep copies!)
  const_sub_array_openfpm& operator=(const const_sub_array_openfpm&);
};

template<unsigned int NumDims>
struct print_debug
{
	template<typename T, typename T2> static void print(const T & obj1, const T2 & obj2)
	{
		printf("HELLO 1 \n");
	}
};

template<>
struct print_debug<2>
{
	template<typename T, typename T2> static void print(T && obj1,const T2 & obj2)
	{
		float * ptr = obj1.origin();
		printf("ORIGIN DST: %p \n",ptr);

		const float * ptr2 = obj2.origin();
		printf("ORIGIN SRC: %p \n",ptr2);

		obj1 = obj2;
	}
};

//
// sub_array
//    multi_array's proxy class to allow multiple overloads of
//    operator[] in order to provide a clean multi-dimensional array 
//    interface.
template <typename T, std::size_t NumDims>
class sub_array_openfpm : public const_sub_array_openfpm<T,NumDims,T*>
{
  typedef const_sub_array_openfpm<T,NumDims,T*> super_type;
public: 
  typedef typename super_type::element element;
  typedef typename super_type::reference reference;
  typedef typename super_type::index index;
  typedef typename super_type::size_type size_type;
  typedef typename super_type::iterator iterator;
  typedef typename super_type::reverse_iterator reverse_iterator;
  typedef typename super_type::const_reference const_reference;
  typedef typename super_type::const_iterator const_iterator;
  typedef typename super_type::const_reverse_iterator const_reverse_iterator;

  // template typedefs
  template <std::size_t NDims>
  struct const_array_view {
    typedef boost::detail::multi_array::const_multi_array_view_openfpm<T,NDims> type;
  };

  template <std::size_t NDims>
  struct array_view {
    typedef boost::detail::multi_array::multi_array_view_openfpm<T,NDims> type;
  };

  // Assignment from other ConstMultiArray types.
  template <typename ConstMultiArray>
  sub_array_openfpm& operator=(const ConstMultiArray& other) {

#ifdef SE_CLASS1
    function_requires< boost::multi_array_concepts::ConstMultiArrayConcept< 
        ConstMultiArray, NumDims> >();

    // make sure the dimensions agree
    BOOST_ASSERT(other.num_dimensions() == this->num_dimensions());
//    BOOST_ASSERT(std::equal(other.shape(),other.shape()+this->num_dimensions(),
//                            this->shape()));

#endif
    // iterator-based copy
//    std::copy(other.begin(),other.end(),begin());
      printf("CHECK: %p \n",other.origin());
      printf("CHECK SIZE: %p \n",other.size());

      this->operator[](0) = other[0];
      this->operator[](1) = other[1];
      this->operator[](2) = other[2];

//      int temp = other.size();
//	  for (int i = 0 ; i < (int)temp ; i++) {}
//	  {/*this->operator[](i) = other[i];*/}
    return *this;
  }

  __device__ __host__ sub_array_openfpm& copy_secondary(const sub_array_openfpm& other) {
    if (&other != this) {
#ifdef SE_CLASS1
      // make sure the dimensions agree
      BOOST_ASSERT(other.num_dimensions() == this->num_dimensions());
//      BOOST_ASSERT(std::equal(other.shape(),
//                              other.shape()+this->num_dimensions(),
//                              this->shape()));
#endif
      // iterator-based copy
      //std::copy(other.begin(),other.end(),begin());

    	  for (int i = 0 ; i < (int)other.size() ; i++)
    	  {this->operator[](i) = other[i];}
    }
    return *this;
  }


  __device__ __host__ sub_array_openfpm& operator=(const sub_array_openfpm& other) {
    if (&other != this) {
#ifdef SE_CLASS1
      // make sure the dimensions agree
      BOOST_ASSERT(other.num_dimensions() == this->num_dimensions());
//      BOOST_ASSERT(std::equal(other.shape(),
//                              other.shape()+this->num_dimensions(),
//                              this->shape()));
#endif
      // iterator-based copy
      //std::copy(other.begin(),other.end(),begin());

      if (this->num_dimensions() < 2)
      {
    	  for (int i = 0 ; i < (int)other.size() ; i++)
    	  {this->operator[](i) = other[i];}
      }
      else
      {
    	  const T * test1 = other.origin();
    	  const T * test2 = this->origin();

    	  printf("ORIGIN: %p DESTINATION: %p \n",test1,test2);
    	  printf("S0: %d S1: %d \n",other.strides()[0],other.strides()[1]);
    	  printf("S_0: %d S_1: %d \n",this->strides()[0],this->strides()[1]);
    	  printf("S0_p: %p S1_p: %p \n",&other.strides()[0],&other.strides()[1]);
    	  printf("dims %d \n",other.num_dimensions());

    	  auto a = other[0];

    	  print_debug<NumDims>::print(this->operator[](1),other[1]);
      }
    }
    return *this;
  }

  __device__ __host__ T* origin() { return this->base_; }
  __device__ __host__ const T* origin() const { return this->base_; }

  __device__ __host__ reference operator[](index idx) {
    return super_type::access(boost::type<reference>(),
                              idx,this->base_,this->shape(),this->strides(),
                              this->index_bases());
  }

  // see generate_array_view in base.hpp
  template <int NDims>
  __device__ __host__ typename array_view<NDims>::type
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices) {
    typedef typename array_view<NDims>::type return_type;

    return
      super_type::generate_array_view(boost::type<return_type>(),
                                      indices,
                                      this->shape(),
                                      this->strides(),
                                      this->index_bases(),
                                      origin());
  }

  template <class IndexList>
  element& operator()(const IndexList& indices) {
    boost::function_requires<
      CollectionConcept<IndexList> >();
    return super_type::access_element(boost::type<element&>(),
                                      indices,origin(),
                                      this->shape(),this->strides(),
                                      this->index_bases());
  }

  __device__ __host__ iterator begin()
  {
    return iterator(*this->index_bases(),origin(),
                    this->shape(),this->strides(),this->index_bases());
  }

  __device__ __host__ iterator end()
  {
    return iterator(*this->index_bases()+(index)*this->shape(),origin(),
                    this->shape(),this->strides(),this->index_bases());
  }

  // RG - rbegin() and rend() written naively to thwart MSVC ICE.
  reverse_iterator rbegin() {
    reverse_iterator ri(end());
    return ri;
  }

  reverse_iterator rend() {
    reverse_iterator ri(begin());
    return ri;
  }

  //
  // proxies
  //

  template <class IndexList>
  const element& operator()(const IndexList& indices) const {
    boost::function_requires<
      CollectionConcept<IndexList> >();
    return super_type::operator()(indices);
  }

  const_reference operator[](index idx) const {
    return super_type::operator[](idx);
  }

  // see generate_array_view in base.hpp
  template <int NDims>
  typename const_array_view<NDims>::type 
  operator[](const boost::detail::multi_array::
             index_gen<NumDims,NDims>& indices)
    const {
    return super_type::operator[](indices);
  }

  const_iterator begin() const {
    return super_type::begin();
  }
  
  const_iterator end() const {
    return super_type::end();
  }

  const_reverse_iterator rbegin() const {
    return super_type::rbegin();
  }

  const_reverse_iterator rend() const {
    return super_type::rend();
  }

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
private:
  template <typename,std::size_t> friend class value_accessor_n_openfpm;
#else
public: // should be private
#endif

  __device__ __host__ sub_array_openfpm (T* base,
            const size_type* extents,
            const index* strides,
            const index* index_base) :
    super_type(base,extents,strides,index_base) {
  }

};

} // namespace multi_array
} // namespace detail
//
// traits classes to get sub_array types
//
template <typename Array, int N>
class subarray_gen_openfpm {
  typedef typename Array::element element;
public:
  typedef boost::detail::multi_array::sub_array_openfpm<element,N> type;
};

template <typename Array, int N>
class const_subarray_gen_openfpm {
  typedef typename Array::element element;
public:
  typedef boost::detail::multi_array::const_sub_array_openfpm<element,N> type;
};
} // namespace boost
  
#endif // SUBARRAY_RG071801_HPP