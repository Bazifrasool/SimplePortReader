#include <windows.h>
#include <iostream>
#include <string>
HANDLE serialHandle;

int main() {
	LPCWSTR PORT_DL = L"\\\\.\\COM6";
	serialHandle = CreateFile(PORT_DL, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = 19200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = 1;
	serialParams.Parity = 0;
	SetCommState(serialHandle, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 50;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;

	SetCommTimeouts(serialHandle, &timeout);
	DWORD nRead;
	DWORD nWritten;
	char BUFFER [26] ;
	int retry = 0;
	memset(BUFFER, 96, 26);
	HANDLE output;
	LPCWSTR OUTPUTFN = L"OUTPUT_FILE.txt";
	std::string ENDLINE = "\n";
	std::string DUMP = "dump";
	output =CreateFile(OUTPUTFN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(serialHandle, (LPVOID)(DUMP.c_str()), DUMP.length(), &nWritten, NULL);
	while (retry <=7) {
		std::cout << "DONE" << std::endl;
		ReadFile(serialHandle, (LPVOID)BUFFER, sizeof(BUFFER), &nRead, NULL);
		std::cout << nRead << std::endl;
		if (nRead ==0 ) {
			retry++;
		}
		else {
			retry = 0;
			WriteFile(output, (LPVOID)ENDLINE.c_str(), ENDLINE.length(), &nWritten, NULL);
			WriteFile(output, (LPVOID)BUFFER, sizeof(BUFFER), &nWritten, NULL);
	

		}
		
	}
	CloseHandle(output);
	CloseHandle(serialHandle);
}