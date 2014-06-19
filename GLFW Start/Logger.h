#pragma once
#include <vector>
#include <fstream>

class Logger 
{
public:
	~Logger();

	//! \brief This avoids the creations of a new instance
	//! If you are using the logger inside a dll, each module will create his own instance of the logger.
	//! You have to give an instance of the logger into every dll.
	static void SetInstance(Logger&);

	//! \brief Singleton instance.
	//! \return returns an instance or creates a new one
	static Logger& GetInstance();

	//! \brief Initialize log file.
	void InitLogFile();

	//! \brief Load File to stream into.
	//! \param logName Path to new logfile which will be created
	//! \return true if initialization succeeded
    bool Init(const char* logName);

	//! \brief Release stream logfile.
    void Release();

	bool IsInitialized() { return m_initialized; }

	void Log(const char* text, ...);

protected:
    Logger();

private:
	    
	void LogOutput(char* buffer); 

	//! \brief Outputs a text into the output
    void DebugOutput(const char* buffer); 

    bool            m_initialized;	//!< True if the logger is ready		
	std::ofstream   m_logStream;	//!< Logfile stream on Harddisk
};