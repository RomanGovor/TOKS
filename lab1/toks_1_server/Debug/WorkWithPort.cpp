#include "WorkWithPort.h"

int readFromFile(HANDLE hFile, char* buffer)
{
    int size;
    DWORD numberOfBytesRead;
    ReadFile(hFile, &size, 1 * sizeof(int), &numberOfBytesRead, NULL);      // Получение размера выхожного буфера
    ReadFile(hFile, buffer, size * sizeof(char), &numberOfBytesRead, NULL); // Чтение информации
    return size;
}

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

bool writeToFile(HANDLE hFile, char* buffer)
{
    while (true)
    {
        char symb;
        int size = 0;
        scanf_s("%c", &symb);
        if (symb == '\n')
        {

            DWORD numberOfBytesWrite;
            buffer[size] = '\0';
            WriteFile(hFile, &size, 1 * sizeof(int), &numberOfBytesWrite, NULL);      // получение размера буфера записи
            WriteFile(hFile, buffer, size * sizeof(char), &numberOfBytesWrite, NULL); // Передачи информациип по каналу
            if (!strcmp(buffer, "close\0"))
                return false;
            else
                return true;
        }
        else
            buffer[size] = symb;
        size++;
    }
}

void processingPorts(HANDLE readEnd, HANDLE writeEnd, HANDLE hFile, HANDLE hExit, HANDLE hWork, std::string mes)
{
    char buffer[maxBuff];
    int size;

    while (WaitForSingleObject(hExit, 2) == WAIT_TIMEOUT)
    {
        if (WaitForSingleObject(hWork, 2) != WAIT_TIMEOUT)
        {
            loading();
            WaitForSingleObject(writeEnd, INFINITE);
            if (WaitForSingleObject(hExit, 2) != WAIT_TIMEOUT)
                return;
            size = readFromFile(hFile, buffer);
            std::cout << mes;
            for (int i = 0; i < size; i++)
                printf("%c", buffer[i]);
            printf("\n");
            SetEvent(readEnd);
        }
        SetEvent(hWork);
        printf("Message: ");
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
            puts("Sended!");
        }
    }
}

void endWork(std::string s, int r)
{
    if (!r)
        std::cout << "It was a good job";
    else
        std::cout << s << " not work";
}