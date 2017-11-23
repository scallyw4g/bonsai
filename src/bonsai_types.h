#ifndef BONSAI_TYPES_H
#define BONSAI_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <assert_types.h>
#include <platform_constants.h>

#include <basic_types.h>
#include <vector_types.h>
#include <line_types.h>
#include <matrix_types.h>
#include <quaternion_types.h>
#include <canonical_position_types.h>
#include <rect_types.h>
#include <render_types.h>
#include <memory_types.h>
#include <platform_types.h>

#include <debug_types.h>
#include <debug_print.h>

#endif
