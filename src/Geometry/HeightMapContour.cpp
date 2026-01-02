#include <hgl/math/geometry/HeightMapContour.h>

namespace hgl::math
{
    // Template instantiations for commonly used types
    template class HeightMapContourExtractor<uint8_t>;
    template class HeightMapContourExtractor<uint16_t>;
    template class HeightMapContourExtractor<uint32_t>;
    template class HeightMapContourExtractor<float>;

}//namespace hgl::math
