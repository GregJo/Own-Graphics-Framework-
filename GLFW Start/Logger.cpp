#pragma once

#include "Logger.h"
#include <stdarg.h>
#include <memory>
#include <assert.h>
#include <cstring>
#include <string>
#include <iostream>

static std::shared_ptr<Logger> Instance;
const int MAX_DEBUG_LINE_LEN = 8192;

Logger::Logger()
{
	m_initialized = false;
}


Logger::~Logger()
{
	Release();
}


void Logger::SetInstance(Logger& logger)
{
	Instance = std::shared_ptr<Logger>(&logger, [](Logger* ptr){});
}


Logger& Logger::GetInstance()
{
	if(Instance.get() == nullptr)
	{
		Instance = std::shared_ptr<Logger>(new Logger());
		Instance->Init(std::string("Log.txt").c_str());
	}
	return *Instance.get();
}

//! \brief Initialize log file.
void Logger::InitLogFile()
{
	m_logStream.open("Log.txt", std::fstream::out);

	if (m_logStream.is_open())
	{
		m_logStream.clear();
	}
}


bool Logger::Init(const char* logName)
{
	if(IsInitialized())
		return false;

	InitLogFile();

	m_initialized = true;

	return true;
}


void Logger::Release()
{
	m_logStream.close();

	m_initialized = false;
}


void Logger::Log(const char* text, ...)
{
	if(!IsInitialized())
		return;

	char buffer[MAX_DEBUG_LINE_LEN];

	va_list args; 
	va_start(args, text);

	int buf = _vsnprintf_s(buffer, MAX_DEBUG_LINE_LEN, text, args); 

	assert((buf >= 0) && (buf < MAX_DEBUG_LINE_LEN)); 
	va_end(args);
		
	// Log output to debug windows and/or disk depending on options
	char buffer2[MAX_DEBUG_LINE_LEN];
	strcpy_s(buffer2, MAX_DEBUG_LINE_LEN, "DEBUG: ");
	strcpy_s(buffer2+7, MAX_DEBUG_LINE_LEN-7, buffer);
	LogOutput(buffer);
}

void Logger::LogOutput(char* buffer)
{
	// Strip any unnecessary newline characters at the end of the buffer
    int i = strlen(buffer);
	if(i == 0)
		return;
	if(buffer[i - 1] == '\n')
		buffer[i - 1] = 0;

	m_logStream << buffer << '\n';

	m_logStream.flush();

	DebugOutput(buffer);
}

void Logger::DebugOutput(const char* buffer)
{
	char buf[MAX_DEBUG_LINE_LEN];
    sprintf_s(buf, "%s\n", buffer);
	std::cout << buf;
}