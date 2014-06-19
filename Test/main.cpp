#include <vector>
#include <fstream>
#include <stdarg.h>
#include <memory>
#include <assert.h>
#include <cstring>
#include <string>
#include <iostream>

const int MAX_DEBUG_LINE_LEN = 1024;
std::ofstream   m_logStream;


void DebugOutput(const char* buffer)
{
	char buf[MAX_DEBUG_LINE_LEN];
    sprintf_s(buf, "%s\n", buffer);
	std::cout << buf;
}

void LogOutput(char* buffer)
{
	// Strip any unnecessary newline characters at the end of the buffer
    int i = strlen(buffer);
	if(i == 0)
		return;
	if(buffer[i - 1] == '\n')
		buffer[i - 1] = 0;

	m_logStream << buffer << '\n';

	DebugOutput(buffer);
}

void Log(const char* text, ...)
{
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

int main()
{
    // Try it out.

    Log("Debug!");

    Log("Debug2!\n");

    return 0;
}
