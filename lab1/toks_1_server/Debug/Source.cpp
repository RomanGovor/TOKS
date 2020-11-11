#include "WorkWithPort.h"

int Server();

int main()
{
    system("cls");
    STARTUPINFO cif;
    ZeroMemory(&cif, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    CreateProcess("C:\\Users\\dell\\source\\repos\\toks_1_client\\Debug\\toks_1_client.exe", NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi); // Создание процесса
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

    HANDLE  readEnd = CreateEvent(NULL, FALSE, FALSE, "readEnd"),            // Инициализация событий для синхронизации                
            writeEnd = CreateEvent(NULL, FALSE, FALSE, "writeEnd"),
            hExit = CreateEvent(NULL, FALSE, FALSE, "exit"),
            hWork = CreateEvent(NULL, FALSE, FALSE, "work");

        HANDLE hFile = CreateFile("COM1", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); // ОТкрытие канала для чтения и записи
        if (hFile == NULL)
        {
            printf("Error in creating file\n");
            Sleep(2000);
            exit(1);
        }
        SetCommMask(hFile, EV_RXCHAR);                      // Устанавливаем маску на события порта
        SetupComm(hFile, 1500, 1500);                       // Инициализирует коммуникационные параметров
    
        COMMTIMEOUTS CommTimeOuts;                          // Структура, характеризующая временные параметры последовательного порта.
        CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;      // Пары множителей для подсчетов периодов записи/чтения
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0;        
        CommTimeOuts.ReadTotalTimeoutConstant = 1200;      
        CommTimeOuts.WriteTotalTimeoutMultiplier = 0;      
        CommTimeOuts.WriteTotalTimeoutConstant = 1200;      
    
        if (!SetCommTimeouts(hFile, &CommTimeOuts))         // Установка соответсвующих временных парметров
        {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
            printf("Error in work with Handle\n");
            Sleep(2000);
            exit(2);
        }
    
        DCB ComDCM;                               // Структура, характеризующая основные параметры последовательного порта
        memset(&ComDCM, 0, sizeof(ComDCM));       // Выделение памяти под структуру
        ComDCM.DCBlength = sizeof(DCB);           // Задает длину, в байтах, структуры
        GetCommState(hFile, &ComDCM);             // Извлекает данные о текущих настройках управляющих сигналов для указанного устройства
        ComDCM.BaudRate = DWORD(baudrate);        // Скорость передачи данных.
        ComDCM.ByteSize = 8;                      // Определяет число информационных бит в передаваемых и принимаемых байтах.
        ComDCM.Parity = NOPARITY;                 // Определяет выбор схемы контроля четности 
        ComDCM.StopBits = ONESTOPBIT;             // Задает количество стоповых бит 
        ComDCM.fAbortOnError = TRUE;              // Задает игнорирование всех операций чтения/записи при возникновении ошибки
        ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // Задает режим управления обменом для сигнала DTR
        ComDCM.fRtsControl = RTS_CONTROL_DISABLE; // Задает режим управления потоком для сигнала RTS
        ComDCM.fBinary = TRUE;                    // Включает двоичный режим обмена
        ComDCM.fParity = FALSE;                   // Включает режим контроля четности
        ComDCM.fInX = FALSE;                      // Задает использование XON/XOFF управления потоком при приеме
        ComDCM.fOutX = FALSE;                     // Задает использование XON/XOFF управления потоком при передаче
        ComDCM.XonChar = 0;                       // Задает символ XON используемый как для приема, так и для передачи
        ComDCM.XoffChar = (unsigned char)0xFF;    // Задает символ XOFF используемый как для приема, так и для передачи
        ComDCM.fErrorChar = FALSE;                // Задает символ, использующийся для замены символов с ошибочной четностью
        ComDCM.fNull = FALSE;                     // Указывает на необходимость замены символов с ошибкой четности на символ задаваемый полем ErrorChar
        ComDCM.fOutxCtsFlow = FALSE;              // Включает режим слежения за сигналом CTS
        ComDCM.fOutxDsrFlow = FALSE;              // Включает режим слежения за сигналом DSR
        ComDCM.XonLim = 128;                      // Задает минимальное число символов в приемном буфере перед посылкой символа XON
        ComDCM.XoffLim = 128;                     // Определяет максимальное количество байт в приемном буфере перед посылкой символа XOFF
    
        if (!SetCommState(hFile, &ComDCM))        // Установка связи порта 
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



