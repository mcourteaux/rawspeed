FILE(GLOB COMMON_SOURCES
  "Common.cpp"
  "Common.h"
  "Point.h"
  "RawImage.cpp"
  "RawImage.h"
  "RawImageDataFloat.cpp"
  "RawImageDataU16.cpp"
  "DngOpcodes.cpp"
  "DngOpcodes.h"
  "Threading.h"
)

set(RAWSPEED_SOURCES "${RAWSPEED_SOURCES};${COMMON_SOURCES}" PARENT_SCOPE)