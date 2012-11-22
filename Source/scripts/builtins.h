
#ifndef BUILTINS_H_
#define BUILTINS_H_

#ifdef __cplusplus
extern "C" {
#endif

// C code callbacks
int tinybasic_interpreter(int argc, const char* argv[]);
int picol_main(int argc, const char* argv[]);
int jimtcl_main(int argc, const char* argv[]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

// C++ only code callbacks
int cmd_help(int argc, const char* argv[]);

#endif

#endif /* BUILTINS_H_ */
