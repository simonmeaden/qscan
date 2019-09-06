/*

The MIT License (MIT)

Copyright (c) 2015 Martin Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef TWPP_DETAIL_FILE_CURVERESPONSE_HPP
#define TWPP_DETAIL_FILE_CURVERESPONSE_HPP

#include "../twpp.hpp"

namespace Twpp {

namespace Detail {

/// Base response class.
class CurveResponse {

public:
    CurveResponse() noexcept{}

    /// Creates a new (Rgb|Gray)Response with default elements, make sure that info.bitsPerPixel() is <= 8.
    /// \throw RangeException When info.bitsPerPixel() is negative or greater than 8.
    explicit CurveResponse(const ImageInfo& info) : CurveResponse(info.bitsPerPixel()){}

    /// Creates a new (Rgb|Gray)Response with default elements, make sure that bitsPerPixel is <= 8.
    /// \throw RangeException When bitsPerPixel is negative or greater than 8.
    explicit CurveResponse(Int16 bitsPerPixel){
        if (bitsPerPixel <= 0 || bitsPerPixel > 8){
            throw RangeException();
        }

        UInt16 size = 1 << static_cast<UInt16>(bitsPerPixel);
        m_data.reset(new Element8[size]);
        for (UInt16 i = 0; i < size; i++){
            m_data[i] = Element8(static_cast<UInt8>(i)); // 0..255 max
        }
    }

    /// Array of size `2^ImageInfo.bitesPerPixel()`, up to 256 elements (2^8)
    Element8* data() noexcept{
        return m_data.get();
    }

    /// Array of size `2^ImageInfo.bitesPerPixel()`, up to 256 elements (2^8)
    const Element8* data() const noexcept{
        return m_data.get();
    }

private:
    std::unique_ptr<Element8[]> m_data;

};

}

/// Rgb response class.
class RgbResponse : public Detail::CurveResponse {

public:
    RgbResponse() noexcept{}

    /// Creates a new RgbResponse with default elements, make sure that info.bitesPerPixel() is <= 8.
    /// \throw RangeException When info.bitsPerPixel() is negative or greater than 8.
    explicit RgbResponse(const ImageInfo& info) :
        Detail::CurveResponse(info) {}

    /// Creates a new RgbResponse with default elements, make sure that bitsPerPixel is <= 8.
    /// \throw RangeException When bitsPerPixel is negative or greater than 8.
    explicit RgbResponse(Int16 bitsPerPixel) :
        Detail::CurveResponse(bitsPerPixel) {}

};

/// Gray response class.
class GrayResponse : public Detail::CurveResponse {

public:
    GrayResponse() noexcept{}

    /// Creates a new GrayResponse with default elements, make sure that info.bitesPerPixel() is <= 8.
    /// \throw RangeException When info.bitsPerPixel() is negative or greater than 8.
    explicit GrayResponse(const ImageInfo& info) :
        Detail::CurveResponse(info) {}

    /// Creates a new GrayResponse with default elements, make sure that bitsPerPixel is <= 8.
    /// \throw RangeException When bitsPerPixel is negative or greater than 8.
    explicit GrayResponse(Int16 bitsPerPixel) :
        Detail::CurveResponse(bitsPerPixel) {}

};

}

#endif // TWPP_DETAIL_FILE_CURVERESPONSE_HPP

