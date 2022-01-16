
global_variable b32 Global_DoRuntimeBreak = False;

global_variable log_level Global_LogLevel = LogLevel_Debug;

// TODO(Jesse): Remove this?  Maybe put it onto the parser?   I'm not crazy
// about that because it' bloats that struct and we create those things like
// crazy.. but I don't really like that it's a global.
static char* Global_ParserErrorBuffer[4096];
