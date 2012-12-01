#ifndef ILCLOG_H
#define ILCLOG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLog.h 50615 2011-07-16 23:19:19Z hristov $ */

#include <TClass.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TString.h>

// deprecation macro
#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
# define ILCROOT_DEPRECATED(func) func  __attribute__ ((deprecated))
#elif defined(_MSC_VER) && _MSC_VER >= 1300
# define ILCROOT_DEPRECATED(func) __declspec(deprecated) func
#else
# define ILCROOT_DEPRECATED(func) func
#endif

/**
 * class for logging debug, info and error messages
 */
class IlcLog: public TObject
{
 public:

		// Log4j log levels: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
  enum EType_t {kFatal = 0, kError, kWarning, kInfo, kDebug, kMaxType};
  typedef void (*IlcLogNotification)(EType_t type, const char* message );

		// NB: singleton constructor & destructor should not be public!
		// ILCROOT_DEPRECATED(IlcLog());
		// ILCROOT_DEPRECATED(virtual ~IlcLog());

		// NB: singleton deprecated static instance method
		// ILCROOT_DEPRECATED(static IlcLog* Instance() {return fgInstance;};)

		// get root logger singleton instance
		static IlcLog *GetRootLogger();

		// delete root logger singleton instance
		static void DeleteRootLogger();

		// NB: the following functions should not be static
		// NB: deprecated: logging configuration should be made through to a configuration file
  static void  EnableDebug(Bool_t enabled);
  static void  SetGlobalLogLevel(EType_t type);
  static Int_t GetGlobalLogLevel();
  static void  SetGlobalDebugLevel(Int_t level);
  static Int_t GetGlobalDebugLevel();
  static void  SetModuleDebugLevel(const char* module, Int_t level);
  static void  ClearModuleDebugLevel(const char* module);
  static void  SetClassDebugLevel(const char* className, Int_t level);
  static void  ClearClassDebugLevel(const char* className);

  static void  SetStandardOutput();
  static void  SetStandardOutput(EType_t type);
  static void  SetErrorOutput();
  static void  SetErrorOutput(EType_t type);
  static void  SetFileOutput(const char* fileName);
  static void  SetFileOutput(EType_t type, const char* fileName);
  static void  SetStreamOutput(ostream* stream);
  static void  SetStreamOutput(EType_t type, ostream* stream);
  static void  SetLogNotification(IlcLogNotification pCallBack);
  static void  SetLogNotification(EType_t type, IlcLogNotification pCallBack);
  static void  Flush();

  static void  SetHandleRootMessages(Bool_t on);

  static void  SetPrintType(Bool_t on);
  static void  SetPrintType(EType_t type, Bool_t on);
  static void  SetPrintModule(Bool_t on);
  static void  SetPrintModule(EType_t type, Bool_t on);
  static void  SetPrintScope(Bool_t on);
  static void  SetPrintScope(EType_t type, Bool_t on);
  static void  SetPrintLocation(Bool_t on);
  static void  SetPrintLocation(EType_t type, Bool_t on);

  static void  SetPrintRepetitions(Bool_t on);

  static void  WriteToFile(const char* name, Int_t option = 0);

  // the following public methods are used by the preprocessor macros 
  // and should not be called directly
#ifdef WIN32
  static Bool_t IsDebugEnabled(); 
  static IlcLog* Instance(); 
#else
  static Bool_t IsDebugEnabled() {return fgDebugEnabled;}
  static IlcLog* Instance() {return fgInstance;}
#endif

  static Int_t GetDebugLevel(const char* module, const char* className);
  static void  Message(UInt_t level, const char* message, 
                       const char* module, const char* className,
                       const char* function, const char* file, Int_t line);
  static void  Debug(UInt_t level, const char* message, 
                     const char* module, const char* className,
                     const char* function, const char* file, Int_t line);

  static Int_t RedirectStdoutTo(EType_t type, UInt_t level, const char* module, 
                                const char* className, const char* function,
                                const char* file, Int_t line, Bool_t print);
  static Int_t RedirectStderrTo(EType_t type, UInt_t level, const char* module, 
                                const char* className, const char* function,
                                const char* file, Int_t line, Bool_t print);
  static void  RestoreStdout(Int_t original);
  static void  RestoreStderr(Int_t original);

  static ostream& Stream(EType_t type, UInt_t level,
                         const char* module, const char* className,
                         const char* function, const char* file, Int_t line);

 private:

		// constructor is made private for implementing a singleton
		IlcLog();
		virtual ~IlcLog();

		// NOT IMPLEMENTED?
  IlcLog(const IlcLog& log);
  IlcLog& operator = (const IlcLog& log);

  void           ReadEnvSettings();

  static void    RootErrorHandler(Int_t level, Bool_t abort, 
				  const char* location, const char* message);

  void           CloseFile(Int_t type);
  FILE*          GetOutputStream(Int_t type);

  UInt_t         GetLogLevel(const char* module, const char* className) const;
  void           PrintMessage(UInt_t type, const char* message, 
                              const char* module, const char* className,
                              const char* function, 
                              const char* file, Int_t line);

  void           PrintString(Int_t type, FILE* stream, const char* format, ...);
  void           PrintRepetitions();

  Int_t          RedirectTo(FILE* stream, EType_t type, UInt_t level,
                            const char* module, const char* className,
                            const char* function,
                            const char* file, Int_t line, Bool_t print);

  ostream&       GetStream(EType_t type, UInt_t level,
                           const char* module, const char* className,
                           const char* function, const char* file, Int_t line);

  enum {kDebugOffset = kDebug-1};

#ifndef __MAKECINT__
	#ifdef WIN32
	  __declspec(dllexport) static  IlcLog* fgInstance;                 //! pointer to current instance

	  __declspec(dllexport) static  Bool_t  fgDebugEnabled;             // flag for debug en-/disabling
	#else
	  static IlcLog* fgInstance;                 //! pointer to current instance

	  static Bool_t  fgDebugEnabled;             // flag for debug en-/disabling
	#endif
#else
	  static IlcLog* fgInstance;                 //! pointer to current instance

	  static Bool_t  fgDebugEnabled;             // flag for debug en-/disabling
#endif  /*__MAKECINT__*/


  UInt_t         fGlobalLogLevel;            // global logging level
  TObjArray      fModuleDebugLevels;         // debug levels for modules
  TObjArray      fClassDebugLevels;          // debug levels for classes

  Int_t          fOutputTypes[kMaxType];     // types of output streams
  TString        fFileNames[kMaxType];       // file names
  FILE*          fOutputFiles[kMaxType];     //! log output files
  ostream*       fOutputStreams[kMaxType];   //! log output streams

  Bool_t         fPrintType[kMaxType];       // print type on/off
  Bool_t         fPrintModule[kMaxType];     // print module on/off
  Bool_t         fPrintScope[kMaxType];      // print scope/class name on/off
  Bool_t         fPrintLocation[kMaxType];   // print file and line on/off

  Bool_t         fPrintRepetitions;          // print number of repetitions instead of repeated message on/off

  Int_t          fRepetitions;               //! counter of repetitions
  UInt_t         fLastType;                  //! type of last message
  TString        fLastMessage;               //! last message
  TString        fLastModule;                //! module name of last message
  TString        fLastClassName;             //! class name of last message
  TString        fLastFunction;              //! function name of last message
  TString        fLastFile;                  //! file name of last message
  Int_t          fLastLine;                  //! line number of last message
  IlcLogNotification fCallBacks[kMaxType];   //! external notification callback

  ClassDef(IlcLog, 1)   // class for logging debug, info and error messages
};


// module name macro
#ifdef _MODULE_
# define MODULENAME() _MODULE_
#else
# define MODULENAME() "NoModule"
#endif

// function name macro
#if defined(__GNUC__) || defined(__ICC) || defined(__ECC) || defined(__APPLE__)
# define FUNCTIONNAME() __FUNCTION__
// #elif defined(__HP_aCC) || defined(__alpha) || defined(__DECCXX)
// #define FUNCTIONNAME() __FUNC__
#else
# define FUNCTIONNAME() "???"
#endif

// redirection
/** 
 * Redirect output to std::cout to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout 
 */
#define REDIRECTSTDOUT(type, level, scope, whatever) \
  do {Int_t originalStdout = IlcLog::RedirectStdoutTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; IlcLog::RestoreStdout(originalStdout);} while(false)
/** 
 * Redirect output to std::cerr to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout 
 */
#define REDIRECTSTDERR(type, level, scope, whatever) \
  do {Int_t originalStderr = IlcLog::RedirectStderrTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; IlcLog::RestoreStderr(originalStderr);} while(false)
/** 
 * Redirect output to std::cout and std::cerr to specified log stream 
 * 
 * @param type      Type of stream to re-direct to
 * @param level     Level of output
 * @param scope     Scope
 * @param whatever  Any code that will output to std::cout or std::cerr
 */
#define REDIRECTSTDOUTANDSTDERR(type, level, scope, whatever) \
  do {Int_t originalStdout = IlcLog::RedirectStdoutTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    Int_t originalStderr = IlcLog::RedirectStderrTo(type, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__, kFALSE); \
    whatever; IlcLog::RestoreStderr(originalStderr); IlcLog::RestoreStdout(originalStdout);} while(false)


// debug level
#ifdef LOG_NO_DEBUG
# define IlcDebugLevel() -1
# define IlcDebugLevelClass() -1
# define IlcDebugLevelGeneral(scope) -1
#else
/** 
 * Get the object scope debug level
 */
# define IlcDebugLevel() ((IlcLog::IsDebugEnabled()) ? IlcLog::GetDebugLevel(MODULENAME(), ClassName()) : -1)
/** 
 * Get the class (ROOT-enabled) debug level
 */
# define IlcDebugLevelClass() ((IlcLog::IsDebugEnabled()) ? IlcLog::GetDebugLevel(MODULENAME(), Class()->GetName()) : -1)
/**
 * Get the debug level associated with scope 
 * @param scope Scope 
 */
# define IlcDebugLevelGeneral(scope) ((IlcLog::IsDebugEnabled()) ? IlcLog::GetDebugLevel(MODULENAME(), scope) : -1)
#endif

// debug messages
#ifdef LOG_NO_DEBUG
# define IlcDebug(level, message) do { } while (false)
# define IlcDebugClass(level, message) do { } while (false)
# define IlcDebugGeneral(scope, level, message) do { } while (false)
# define IlcDebugF(level, message,...) do { } while (false)
# define IlcDebugClassF(level, message,...) do { } while (false)
# define IlcDebugGeneralF(scope, level, message,...) do { } while (false)
#else

// inspired by log4cxx code (see log4cxx/Logger.h)
// implements GCC branch prediction for increasing logging performance
# if !defined(ILCROOT_UNLIKELY)
#  if defined(__GNUC__) && (__GNUC__ >= 3)
/**
 * Provides optimization hint to the compiler
 * to optimize for the expression being false.
 * @param expr boolean expression.
 * @returns value of expression.
 */
#   define ILCROOT_UNLIKELY(expr) __builtin_expect(expr, 0)
#  else
/**
 * Provides optimization hint to the compiler
 * to optimize for the expression being false.
 * @param expr boolean expression.
 * @returns value of expression.
 */
#   define ILCROOT_UNLIKELY(expr) expr
#  endif
# endif 

/**
 * 
 * Logs a message to a specified logger with the DEBUG level.
 * 
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>IlcDebug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
 */
# define IlcDebug(logLevel, message) \
        do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), ClassName()) >= logLevel)) {\
	  IlcLog::Debug(logLevel, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/**
 * 
 * Logs a message to a specified logger with the DEBUG level.  For use
 * in static member functions of a class 
 * 
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>IlcDebug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
 */
# define IlcDebugClass(logLevel, message) \
	do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), Class()->GetName()) >= logLevel)) {\
	  IlcLog::Debug(logLevel, message, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)

/**
 * Logs a message to a specified logger with the DEBUG level.  For use
 * in non-ROOT-enabled-class scope.
 * 
 * @param scope the logging scope.
 * @param logLevel the debug level.
 * @param message message to print in the following format: Form(message).
 * Note, that message should contain balanced parenthesis, like 
 * <code>IlcDebug(1, Form("Failed to decode line %d of %s", line, filename));</code> 
*/
# define IlcDebugGeneral(scope, logLevel, message) \
	do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), scope) >= logLevel)) {\
	  IlcLog::Debug(logLevel, message, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Macro to output debugging information.  This excepts a printf-like
 * format statement.   Note, at least 3 arguments (in total) must be
 * passed. 
 * 
 * @param logLevel Debug level
 * @param format   Printf-like format. 
 */
# define IlcDebugF(logLevel,format,...) \
do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), ClassName()) >= logLevel)) { \
    TString m;m.Form(format,__VA_ARGS__);					\
    IlcLog::Debug(logLevel, m, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Outut debug information, filtered on debug level.  For use in
 * static member function of a ROOT-enabled class. This excepts a
 * printf-like format statement.  Note, at least 3 arguments (in
 * total) must be passed.
 * 
 * @param logLevel Debug level
 * @param format   Printf-like format 
 * 
 * @return 
 */
# define IlcDebugClassF(logLevel,format,...) \
  do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), Class()->GetName()) >= logLevel)) { \
      TString m;m.Form(format,__VA_ARGS__);					\
      IlcLog::Debug(logLevel, m, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
/** 
 * Outut debug information, filtered on debug level.  For use in
 * static member function of a non-ROOT-enabled class-scope. This
 * excepts a printf-like format statement.  Note, at least 3 arguments
 * (in total) must be passed.
 * 
 * @param scope    Scope 
 * @param logLevel Debug level
 * @param format   Printf-like format 
 * 
 * @return 
 */
# define IlcDebugGeneralF(scope,logLevel,format,...) \
  do { if (ILCROOT_UNLIKELY(IlcLog::IsDebugEnabled() && IlcLog::GetDebugLevel(MODULENAME(), scope) >= logLevel)) { \
      TString m;m.Form(format,__VA_ARGS__);					\
      IlcLog::Debug(logLevel, m, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__); }} while (0)
    
#endif

// redirection to debug
#define StdoutToIlcDebug(level, whatever) REDIRECTSTDOUT(IlcLog::kDebug, level, ClassName(), whatever)
#define StderrToIlcDebug(level, whatever) REDIRECTSTDERR(IlcLog::kDebug, level, ClassName(), whatever)
#define ToIlcDebug(level, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kDebug, level, ClassName(), whatever)
#define StdoutToIlcDebugClass(level, whatever) REDIRECTSTDOUT(IlcLog::kDebug, level, Class()->GetName(), whatever)
#define StderrToIlcDebugClass(level, whatever) REDIRECTSTDERR(IlcLog::kDebug, level, Class()->GetName(), whatever)
#define ToIlcDebugClass(level, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kDebug, level, Class()->GetName(), whatever)
#define StdoutToIlcDebugGeneral(scope, level, whatever) REDIRECTSTDOUT(IlcLog::kDebug, level, scope, whatever)
#define StderrToIlcDebugGeneral(scope, level, whatever) REDIRECTSTDERR(IlcLog::kDebug, level, scope, whatever)
#define ToIlcDebugGeneral(scope, level, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kDebug, level, scope, whatever)

// debug stream objects
#define IlcDebugStream(level) IlcLog::Stream(IlcLog::kDebug, level, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcDebugClassStream(level) IlcLog::Stream(IlcLog::kDebug, level, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcDebugGeneralStream(scope, level) IlcLog::Stream(IlcLog::kDebug, level, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


/** 
 * Macro that will output stuff using the logging facilities. 
 * 
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define IlcMessage(lvl,message) do { \
      IlcLog::Message(lvl, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Macro that will output stuff using the logging facilities. 
 * For use in static member function of ROOT-enabled class-scope.
 *
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define IlcMessageClass(lvl,message) do { \
    IlcLog::Message(lvl, message, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Macro that will output stuff using the logging facilities. 
 * For use in non-ROOT-enabled class-scope.
 *
 * @param scope   Scope 
 * @param lvl     Message level 
 * @param message Message to show 
 */
#define IlcMessageGeneral(scope,lvl,message) do {			\
    IlcLog::Message(lvl, message, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the IlcLog logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   IlcMessageF(1, "foo");        // <-- Failes
 *   IlcMessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define IlcMessageF(lvl,format,...) do { \
  TString m; m.Form(format,__VA_ARGS__); \
  IlcLog::Message(lvl, m, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the IlcLog logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   IlcMessageF(1, "foo");        // <-- Failes
 *   IlcMessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * This is for static member function in for ROOT-enabled class-scope
 *
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define IlcMessageClassF(lvl,format,...) do { \
  TString m; m.Form(format,__VA_ARGS__); \
  IlcLog::Message(lvl, m, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 
/** 
 * Print a message using the IlcLog logging facility. This macro
 * accepts printf-like format arguments.  Note, at least 3 arguments
 * must be passed.  
 * @code
 *   IlcMessageF(1, "foo");        // <-- Failes
 *   IlcMessageF(1, "foo %d", 42); // <-- OK
 * @endcode
 *
 * This is for non-ROOT-enabled class-scope
 *
 * @param scope   Scope 
 * @param lvl     Message level
 * @param format  printf-like format
 */
#define IlcMessageGeneralF(scope,lvl,format,...) do {	\
  TString m; m.Form(format,__VA_ARGS__); \
  IlcLog::Message(lvl, m, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__);} while(false) 

// info messages
#ifdef LOG_NO_INFO
# define IlcInfo(message) do { } while (false)
# define IlcInfoClass(message) do { } while (false)
# define IlcInfoGeneral(scope, message) do { } while (false)
# define IlcInfoF(message,...) do { } while (false)
# define IlcInfoClassF(message,...) do { } while (false)
# define IlcInfoGeneralF(scope, message,...) do { } while (false)
#else
/**
 * Forwards to IlcMessage with log level of IlcLog::kInfo
 * @see IlcMessage 
 */
# define IlcInfo(message)               IlcMessage(IlcLog::kInfo, message)
/**
 * Forwards to IlcMessageClass with log level of IlcLog::kInfo
 * @see IlcMessageClass 
 */
# define IlcInfoClass(message)          IlcMessageClass(IlcLog::kInfo, message)
/**
 * Forwards to IlcMessageGeneral with log level of IlcLog::kInfo
 * @see IlcMessageGeneral
 */
# define IlcInfoGeneral(scope, message) IlcMessageGeneral(scope, IlcLog::kInfo, message)
/**
 * Forwards to IlcMessageF with log level of IlcLog::kInfo
 * @see IlcMessageF 
 */
# define IlcInfoF(message,...)               IlcMessageF(IlcLog::kInfo, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageClassF with log level of IlcLog::kInfo
 * @see IlcMessageClassF 
 */
# define IlcInfoClassF(message,...)          IlcMessageClassF(IlcLog::kInfo, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageGeneralF with log level of IlcLog::kInfo
 * @see IlcMessageGeneralF
 */
# define IlcInfoGeneralF(scope,message,...)  IlcMessageGeneralF(scope, IlcLog::kInfo, message, __VA_ARGS__)
#endif

// redirection to info
#define StdoutToIlcInfo(whatever) REDIRECTSTDOUT(IlcLog::kInfo, 0, ClassName(), whatever)
#define StderrToIlcInfo(whatever) REDIRECTSTDERR(IlcLog::kInfo, 0, ClassName(), whatever)
#define ToIlcInfo(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kInfo, 0, ClassName(), whatever)
#define StdoutToIlcInfoClass(whatever) REDIRECTSTDOUT(IlcLog::kInfo, 0, Class()->GetName(), whatever)
#define StderrToIlcInfoClass(whatever) REDIRECTSTDERR(IlcLog::kInfo, 0, Class()->GetName(), whatever)
#define ToIlcInfoClass(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kInfo, 0, Class()->GetName(), whatever)
#define StdoutToIlcInfoGeneral(scope, whatever) REDIRECTSTDOUT(IlcLog::kInfo, 0, scope, whatever)
#define StderrToIlcInfoGeneral(scope, whatever) REDIRECTSTDERR(IlcLog::kInfo, 0, scope, whatever)
#define ToIlcInfoGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kInfo, 0, scope, whatever)

// info stream objects
#define IlcInfoStream() IlcLog::Stream(IlcLog::kInfo, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcInfoClassStream() IlcLog::Stream(IlcLog::kInfo, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcInfoGeneralStream(scope) IlcLog::Stream(IlcLog::kInfo, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)

// warning messages
#ifdef LOG_NO_WARNING
# define IlcWarning(message) do { } while (false)
# define IlcWarningClass(message) do { } while (false)
# define IlcWarningGeneral(scope, message) do { } while (false)
# define IlcWarningF(message,...) do { } while (false)
# define IlcWarningClassF(message,...) do { } while (false)
# define IlcWarningGeneralF(scope, message,...) do { } while (false)
#else
/**
 * Forwards to IlcMessage with log level of IlcLog::kWarning
 * @see IlcMessage 
 */
# define IlcWarning(message)               IlcMessage(IlcLog::kWarning, message)
/**
 * Forwards to IlcMessageClass with log level of IlcLog::kWarning
 * @see IlcMessageClass 
 */
# define IlcWarningClass(message)          IlcMessageClass(IlcLog::kWarning, message)
/**
 * Forwards to IlcMessageGeneral with log level of IlcLog::kWarning
 * @see IlcMessageGeneral
 */
# define IlcWarningGeneral(scope, message) IlcMessageGeneral(scope, IlcLog::kWarning, message)
/**
 * Forwards to IlcMessageF with log level of IlcLog::kWarning
 * @see IlcMessageF 
 */
# define IlcWarningF(message,...)               IlcMessageF(IlcLog::kWarning, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageClassF with log level of IlcLog::kWarning
 * @see IlcMessageClassF 
 */
# define IlcWarningClassF(message,...)          IlcMessageClassF(IlcLog::kWarning, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageGeneralF with log level of IlcLog::kWarning
 * @see IlcMessageGeneralF
 */
# define IlcWarningGeneralF(scope,message,...)  IlcMessageGeneralF(scope, IlcLog::kWarning, message, __VA_ARGS__)
#endif

// redirection to warning
#define StdoutToIlcWarning(whatever) REDIRECTSTDOUT(IlcLog::kWarning, 0, ClassName(), whatever)
#define StderrToIlcWarning(whatever) REDIRECTSTDERR(IlcLog::kWarning, 0, ClassName(), whatever)
#define ToIlcWarning(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kWarning, 0, ClassName(), whatever)
#define StdoutToIlcWarningClass(whatever) REDIRECTSTDOUT(IlcLog::kWarning, 0, Class()->GetName(), whatever)
#define StderrToIlcWarningClass(whatever) REDIRECTSTDERR(IlcLog::kWarning, 0, Class()->GetName(), whatever)
#define ToIlcWarningClass(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kWarning, 0, Class()->GetName(), whatever)
#define StdoutToIlcWarningGeneral(scope, whatever) REDIRECTSTDOUT(IlcLog::kWarning, 0, scope, whatever)
#define StderrToIlcWarningGeneral(scope, whatever) REDIRECTSTDERR(IlcLog::kWarning, 0, scope, whatever)
#define ToIlcWarningGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kWarning, 0, scope, whatever)

// warning stream objects
#define IlcWarningStream() IlcLog::Stream(IlcLog::kWarning, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcWarningClassStream() IlcLog::Stream(IlcLog::kWarning, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcWarningGeneralStream(scope) IlcLog::Stream(IlcLog::kWarning, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


// error messages
/**
 * Forwards to IlcMessage with log level of IlcLog::kError
 * @see IlcMessage 
 */
#define IlcError(message)               IlcMessage(IlcLog::kError, message)
/**
 * Forwards to IlcMessageClass with log level of IlcLog::kError
 * @see IlcMessageClass 
 */
#define IlcErrorClass(message)          IlcMessageClass(IlcLog::kError, message)
/**
 * Forwards to IlcMessageGeneral with log level of IlcLog::kError
 * @see IlcMessageGeneral
 */
#define IlcErrorGeneral(scope, message) IlcMessageGeneral(scope, IlcLog::kError, message)
/**
 * Forwards to IlcMessageF with log level of IlcLog::kError
 * @see IlcMessageF 
 */
#define IlcErrorF(message,...)               IlcMessageF(IlcLog::kError, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageClassF with log level of IlcLog::kError
 * @see IlcMessageClassF 
 */
#define IlcErrorClassF(message,...)          IlcMessageClassF(IlcLog::kError, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageGeneralF with log level of IlcLog::kError
 * @see IlcMessageGeneralF
 */
#define IlcErrorGeneralF(scope,message,...)  IlcMessageGeneralF(scope, IlcLog::kError, message, __VA_ARGS__)

// redirection to error
#define StdoutToIlcError(whatever) REDIRECTSTDOUT(IlcLog::kError, 0, ClassName(), whatever)
#define StderrToIlcError(whatever) REDIRECTSTDERR(IlcLog::kError, 0, ClassName(), whatever)
#define ToIlcError(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kError, 0, ClassName(), whatever)
#define StdoutToIlcErrorClass(whatever) REDIRECTSTDOUT(IlcLog::kError, 0, Class()->GetName(), whatever)
#define StderrToIlcErrorClass(whatever) REDIRECTSTDERR(IlcLog::kError, 0, Class()->GetName(), whatever)
#define ToIlcErrorClass(whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kError, 0, Class()->GetName(), whatever)
#define StdoutToIlcErrorGeneral(scope, whatever) REDIRECTSTDOUT(IlcLog::kError, 0, scope, whatever)
#define StderrToIlcErrorGeneral(scope, whatever) REDIRECTSTDERR(IlcLog::kError, 0, scope, whatever)
#define ToIlcErrorGeneral(scope, whatever) REDIRECTSTDOUTANDSTDERR(IlcLog::kError, 0, scope, whatever)

// error stream objects
#define IlcErrorStream() IlcLog::Stream(IlcLog::kError, 0, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcErrorClassStream() IlcLog::Stream(IlcLog::kError, 0, MODULENAME(), Class()->GetName(), FUNCTIONNAME(), __FILE__, __LINE__)
#define IlcErrorGeneralStream(scope) IlcLog::Stream(IlcLog::kError, 0, MODULENAME(), scope, FUNCTIONNAME(), __FILE__, __LINE__)


// fatal messages
/**
 * Forwards to IlcMessage with log level of IlcLog::kFatal
 * @see IlcMessage 
 */
#define IlcFatal(message)               IlcMessage(IlcLog::kFatal, message)
/**
 * Forwards to IlcMessageClass with log level of IlcLog::kFatal
 * @see IlcMessageClass 
 */
#define IlcFatalClass(message)          IlcMessageClass(IlcLog::kFatal, message)
/**
 * Forwards to IlcMessageGeneral with log level of IlcLog::kFatal
 * @see IlcMessageGeneral
 */
#define IlcFatalGeneral(scope, message) IlcMessageGeneral(scope, IlcLog::kFatal, message)
/**
 * Forwards to IlcMessageF with log level of IlcLog::kFatal
 * @see IlcMessageF 
 */
#define IlcFatalF(message,...)               IlcMessageF(IlcLog::kFatal, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageClassF with log level of IlcLog::kFatal
 * @see IlcMessageClassF 
 */
#define IlcFatalClassF(message,...)          IlcMessageClassF(IlcLog::kFatal, message, __VA_ARGS__)
/**
 * Forwards to IlcMessageGeneralF with log level of IlcLog::kFatal
 * @see IlcMessageGeneralF
 */
#define IlcFatalGeneralF(scope,message,...)  IlcMessageGeneralF(scope, IlcLog::kFatal, message, __VA_ARGS__)

#endif
