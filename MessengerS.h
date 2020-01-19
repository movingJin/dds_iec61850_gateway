// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.2a_p15
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// be/be_codegen.cpp:461

#ifndef _TAO_IDL_MESSENGERS_ADVGDG_H_
#define _TAO_IDL_MESSENGERS_ADVGDG_H_

#include /**/ "ace/pre.h"

#include "MessengerC.h"
#include "tao/PortableServer/Basic_SArguments.h"
#include "tao/PortableServer/Special_Basic_SArguments.h"
#include "tao/PortableServer/BD_String_SArgument_T.h"
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/UB_String_SArguments.h"
#include "tao/PortableServer/Fixed_Array_SArgument_T.h"
#include "tao/PortableServer/Var_Array_SArgument_T.h"

// Skeleton file generation suppressed with command line option -SS

// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:68

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// Arg traits specializations.
namespace TAO
{

  template<>
  class SArg_Traits< ::StringArray_tag>
    : public
        Var_Array_SArg_Traits_T<
            ::StringArray_out,
            ::StringArray_forany,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

#if !defined (_NAMETYPE128__SARG_TRAITS_)
#define _NAMETYPE128__SARG_TRAITS_

  template<>
  class SArg_Traits<NameType_128>
    : public
        BD_String_SArg_Traits_T<
            CORBA::String_var,
            128,
            TAO::Any_Insert_Policy_Noop
        >
  {
  };

#endif /* end #if !defined */

  // TAO_IDL - Generated from
  // be/be_visitor_arg_traits.cpp:685

  template<>
  class SArg_Traits< ::BinaryContentsType>
    : public
        Var_Size_SArg_Traits_T<
            ::BinaryContentsType,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be/be_visitor_arg_traits.cpp:947

  template<>
  class SArg_Traits< ::Messenger::timespec>
    : public
        Fixed_Size_SArg_Traits_T<
            ::Messenger::timespec,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };

  // TAO_IDL - Generated from
  // be/be_visitor_arg_traits.cpp:947

  template<>
  class SArg_Traits< ::Messenger::Message>
    : public
        Var_Size_SArg_Traits_T<
            ::Messenger::Message,
            TAO::Any_Insert_Policy_Noop
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL



#include /**/ "ace/post.h"

#endif /* ifndef */

