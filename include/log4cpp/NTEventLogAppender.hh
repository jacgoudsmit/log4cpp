/*
 * NTEventLogAppender.hh
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _LOG4CPP_NTEVENTLOGAPPENDER_HH
#define _LOG4CPP_NTEVENTLOGAPPENDER_HH

#ifdef WIN32 // only available on Win32

// deal with ERROR #define
// N.B. This #includes windows.h with NOGDI and WIN32_LEAN_AND_MEAN #defined.
//      If this is not what the user wants, #include windows.h before this file.
#ifndef _WINDOWS_
#  ifndef NOGDI
#    define NOGDI  // this will circumvent the ERROR #define in windows.h
#    define LOG4CPP_UNDEFINE_NOGDI
#  endif

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define LOG4CPP_UNDEFINE_WIN32_LEAN_AND_MEAN
#  endif

#  include <windows.h>

#  ifdef LOG4CPP_UNDEFINE_NOGDI
#    undef NOGDI
#  endif

#  ifdef LOG4CPP_UNDEFINE_WIN32_LEAN_AND_MEAN
#    undef WIN32_LEAN_AND_MEAN
#  endif

#endif // done dealing with ERROR #define

#include <log4cpp/Portability.hh>
#include <log4cpp/AppenderSkeleton.hh>

namespace log4cpp {

    /**
     * NTEventLogAppender is an Appender that sends LoggingEvents to the 
     * Windows event log.
     * <BR>
     * Building log4cpp creates the resource DLL NTEventLogAppender.dll.
     * Unless you use a constructor that takes the path to the DLL as a
     * parameter, and you specify the complete location (drive, directory
     * and filename) of the resource DLL, you must place this DLL in a
     * directory that is on the PATH of the Windows system. Otherwise, Event
     * Viewer will not be able to correctly display the events.
     * <BR>
     * <B>NB:</B> This class is only available on Win32 platforms.
     **/
    class LOG4CPP_EXPORT NTEventLogAppender : public AppenderSkeleton {
    public:

        /**
          * Constructor for backwards compatibility
          * <BR>
          * The source name must be non-blank, otherwise the appender is
          * created in closed state and cannot be opened. This is useful
          * when creating a subclass.
          * <BR>
          * This constructor uses "NTEventLogAppender.dll" as dll location;
          * this requires that that DLL be copied to a directory in the PATH
          * environment variable. Use the other constructor to set a full
          * location.
          *
          * @param name Name of the appender
          * @param sourceName Name (registry subkey name) to use as event logging source. Blank=don't open the appender
          */
        NTEventLogAppender(const std::string& name, const std::string& sourceName);

        /**
          * Constructor with DLL location
          * <BR>
          * If a blank DLL location is specified, "NTEventLogAppender.dll" is
          * used.
          * <BR>
          * If the DLL location doesn't include a full path to the file, or
          * if the location parameter is blank, Windows will use the PATH
          * environment variable to find the file. If the file cannot be
          * found, Event Viewer will not format the events correctly.
          * <BR>
          * The source name must be non-blank, otherwise the appender is
          * created in closed state and cannot be opened. This is useful
          * when creating a subclass.
          *
          * @param name Name of the appender
          * @param sourceName Name (registry subkey name) to use as event logging source. Blank=don't open the appender
          * @param dllLocation Location of the resource DLL. Blank="NTEventLogAppender.dll"
          */
        NTEventLogAppender(const std::string &name, const std::string &sourceName, const std::string &dllLocation);

        virtual ~NTEventLogAppender();

        /**
         * Starts logging to the source initialized in the registry.
         * <BR>
         * If the appender is already open, it's closed first.
         * <BR>
         * If the source name was not set at construction time, the function
         * fails.
         *
         * @returns true if the operation was successful.
         **/
        virtual bool reopen();

        /**
          * Closes the appender if it's open.
          * <BR>
          * No action if the appender is already closed.
          **/
        virtual void close();

        /**
         * The NTEventLogAppender does its own Layout.
         *
         * @returns false
         **/
        virtual bool requiresLayout() const;

        virtual void setLayout(Layout* layout);

    protected:
        
        WORD getCategory(Priority::Value priority);
        WORD getType(Priority::Value priority);
        HKEY regGetKey(LPCSTR subkey, LPDWORD pdisposition);
        void regSetString(HKEY hkey, LPCSTR name, LPCSTR value);
        void regSetDword(HKEY hkey, LPCSTR name, DWORD value);

        /**
          * Register the appender in the registry using the current values
          * of the member variables. This is called by the constructors,
          * but can also be useful if you want to create a subclass. The
          * function should be called before calling reopen(), otherwise
          * the reopen() will fail.
          * <BR>
          * The function uses the source name to create a key in the registry
          * which is then used as source to log events. The name of the
          * resource DLL is stored in the values under the key.
          * <BR>
          * If the source name is blank, the function fails. If the DLL
          * location is blank, the function uses the default
          * "NTEventLogAppender.dll".
          * <BR>
          * NOTE: if the DLL location doesn't include a full path (or if
          * it's blank), Windows will attempt to find the DLL with the PATH
          * environment variable.
          */
        virtual bool initRegistry();

        /**
          * \deprecated This function is deprecated. Call initRegistry() instead.
          */
        void addRegistryInfo(LPCSTR source) { initRegistry(); }

        /**
          * \deprecated This function is deprecated. Call reopen() instead.
          */
        virtual void open() { reopen(); }

        /**
         * Sends a LoggingEvent to NT Event log.
         * @param event the LoggingEvent to log.
         **/
        virtual void _append(const LoggingEvent& event);

        HANDLE _hEventSource;
        std::string _strSourceName;
        std::string _strDllLocation;
    };
}

#else // WIN32
#error NTEventLoggAppender is not available on non-Win32 platforms
#endif // WIN32

#endif // _LOG4CPP_NTEVENTLOGAPPENDER_HH

