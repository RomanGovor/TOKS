#include "WorkWithPort.h"

int Server();

int main()
{
    system("cls");
    STARTUPINFO cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    CreateProcess("C:\\Users\\dell\\source\\repos\\toks_1_client\\Debug\\toks_1_client.exe", NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi); // �������� ��������
    MoveWindow(GetConsoleWindow(), 100, 100, 700, 600, true);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
    puts("\tPort #1 \n");
    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
    puts("If you want to close this program, print -'close' ");
    
    int res = Server();
    endWork("Port 1 ", res);
    return 0;
}

int Server()
{
   //int baudrate = inputBaudrate();
    int baudrate = setBaudrate();

    HANDLE  readEnd = CreateEvent(NULL, FALSE, FALSE, "readEnd"),            // ������������� ������� ��� �������������                
            writeEnd = CreateEvent(NULL, FALSE, FALSE, "writeEnd"),
            hExit = CreateEvent(NULL, FALSE, FALSE, "exit"),
            hWork = CreateEvent(NULL, FALSE, FALSE, "work");

        HANDLE hFile = CreateFile("COM1", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); // �������� ������ ��� ������ � ������
        if (hFile == NULL)
        {
            printf("Error in creating file\n");
            Sleep(2000);
            exit(1);
        }
        SetCommMask(hFile, EV_RXCHAR);                      // ������������� ����� �� ������� �����
        SetupComm(hFile, 1500, 1500);                       // �������������� ���������������� ����������
    
        COMMTIMEOUTS CommTimeOuts;                          // ���������, ��������������� ��������� ��������� ����������������� �����.
        CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;      // ���� ���������� ��� ��������� �������� ������/������
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0;        
        CommTimeOuts.ReadTotalTimeoutConstant = 1200;      
        CommTimeOuts.WriteTotalTimeoutMultiplier = 0;      
        CommTimeOuts.WriteTotalTimeoutConstant = 1200;      
    
        if (!SetCommTimeouts(hFile, &CommTimeOuts))         // ��������� �������������� ��������� ���������
        {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            printf("Error in work with Handle\n");
            Sleep(2000);
            exit(2);
        }
    
        DCB ComDCM;                               // ���������, ��������������� �������� ��������� ����������������� �����
        memset(&ComDCM, 0, sizeof(ComDCM));       // ��������� ������ ��� ���������
        ComDCM.DCBlength = sizeof(DCB);           // ������ �����, � ������, ���������
        GetCommState(hFile, &ComDCM);             // ��������� ������ � ������� ���������� ����������� �������� ��� ���������� ����������
        ComDCM.BaudRate = DWORD(baudrate);        // �������� �������� ������.
        ComDCM.ByteSize = 8;                      // ���������� ����� �������������� ��� � ������������ � ����������� ������.
        ComDCM.Parity = NOPARITY;                 // ���������� ����� ����� �������� �������� 
        ComDCM.StopBits = ONESTOPBIT;             // ������ ���������� �������� ��� 
        ComDCM.fAbortOnError = TRUE;              // ������ ������������� ���� �������� ������/������ ��� ������������� ������
        ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // ������ ����� ���������� ������� ��� ������� DTR
        ComDCM.fRtsControl = RTS_CONTROL_DISABLE; // ������ ����� ���������� ������� ��� ������� RTS
        ComDCM.fBinary = TRUE;                    // �������� �������� ����� ������
        ComDCM.fParity = FALSE;                   // �������� ����� �������� ��������
        ComDCM.fInX = FALSE;                      // ������ ������������� XON/XOFF ���������� ������� ��� ������
        ComDCM.fOutX = FALSE;                     // ������ ������������� XON/XOFF ���������� ������� ��� ��������
        ComDCM.XonChar = 0;                       // ������ ������ XON ������������ ��� ��� ������, ��� � ��� ��������
        ComDCM.XoffChar = (unsigned char)0xFF;    // ������ ������ XOFF ������������ ��� ��� ������, ��� � ��� ��������
        ComDCM.fErrorChar = FALSE;                // ������ ������, �������������� ��� ������ �������� � ��������� ���������
        ComDCM.fNull = FALSE;                     // ��������� �� ������������� ������ �������� � ������� �������� �� ������ ���������� ����� ErrorChar
        ComDCM.fOutxCtsFlow = FALSE;              // �������� ����� �������� �� �������� CTS
        ComDCM.fOutxDsrFlow = FALSE;              // �������� ����� �������� �� �������� DSR
        ComDCM.XonLim = 128;                      // ������ ����������� ����� �������� � �������� ������ ����� �������� ������� XON
        ComDCM.XoffLim = 128;                     // ���������� ������������ ���������� ���� � �������� ������ ����� �������� ������� XOFF
    
        if (!SetCommState(hFile, &ComDCM))        // ��������� ����� ����� 
        {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            printf("Error in work communication\n");
            Sleep(2000);
            exit(3);
        }
    
        processingPorts(readEnd, writeEnd, hFile, hExit, hWork);

        CloseHandle(hFile);
    return 0;
}



