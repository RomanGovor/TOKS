#include "WorkWithPort.h"

void loading()                                                              // загрузка
{
    std::cout << "Waiting message.";
    for (size_t i = 0; i < 4; i++)
    {
        std::cout << '.';
        Sleep(250);
    }
    return;
}

void sended()
{
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << '=';
        Sleep(50);
    }
    std::cout << ">\n\n";
    return;
}

void processingPorts(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork)
{
    char buffer[maxBuff];
    int size;
    while (WaitForSingleObject(hExit, 1) == WAIT_TIMEOUT)
    {
        if (WaitForSingleObject(hWork, 1) != WAIT_TIMEOUT)
        {
            loading();
            WaitForSingleObject(writeEnd, INFINITE);
            if (WaitForSingleObject(hExit, 1) != WAIT_TIMEOUT)
                return;
            size = readFromFile(hFile, buffer);
            printf("\nPort 1 >> : ");
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));          
            for (int i = 0; i < size; i++) {
                printf("%c", buffer[i]);
            }             
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
            printf("\n");
            SetEvent(readEnd);
        }
        SetEvent(hWork);
        printf("Me >> ");
        if (!writeToFile(hFile, buffer))
        {
            SetEvent(hExit);
            SetEvent(writeEnd);
            return;
        }
        else
        {
            SetEvent(writeEnd);
            WaitForSingleObject(readEnd, INFINITE);
            sended();
        }
    }
}

void endWork(std::string s, int r)                                  // Завершение работы
{
    if (!r)
        std::cout << "It was a good job";
    else
        std::cout << s << " not work";
}

int readFromFile(HANDLE hFile, char* buffer)                                // Чтение информации с канала
{
    int size;
    DWORD num;
    ReadFile(hFile, &size, 1 * sizeof(int), &num, NULL);      // Получение размера буфера
    ReadFile(hFile, buffer, size * sizeof(char), &num, NULL); // Чтение инфы в буфер
    return size;                                                            // Возврат числа считанных байт
}

bool writeToFile(HANDLE hFile, char* buffer)                                // Запись инфы в файл
{
    char symb;
    int size = 0;
    DWORD num;
    while (true)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 6));
        scanf_s("%c", &symb);
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
        
        if (symb == '\n')
        {
            buffer[size] = '\0';
            WriteFile(hFile, &size, 1 * sizeof(int), &num, NULL);      // Получнеие размера записи
            WriteFile(hFile, buffer, size * sizeof(char), &num, NULL);  // Запись 

            if (!strcmp(buffer, "exit\0"))
                return false;
            else
                return true;
        }
        else
            buffer[size] = symb;
        size++;
    }
}

int setBaudrate()
{
    int baudrate = 300;
    std::cout << "Choose baudrate:\n";
    std::cout << "0 - 4800 \t1 - 9600 \t2 - 19200\t3 - 38400\t4 - 57600\n5 - 76800\t6 - 96000\t7 - 115200\t8 - 256000\n";
    switch (_getch())
    {
    case '0':
        baudrate = 4800;
        break;
    case '1':
        baudrate = 9600;
        break;
    case '2':
        baudrate = 19200;
        break;
    case '3':
        baudrate = 38400;
        break;
    case '4':
        baudrate = 57600;
        break;
    case '5':
        baudrate = 76800;
        break;
    case '6':
        baudrate = 96000;
        break;
    case '7':
        baudrate = 115200;
        break;
    case '8':
        baudrate = 256000;
        break;
    default:
        baudrate = 9600;
        break;
    }
    return baudrate;
}