/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 Types.h
 
 Leland Lucius
 
 Copyright (c) 2014, Audacity Team
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 **********************************************************************/

#pragma once
#include <vector>

typedef std::vector<float> FloatVector;

#include <algorithm>

// ----------------------------------------------------------------------------
// TODO:  I'd imagine this header may be replaced by other public headers. But,
//        to try and minimize more changes to the base code, we can use this
//        until proper public headers are created for the stuff in here.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// A native 64-bit integer...used when referring to any number of samples
// ----------------------------------------------------------------------------

class sampleCount
{
public:
    using type = long long;
    static_assert(sizeof(type) == 8, "Wrong width of sampleCount");
    
    sampleCount () : value { 0 } {}
    
    // Allow implicit conversion from integral types
    sampleCount ( type v ) : value { v } {}
    sampleCount ( unsigned long long v ) : value ( v ) {}
    sampleCount ( int v ) : value { v } {}
    sampleCount ( unsigned v ) : value { v } {}
    sampleCount ( long v ) : value { v } {}
    
    // unsigned long is 64 bit on some platforms.  Let it narrow.
    sampleCount ( unsigned long v ) : value ( v ) {}
    
    // Beware implicit conversions from floating point values!
    // Otherwise the meaning of binary operators with sampleCount change
    // their meaning when sampleCount is not an alias!
    explicit sampleCount ( float f ) : value ( f ) {}
    explicit sampleCount ( double d ) : value ( d ) {}
    
    sampleCount ( const sampleCount& ) = default;
    sampleCount &operator= ( const sampleCount& ) = default;
    
    float as_float() const { return value; }
    double as_double() const { return value; }
    
    long long as_long_long() const { return value; }
    
    size_t as_size_t() const {
        assert(value >= 0);
        assert(value <= std::numeric_limits<size_t>::max());
        return value;
    }
    
    sampleCount &operator += (sampleCount b) { value += b.value; return *this; }
    sampleCount &operator -= (sampleCount b) { value -= b.value; return *this; }
    sampleCount &operator *= (sampleCount b) { value *= b.value; return *this; }
    sampleCount &operator /= (sampleCount b) { value /= b.value; return *this; }
    sampleCount &operator %= (sampleCount b) { value %= b.value; return *this; }
    
    sampleCount operator - () const { return -value; }
    
    sampleCount &operator ++ () { ++value; return *this; }
    sampleCount operator ++ (int)
    { sampleCount result{ *this }; ++value; return result; }
    
    sampleCount &operator -- () { --value; return *this; }
    sampleCount operator -- (int)
    { sampleCount result{ *this }; --value; return result; }
    
private:
    type value;
};

inline bool operator == (sampleCount a, sampleCount b)
{
    return a.as_long_long() == b.as_long_long();
}

inline bool operator != (sampleCount a, sampleCount b)
{
    return !(a == b);
}

inline bool operator < (sampleCount a, sampleCount b)
{
    return a.as_long_long() < b.as_long_long();
}

inline bool operator >= (sampleCount a, sampleCount b)
{
    return !(a < b);
}

inline bool operator > (sampleCount a, sampleCount b)
{
    return b < a;
}

inline bool operator <= (sampleCount a, sampleCount b)
{
    return !(b < a);
}

inline sampleCount operator + (sampleCount a, sampleCount b)
{
    return sampleCount{ a } += b;
}

inline sampleCount operator - (sampleCount a, sampleCount b)
{
    return sampleCount{ a } -= b;
}

inline sampleCount operator * (sampleCount a, sampleCount b)
{
    return sampleCount{ a } *= b;
}

inline sampleCount operator / (sampleCount a, sampleCount b)
{
    return sampleCount{ a } /= b;
}

inline sampleCount operator % (sampleCount a, sampleCount b)
{
    return sampleCount{ a } %= b;
}

// ----------------------------------------------------------------------------
// Function returning the minimum of a sampleCount and a size_t,
// hiding the casts
// ----------------------------------------------------------------------------

inline size_t limitSampleBufferSize( size_t bufferSize, sampleCount limit )
{
    return
    std::min( sampleCount( bufferSize ), std::max( sampleCount(0), limit ) )
    .as_size_t();
}

// ----------------------------------------------------------------------------
// Supported sample formats
// ----------------------------------------------------------------------------
typedef enum
{
    int16Sample = 0x00020001,
    int24Sample = 0x00040001,
    floatSample = 0x0004000F
} sampleFormat;

// ----------------------------------------------------------------------------
// Provide the number of bytes a specific sample will take
// ----------------------------------------------------------------------------
#define SAMPLE_SIZE(SampleFormat) (SampleFormat >> 16)

// ----------------------------------------------------------------------------
// Generic pointer to sample data
// ----------------------------------------------------------------------------
typedef char *samplePtr;
typedef const char *constSamplePtr;
