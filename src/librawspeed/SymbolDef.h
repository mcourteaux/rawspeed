#pragma once


#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning( disable: 4251 )
#endif
#ifdef RAWSPEED_NODLL
# define RAWSPEED_API
#else
# ifdef RAWSPEED_BUILDLIB
#    define RAWSPEED_API   __declspec( dllexport )
# else
#    define RAWSPEED_API   __declspec( dllimport )
# endif
#endif
#else
#  define RAWSPEED_API
#endif
