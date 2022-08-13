/* #include <linux/ftrace.h> */
/* #include <linux/getcpu.h> */

// TODO(Jesse id: 366): Can this go in the posix platform?  How do we open dlls?
// dlopen
#include <dlfcn.h>

// X11 typedefs Cursor to some internal type.. and I want it, so we're hijacking it here.
// https://stackoverflow.com/questions/25867905/how-could-i-temporarily-un-typedef-something
//
// Could instead shove these includes in a namespace but.. meh.
#define Cursor X11_Please_Stahp
#include <X11/X.h>
#include <X11/Xlib.h>
#undef Cursor

#define XK_LATIN1 1
#define XK_MISCELLANY 1
#include <X11/keysymdef.h>

// X11 defines "Success" to 0, which is really annoying
#ifdef Success
#undef Success
#endif

// Backtrace
#include <errno.h>
#include <execinfo.h>
#include <unistd.h>

#define RED_TERMINAL "\x1b[31m"
#define BLUE_TERMINAL "\x1b[34m"
#define PURPLE_TERMINAL "\x1b[35m"
#define GREEN_TERMINAL "\x1b[32m"
#define YELLOW_TERMINAL "\x1b[33m"
#define WHITE_TERMINAL "\x1b[37m"

#define PLATFORM_RUNTIME_LIB_EXTENSION ".so"

#define link_external extern "C"

#define WindowEventMasks StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask


//
// GLX
//

#define GLX_RGBA           4
#define GLX_DOUBLEBUFFER   5
#define GLX_DEPTH_SIZE     12

#define GLX_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GLX_CONTEXT_FLAGS_ARB             0x2094
#define GLX_CONTEXT_PROFILE_MASK_ARB      0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define GLX_CONTEXT_DEBUG_BIT_ARB         0x00000001

typedef struct {
  Visual *visual;
  VisualID visualid;
  s32 screen;
  s32 depth;
  s32 c_class;
  u64 red_mask;
  u64 green_mask;
  u64 blue_mask;
  s32 colormap_size;
  s32 bits_per_rgb;
} XVisualInfo;


typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXPixmap;
typedef XID GLXDrawable;
/* GLX 1.3 and later */
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXFBConfigID;
typedef XID GLXContextID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;

link_external XVisualInfo* glXChooseVisual( Display *dpy, s32 screen, s32 *attribList );
link_external GLXFBConfig* glXChooseFBConfig( Display *dpy, s32 screen, const s32 *attribList, s32 *nitems );
link_external void (*glXGetProcAddress(const u8 *procname))( void );
link_external Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx);
link_external void glXSwapBuffers( Display *dpy, GLXDrawable drawable );

typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const s32 *attrib_list);

typedef void ( *PFNGLXSWAPINTERVALEXTPROC) (Display *dpy, GLXDrawable drawable, s32 s32);
typedef PFNGLXSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;



//
// GLX
//



typedef Display* display;
typedef GLXContext gl_context;
typedef Window window;

inline u64
GetCycleCount()
{
  u64 Result = __rdtsc();
  return Result;
}

void
PlatformDebugStacktrace()
{
  void *StackSymbols[32];
  s32 SymbolCount = backtrace(StackSymbols, 32);
  backtrace_symbols_fd(StackSymbols, SymbolCount, STDERR_FILENO);
  return;
}

struct os
{
  window Window;
  display Display;
  gl_context GlContext;

  b32 ContinueRunning = True;
};

inline b32
fopen_s(FILE **HandleOut, const char *FilePath, const char *Permissions)
{
  *HandleOut = fopen(FilePath, Permissions);
  b32 Result = *HandleOut != 0;
  return Result;
}

inline s32
_chdir(const char* DirName)
{
  s32 Result = chdir(DirName);
  return Result;
}

link_internal void*
PlatformGetGlFunction(const char* Name)
{
  void *Result = (void*)glXGetProcAddress((const u8*)Name);
  if (!Result)
  {
    Error("Couldn't load Opengl fucntion (%s)", Name);
  }

  return Result;
}


// Errno cases and error descriptions taken from : https://man7.org/linux/man-pages/man3/errno.3.html
link_external const char*
ErrnoToString(s32 Error)
{
  const char* Result = 0;
  switch(Error)
  {
    case E2BIG:
    {
      Result = "E2BIG : Argument list too long (POSIX.1-2001).";
      break;
    }

    case EACCES:
    {
      Result = "EACCES : Permission denied (POSIX.1-2001).";
      break;
    }

    case EADDRINUSE:
    {
      Result = "EADDRINUSE : Address already in use (POSIX.1-2001).";
      break;
    }

    case EADDRNOTAVAIL:
    {
      Result = "EADDRNOTAVAIL : Address not available (POSIX.1-2001).";
      break;
    }

    case EAFNOSUPPORT:
    {
      Result = "EAFNOSUPPORT : Address family not supported (POSIX.1-2001).";
      break;
    }

    case EAGAIN:
    {
      CAssert( EAGAIN == EWOULDBLOCK );
      Result = "EAGAIN : Resource temporarily unavailable (may be the same value as EWOULDBLOCK) (POSIX.1-2001).";
      break;
    }

    case EALREADY:
    {
      Result = "EALREADY : Connection already in progress (POSIX.1-2001).";
      break;
    }

    case EBADE:
    {
      Result = "EBADE : Invalid exchange.";
      break;
    }

    case EBADF:
    {
      Result = "EBADF : Bad file descriptor (POSIX.1-2001).";
      break;
    }

    case EBADFD:
    {
      Result = "EBADFD : File descriptor in bad state.";
      break;
    }

    case EBADMSG:
    {
      Result = "EBADMSG : Bad message (POSIX.1-2001).";
      break;
    }

    case EBADR:
    {
      Result = "EBADR : Invalid request descriptor.";
      break;
    }

    case EBADRQC:
    {
      Result = "EBADRQC : Invalid request code.";
      break;
    }

    case EBADSLT:
    {
      Result = "EBADSLT : Invalid slot.";
      break;
    }

    case EBUSY:
    {
      Result = "EBUSY : Device or resource busy (POSIX.1-2001).";
      break;
    }

    case ECANCELED:
    {
      Result = "ECANCELED : Operation canceled (POSIX.1-2001).";
      break;
    }

    case ECHILD:
    {
      Result = "ECHILD : No child processes (POSIX.1-2001).";
      break;
    }

    case ECHRNG:
    {
      Result = "ECHRNG : Channel number out of range.";
      break;
    }

    case ECOMM:
    {
      Result = "ECOMM : Communication error on send.";
      break;
    }

    case ECONNABORTED:
    {
      Result = "ECONNABORTED : Connection aborted (POSIX.1-2001).";
      break;
    }

    case ECONNRESET:
    {
      Result = "ECONNRESET : Connection reset (POSIX.1-2001).";
      break;
    }

    case EDEADLOCK:
    {
      CAssert( EDEADLK == EDEADLOCK);
      Result = "EDEADLOCK : On most architectures, a synonym for EDEADLK.  On some architectures (e.g., Linux MIPS, PowerPC, SPARC), it is a separate error code \"File locking deadlock error\".";
      break;
    }

    case EDESTADDRREQ:
    {
      Result = "EDESTADDRREQ : Destination address required (POSIX.1-2001).";
      break;
    }

    case EDOM:
    {
      Result = "EDOM : Mathematics argument out of domain of function (POSIX.1, C99).";
      break;
    }

    case EDQUOT:
    {
      Result = "EDQUOT : Disk quota exceeded (POSIX.1-2001).";
      break;
    }

    case EEXIST:
    {
      Result = "EEXIST : File exists (POSIX.1-2001).";
      break;
    }

    case EFAULT:
    {
      Result = "EFAULT : Bad address (POSIX.1-2001).";
      break;
    }

    case EFBIG:
    {
      Result = "EFBIG : File too large (POSIX.1-2001).";
      break;
    }

    case EHOSTDOWN:
    {
      Result = "EHOSTDOWN : Host is down.";
      break;
    }

    case EHOSTUNREACH:
    {
      Result = "EHOSTUNREACH : Host is unreachable (POSIX.1-2001).";
      break;
    }

    case EHWPOISON:
    {
      Result = "EHWPOISON : Memory page has hardware error.";
      break;
    }

    case EIDRM:
    {
      Result = "EIDRM : Identifier removed (POSIX.1-2001).";
      break;
    }

    case EILSEQ:
    {
      Result = "EILSEQ : Invalid or incomplete multibyte or wide character (POSIX.1, C99).  The text shown here is the glibc error description; in POSIX.1, this error is described as \"Illegal byte sequence\".";
      break;
    }

    case EINPROGRESS:
    {
      Result = "EINPROGRESS : Operation in progress (POSIX.1-2001).";
      break;
    }

    case EINTR:
    {
      Result = "EINTR : Interrupted function call (POSIX.1-2001); see signal(7).";
      break;
    }

    case EINVAL:
    {
      Result = "EINVAL : Invalid argument (POSIX.1-2001).";
      break;
    }

    case EIO:
    {
      Result = "EIO : Input/output error (POSIX.1-2001).";
      break;
    }

    case EISCONN:
    {
      Result = "EISCONN : Socket is connected (POSIX.1-2001).";
      break;
    }

    case EISDIR:
    {
      Result = "EISDIR : Is a directory (POSIX.1-2001).";
      break;
    }

    case EISNAM:
    {
      Result = "EISNAM : Is a named type file.";
      break;
    }

    case EKEYEXPIRED:
    {
      Result = "EKEYEXPIRED : Key has expired.";
      break;
    }

    case EKEYREJECTED:
    {
      Result = "EKEYREJECTED : Key was rejected by service.";
      break;
    }

    case EKEYREVOKED:
    {
      Result = "EKEYREVOKED : Key has been revoked.";
      break;
    }

    case EL2HLT:
    {
      Result = "EL2HLT : Level 2 halted.";
      break;
    }

    case EL2NSYNC:
    {
      Result = "EL2NSYNC : Level 2 not synchronized.";
      break;
    }

    case EL3HLT:
    {
      Result = "EL3HLT : Level 3 halted.";
      break;
    }

    case EL3RST:
    {
      Result = "EL3RST : Level 3 reset.";
      break;
    }

    case ELIBACC:
    {
      Result = "ELIBACC : Cannot access a needed shared library.";
      break;
    }

    case ELIBBAD:
    {
      Result = "ELIBBAD : Accessing a corrupted shared library.";
      break;
    }

    case ELIBMAX:
    {
      Result = "ELIBMAX : Attempting to link in too many shared libraries.";
      break;
    }

    case ELIBSCN:
    {
      Result = "ELIBSCN : .lib section in a.out corrupted";
      break;
    }

    case ELIBEXEC:
    {
      Result = "ELIBEXEC : Cannot exec a shared library directly.";
      break;
    }

    case ELOOP:
    {
      Result = "ELOOP : Too many levels of symbolic links (POSIX.1-2001).";
      break;
    }

    case EMEDIUMTYPE:
    {
      Result = "EMEDIUMTYPE : Wrong medium type.";
      break;
    }

    case EMFILE:
    {
      Result = "EMFILE : Too many open files (POSIX.1-2001).  Commonly caused by exceeding the RLIMIT_NOFILE resource limit described in getrlimit(2).  Can also be caused by exceeding the limit specified in /proc/sys/fs/nr_open.";
      break;
    }

    case EMLINK:
    {
      Result = "EMLINK : Too many links (POSIX.1-2001).";
      break;
    }

    case EMSGSIZE:
    {
      Result = "EMSGSIZE : Message too long (POSIX.1-2001).";
      break;
    }

    case EMULTIHOP:
    {
      Result = "EMULTIHOP : Multihop attempted (POSIX.1-2001).";
      break;
    }

    case ENAMETOOLONG:
    {
      Result = "ENAMETOOLONG : Filename too long (POSIX.1-2001).";
      break;
    }

    case ENETDOWN:
    {
      Result = "ENETDOWN : Network is down (POSIX.1-2001).";
      break;
    }

    case ENETRESET:
    {
      Result = "ENETRESET : Connection aborted by network (POSIX.1-2001).";
      break;
    }

    case ENETUNREACH:
    {
      Result = "ENETUNREACH : Network unreachable (POSIX.1-2001).";
      break;
    }

    case ENFILE:
    {
      Result = "ENFILE : Too many open files in system (POSIX.1-2001).  On Linux, this is probably a result of encountering the /proc/sys/fs/file-max limit (see proc(5)).";
      break;
    }

    case ENOANO:
    {
      Result = "ENOANO : No anode.";
      break;
    }

    case ENOBUFS:
    {
      Result = "ENOBUFS : No buffer space available (POSIX.1 (XSI STREAMS option)).";
      break;
    }

    case ENODATA:
    {
      Result = "ENODATA : No message is available on the STREAM head read queue (POSIX.1-2001).";
      break;
    }

    case ENODEV:
    {
      Result = "ENODEV : No such device (POSIX.1-2001).";
      break;
    }

    case ENOENT:
    {
      Result = "ENOENT : No such file or directory (POSIX.1-2001).  Typically, this error results when a specified pathname does not exist, or one of the components in the directory prefix of a pathname does not exist, or the specified pathname is a dangling symbolic link.";
      break;
    }

    case ENOEXEC:
    {
      Result = "ENOEXEC : Exec format error (POSIX.1-2001).";
      break;
    }

    case ENOKEY:
    {
      Result = "ENOKEY : Required key not available.";
      break;
    }

    case ENOLCK:
    {
      Result = "ENOLCK : No locks available (POSIX.1-2001).";
      break;
    }

    case ENOLINK:
    {
      Result = "ENOLINK : Link has been severed (POSIX.1-2001).";
      break;
    }

    case ENOMEDIUM:
    {
      Result = "ENOMEDIUM : No medium found.";
      break;
    }

    case ENOMEM:
    {
      Result = "ENOMEM : Not enough space/cannot allocate memory (POSIX.1-2001).";
      break;
    }

    case ENOMSG:
    {
      Result = "ENOMSG : No message of the desired type (POSIX.1-2001).";
      break;
    }

    case ENONET:
    {
      Result = "ENONET : Machine is not on the network.";
      break;
    }

    case ENOPKG:
    {
      Result = "ENOPKG : Package not installed.";
      break;
    }

    case ENOPROTOOPT:
    {
      Result = "ENOPROTOOPT : Protocol not available (POSIX.1-2001).";
      break;
    }

    case ENOSPC:
    {
      Result = "ENOSPC : No space left on device (POSIX.1-2001).";
      break;
    }

    case ENOSR:
    {
      Result = "ENOSR : No STREAM resources (POSIX.1 (XSI STREAMS option)).";
      break;
    }

    case ENOSTR:
    {
      Result = "ENOSTR : Not a STREAM (POSIX.1 (XSI STREAMS option)).";
      break;
    }

    case ENOSYS:
    {
      Result = "ENOSYS : Function not implemented (POSIX.1-2001).";
      break;
    }

    case ENOTBLK:
    {
      Result = "ENOTBLK : Block device required.";
      break;
    }

    case ENOTCONN:
    {
      Result = "ENOTCONN : The socket is not connected (POSIX.1-2001).";
      break;
    }

    case ENOTDIR:
    {
      Result = "ENOTDIR : Not a directory (POSIX.1-2001).";
      break;
    }

    case ENOTEMPTY:
    {
      Result = "ENOTEMPTY : Directory not empty (POSIX.1-2001).";
      break;
    }

    case ENOTRECOVERABLE:
    {
      Result = "ENOTRECOVERABLE : State not recoverable (POSIX.1-2008).";
      break;
    }

    case ENOTSOCK:
    {
      Result = "ENOTSOCK : Not a socket (POSIX.1-2001).";
      break;
    }

    case ENOTTY:
    {
      Result = "ENOTTY : Inappropriate I/O control operation (POSIX.1-2001).";
      break;
    }

    case ENOTUNIQ:
    {
      Result = "ENOTUNIQ : Name not unique on network.";
      break;
    }

    case ENXIO:
    {
      Result = "ENXIO : No such device or address (POSIX.1-2001).";
      break;
    }

    case EOPNOTSUPP:
    {
      CAssert(ENOTSUP == EOPNOTSUPP);
      Result = "EOPNOTSUPP : Operation not supported on socket (POSIX.1-2001).  (ENOTSUP and EOPNOTSUPP have the same value on Linux, but according to POSIX.1 these error values should be distinct.)";
      break;
    }

    case EOVERFLOW:
    {
      Result = "EOVERFLOW : Value too large to be stored in data type (POSIX.1-2001).";
      break;
    }

    case EOWNERDEAD:
    {
      Result = "EOWNERDEAD : Owner died (POSIX.1-2008).";
      break;
    }

    case EPERM:
    {
      Result = "EPERM : Operation not permitted (POSIX.1-2001).";
      break;
    }

    case EPFNOSUPPORT:
    {
      Result = "EPFNOSUPPORT : Protocol family not supported.";
      break;
    }

    case EPIPE:
    {
      Result = "EPIPE : Broken pipe (POSIX.1-2001).";
      break;
    }

    case EPROTO:
    {
      Result = "EPROTO : Protocol error (POSIX.1-2001).";
      break;
    }

    case EPROTONOSUPPORT:
    {
      Result = "EPROTONOSUPPORT : Protocol not supported (POSIX.1-2001).";
      break;
    }

    case EPROTOTYPE:
    {
      Result = "EPROTOTYPE : Protocol wrong type for socket (POSIX.1-2001).";
      break;
    }

    case ERANGE:
    {
      Result = "ERANGE : Result too large (POSIX.1, C99).";
      break;
    }

    case EREMCHG:
    {
      Result = "EREMCHG : Remote address changed.";
      break;
    }

    case EREMOTE:
    {
      Result = "EREMOTE : Object is remote.";
      break;
    }

    case EREMOTEIO:
    {
      Result = "EREMOTEIO : Remote I/O error.";
      break;
    }

    case ERESTART:
    {
      Result = "ERESTART : Interrupted system call should be restarted.";
      break;
    }

    case ERFKILL:
    {
      Result = "ERFKILL : Operation not possible due to RF-kill.";
      break;
    }

    case EROFS:
    {
      Result = "EROFS : Read-only filesystem (POSIX.1-2001).";
      break;
    }

    case ESHUTDOWN:
    {
      Result = "ESHUTDOWN : Cannot send after transport endpoint shutdown.";
      break;
    }

    case ESPIPE:
    {
      Result = "ESPIPE : Invalid seek (POSIX.1-2001).";
      break;
    }

    case ESOCKTNOSUPPORT:
    {
      Result = "ESOCKTNOSUPPORT : Socket type not supported.";
      break;
    }

    case ESRCH:
    {
      Result = "ESRCH : No such process (POSIX.1-2001).";
      break;
    }

    case ESTALE:
    {
      Result = "ESTALE : Stale file handle (POSIX.1-2001).  This error can occur for NFS and for other filesystems.";
      break;
    }

    case ESTRPIPE:
    {
      Result = "ESTRPIPE : Streams pipe error.";
      break;
    }

    case ETIME:
    {
      Result = "ETIME : Timer expired (POSIX.1 (XSI STREAMS option)).  (POSIX.1 says \"STREAM ioctl(2) timeout\".)";
      break;
    }

    case ETIMEDOUT:
    {
      Result = "ETIMEDOUT : Connection timed out (POSIX.1-2001).";
      break;
    }

    case ETOOMANYREFS:
    {
      Result = "ETOOMANYREFS : Too many references: cannot splice.";
      break;
    }

    case ETXTBSY:
    {
      Result = "ETXTBSY : Text file busy (POSIX.1-2001).";
      break;
    }

    case EUCLEAN:
    {
      Result = "EUCLEAN : Structure needs cleaning.";
      break;
    }

    case EUNATCH:
    {
      Result = "EUNATCH : Protocol driver not attached.";
      break;
    }

    case EUSERS:
    {
      Result = "EUSERS : Too many users.";
      break;
    }

    case EXDEV:
    {
      Result = "EXDEV : Improper link (POSIX.1-2001).";
      break;
    }

    case EXFULL:
    {
      Result = "EXFULL : Exchange full.";
      break;
    }
  }

  Assert(Result);
  return Result;
}
