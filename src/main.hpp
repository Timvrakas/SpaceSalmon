#pragma once

#ifdef GUPPY
    #include "guppy/System.hpp"
#endif

#ifdef SALMON
    #include "salmon/System.hpp"
#endif

#ifdef QUAIL
    #include "quail/System.hpp"
#endif

#ifdef CHOVY
    #include "chovy/System.hpp"
#endif

#ifdef GUPPY_GROUND
    #include "guppy-ground/System.hpp"
#endif

#ifdef QUAIL_GROUND
    #include "quail-ground/System.hpp"
#endif

#ifdef DUMMY
    #include "dummy/System.hpp"
#endif

extern System sys;
extern const char* build_version;