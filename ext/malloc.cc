/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007, 2008, 2009   Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include "error.hh"
#include "malloc.hh"

#include <iostream>
using namespace std;

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#define DLLLOCAL
#else
#define DLLEXPORT __attribute__ ((visibility("default")))
#define DLLLOCAL __attribute__ ((visibility("hidden")))
#endif

VALUE Malloc::cRubyClass = Qnil;

VALUE Malloc::init(void)
{
  cRubyClass = rb_define_class( "Malloc", rb_cObject );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( mallocNew ), 1 );
  rb_define_method( cRubyClass, "+",
                    RUBY_METHOD_FUNC( mallocPlus ), 1 );
  rb_define_method( cRubyClass, "read",
                    RUBY_METHOD_FUNC( mallocRead ), 1 );
  rb_define_method( cRubyClass, "write",
                    RUBY_METHOD_FUNC( mallocWrite ), 1 );
  return cRubyClass;
}

VALUE Malloc::mallocNew( VALUE rbClass, VALUE rbSize )
{
  VALUE retVal = Qnil;
  try {
    unsigned int size = NUM2UINT( rbSize );
    char *self = ALLOC_N( char, size );
    ERRORMACRO( self != NULL, Error, , "Failed to allocate " << size
                << " bytes of memory" );
    retVal = Data_Wrap_Struct( rbClass, 0, xfree, (void *)self );
  } catch( std::exception &e ) {
    rb_raise( rb_eRuntimeError, e.what() );
  };
  return retVal;
}

VALUE Malloc::mallocPlus( VALUE rbSelf, VALUE rbOffset )
{
  VALUE retVal = Qnil;
  try {
    char *self; Data_Get_Struct( rbSelf, char, self );
    int offset = NUM2INT( rbOffset );
    ERRORMACRO( offset >= 0, Error, , "Offset must be non-negative (but was "
                << offset << ')' );
    retVal = Data_Wrap_Struct( cRubyClass, 0, 0, (void *)( self + offset ) );
  } catch( std::exception &e ) {
    rb_raise( rb_eRuntimeError, e.what() );
  };
  return retVal;
}

VALUE Malloc::mallocRead( VALUE rbSelf, VALUE rbLength )
{
  char *self; Data_Get_Struct( rbSelf, char, self );
  unsigned int length = NUM2UINT( rbLength );
  return rb_str_new( self, length );
}

VALUE Malloc::mallocWrite( VALUE rbSelf, VALUE rbString )
{
  char *self; Data_Get_Struct( rbSelf, char, self );
  memcpy( self, StringValuePtr( rbString ), RSTRING_LEN( rbString ) );
  return rbString;
}
