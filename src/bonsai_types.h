void BreakHere() { return; }

#include <assert_types.h>

#ifdef _WIN32
#include <win32_platform.h>
#else
#include <unix_platform.h>
#endif

#include <platform_constants.h>

#include <basic_types.h>
#include <memory_types.h>
#include <vector_types.h>
#include <colors.h>
#include <line_types.h>
#include <matrix_types.h>
#include <quaternion_types.h>
#include <canonical_position_types.h>
#include <rect_types.h>
#include <render_types.h>
#include <graphics_types.h>
#include <platform_types.h>

#include <string.h> // TODO(Jesse): Get rid of this shit!!

#include <debug_types.h>
#include <debug_print.h>

#include <bonsai_vertex.h>
#include <net/network.h>
#include <net/server.h>
