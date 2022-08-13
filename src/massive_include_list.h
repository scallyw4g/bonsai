#if BONSAI_PREPROCESSOR

#define __FLOAT_H

#include <aio.h>
#include <aliases.h>
#include <alloca.h>
#include <argp.h>
#include <argz.h>
#include <ar.h>
#include <assert.h>
#include <builtin_types.h>
#include <byteswap.h>
#include <channel_descriptor.h>
#include <common_functions.h>
/* #include <complex.h> */

// NOTE(Jesse): breaks on namespace keyword
/* #include <cooperative_groups.h> */
/* #include <cooperative_groups_helpers.h> */

#include <cpio.h>
#include <crypt.h>
#include <ctype.h>

// NOTE(JEsse): Not sure what these are
/* #include <cublas_api.h> */
/* #include <cublas.h> */
/* #include <cublas_v2.h> */
/* #include <cublasXt.h> */


#include <host_defines.h>

// NOTE(Jesse): these expose some (seemingly) pretty serious bugs..
#if 0
// NOTE(Jesse): These are special keywords defined by nvcc (the cuda compiler)
#define __host__
#define __device__

#include <cuComplex.h>
#include <cuda.h>
#include <cuda_device_runtime_api.h>
#include <cudaEGL.h>
/* #include <cuda_egl_interop.h> */
#include <cuda_fp16.h>
#include <cuda_fp16.hpp>
#include <cudaGL.h>
/* #include <cuda_gl_interop.h> */
#include <cudalibxt.h>
#include <cuda_occupancy.h>
#include <cuda_profiler_api.h>
#include <cudaProfiler.h>
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#include <cuda_surface_types.h>
#include <cuda_texture_types.h>
#include <cudaVDPAU.h>
#include <cuda_vdpau_interop.h>
#endif

// NOTE(Jesse): More CUDA stuff
#if 0
#include <cufft.h>
#include <cufftw.h>
#include <cufftXt.h>

#include <curand_discrete2.h>
#include <curand_discrete.h>
#include <curand_globals.h>
#include <curand.h>
#include <curand_kernel.h>
#include <curand_lognormal.h>
#include <curand_mrg32k3a.h>
#include <curand_mtgp32dc_p_11213.h>
#include <curand_mtgp32.h>
#include <curand_mtgp32_host.h>
#include <curand_mtgp32_kernel.h>
#include <curand_normal.h>
#include <curand_normal_static.h>
#include <curand_philox4x32_x.h>
#include <curand_poisson.h>
#include <curand_precalc.h>
#include <curand_uniform.h>
#include <cusolver_common.h>
#include <cusolverDn.h>
#include <cusolverRf.h>
#include <cusolverSp.h>
#include <cusolverSp_LOWLEVEL_PREVIEW.h>
#include <cusparse.h>
#include <cusparse_v2.h>
#endif

// NOTE(Jesse): More CUDA stuff
#if 0
#include <device_atomic_functions.h>
#include <device_atomic_functions.hpp>
#include <device_double_functions.h>
#include <device_functions.h>
#include <device_launch_parameters.h>
#include <device_types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <driver_functions.h>
#include <driver_types.h>
#include <dynlink_cuda_cuda.h>
#include <dynlink_cuda.h>
#include <dynlink_cuviddec.h>
#include <dynlink_nvcuvid.h>
#endif

#include <elf.h>
#include <endian.h>
#include <envz.h>
#include <err.h>
#include <errno.h>
#include <error.h>
#include <execinfo.h>
#include <expat_external.h>
#include <expat.h>
#include <fatBinaryCtl.h>
#include <fatbinary.h>
#include <fcntl.h>
#include <features.h>
#include <fenv.h>
#include <fmtmsg.h>
#include <fnmatch.h>
#include <fstab.h>
#include <fts.h>
#include <ftw.h>
#include <gawkapi.h>
#include <_G_config.h>
#include <gconv.h>
#include <getopt.h>
#include <glob.h>
#include <gnumake.h>
#include <gnu-versions.h>
/* #include <graphite2> */ // NOTE(Jesse): Buggy
#include <grp.h>
#include <gshadow.h>
#include <host_config.h>
#include <iconv.h>
/* #include <icu-le-hb> */
#include <ifaddrs.h>
#include <inttypes.h>
#include <langinfo.h>
#include <lastlog.h>
#include <libgen.h>
#include <libintl.h>
#include <libio.h>
#include <library_types.h>
#include <libsync.h>
/* #include <limits.h> */
#include <link.h>
#include <locale.h>
#include <malloc.h>
#include <math_constants.h>
#include <math_functions.h>
#include <math.h>
#include <mcheck.h>
#include <memory.h>
#include <mma.h>
#include <mntent.h>
#include <monetary.h>
#include <mqueue.h>
/* #include <netash> */
#include <netdb.h>
#include <nl_types.h>
#include <nppcore.h>
#include <nppdefs.h>
#include <npp.h>
#include <nppi_arithmetic_and_logical_operations.h>
#include <nppi_color_conversion.h>
#include <nppi_compression_functions.h>
#include <nppi_computer_vision.h>
#include <nppi_data_exchange_and_initialization.h>
#include <nppi_filtering_functions.h>
#include <nppi_geometry_transforms.h>
#include <nppi.h>
#include <nppi_linear_transforms.h>
#include <nppi_morphological_operations.h>
#include <nppi_statistics_functions.h>
#include <nppi_support_functions.h>
#include <nppi_threshold_and_compare_operations.h>
#include <npps_arithmetic_and_logical_operations.h>
#include <npps_conversion_functions.h>
#include <npps_filtering_functions.h>
#include <npps.h>
#include <npps_initialization.h>
#include <npps_statistics_functions.h>
#include <npps_support_functions.h>
#include <nppversion.h>
#include <nss.h>
#include <nvblas.h>
#include <nvgraph.h>
#include <nvml.h>
#include <nvrtc.h>
#include <nvToolsExtCuda.h>
#include <nvToolsExtCudaRt.h>
#include <nvToolsExt.h>
#include <nvToolsExtMeta.h>
#include <nvToolsExtSync.h>
#include <obstack.h>
#include <omp.h>
#include <paths.h>
#include <pcrecpparg.h>
#include <pcrecpp.h>
#include <pcre.h>
#include <pcreposix.h>
#include <pcre_scanner.h>
#include <pcre_stringpiece.h>
#include <poll.h>
#include <printf.h>
#include <proc_service.h>
#include <pthread.h>
#include <pty.h>
#include <pwd.h>
/* #include <python2.7> */
/* #include <python3.6> */
/* #include <python3.6m> */
#include <re_comp.h>
#include <regex.h>
#include <regexp.h>
#include <resolv.h>
#include <sched.h>
#include <search.h>
#include <semaphore.h>
#include <setjmp.h>
#include <sgtty.h>
#include <shadow.h>
#include <signal.h>
#include <sm_20_atomic_functions.h>
#include <sm_20_atomic_functions.hpp>
#include <sm_20_intrinsics.h>
#include <sm_20_intrinsics.hpp>
#include <sm_30_intrinsics.h>
#include <sm_30_intrinsics.hpp>
#include <sm_32_atomic_functions.h>
#include <sm_32_atomic_functions.hpp>
#include <sm_32_intrinsics.h>
#include <sm_32_intrinsics.hpp>
#include <sm_35_atomic_functions.h>
#include <sm_35_intrinsics.h>
#include <sm_60_atomic_functions.h>
#include <sm_60_atomic_functions.hpp>
#include <sm_61_intrinsics.h>
#include <sm_61_intrinsics.hpp>
/* #include <sobol_direction_vectors.h> */
#include <spawn.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <stab.h>
#include <stdc-predef.h>
#include <stdint.h>
#include <stdio_ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stropts.h>
#include <sudo_plugin.h>
#include <surface_functions.h>
#include <surface_functions.hpp>
#include <surface_indirect_functions.h>
#include <surface_indirect_functions.hpp>
#include <surface_types.h>
#include <syscall.h>
#include <sysexits.h>
#include <syslog.h>
#include <tar.h>
#include <termio.h>
#include <termios.h>
#include <texture_fetch_functions.h>
#include <texture_fetch_functions.hpp>
#include <texture_indirect_functions.h>
#include <texture_indirect_functions.hpp>
#include <texture_types.h>
#include <tgmath.h>
#include <thread_db.h>
/* #include <thrust> */
#include <time.h>
#include <ttyent.h>
#include <uchar.h>
#include <ucontext.h>
#include <ulimit.h>
#include <unistd.h>
#include <ustat.h>
#include <utime.h>
#include <utmp.h>
#include <utmpx.h>
#include <uv-errno.h>
/* #include <uv.h> */
#include <uv-linux.h>
#include <uv-threadpool.h>
#include <uv-unix.h>
#include <uv-version.h>
#include <values.h>
#include <vector_functions.h>
#include <vector_functions.hpp>
#include <vector_types.h>
#include <wait.h>
#include <wchar.h>
#include <wctype.h>
#include <wordexp.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <yaml.h>
#include <zconf.h>
#include <zlib.h>

#endif
