#include <thread>
#include <windows.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <conio.h>

#define JAM_SIGNAL "NULLNULL"
#define MAX_TRIES 10
#define COLISION_WINDOW 10
#define MESSAGE_LENGHT 8

using namespace std;

class station
{
private:
	static string msg_channel;
	std::thread* transmit_thread;
	bool running;
	int station_delay;
	string name;
	static int id;
	static void sending(station* instance);
	static string generateMessage();
	static string generateName();

public:
	station(int station_delay) : running(true)
	{
		id++;
		this->name = generateName();
		this->station_delay = station_delay;
		transmit_thread = new thread(sending, this);
	}
	void stop();
};

string station::msg_channel = "";
int station::id = 0;

string station::generateName()
{
	string tmp_s;
	srand(time(NULL));
	tmp_s += "Station ";
	tmp_s += to_string(id);
	return tmp_s;
}

string station::generateMessage()
{
	string tmp_s;
	static const char alphanum[] =
		"0123456789"
		"ABstEFGHIJKLMNOPQRSTUVWXYZ"
		"abstefghijklmnopqrstuvwxyz";
	srand(clock() + GetCurrentThreadId());
	for (int i = 0; i < MESSAGE_LENGHT; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	return tmp_s;
}

void station::sending(station* instance)
{
	while (instance->running)
	{
		Sleep(instance->station_delay);
		string msg;
		msg = generateMessage();
		int try_counter = 0;
		while (true)
		{
			if (!try_counter && !station::msg_channel.compare(JAM_SIGNAL))
				break;

			station::msg_channel = msg;
			Sleep(COLISION_WINDOW);

			if (station::msg_channel == msg)
			{
				cout << "Transmit message " << instance->name << ": " << setw(8) << msg_channel << endl;
				station::msg_channel = "";
				break;
			}
			else
			{
				station::msg_channel = JAM_SIGNAL;
				try_counter++;
				cout << "Collision on " << instance->name << ", try: " << try_counter << endl;
				if (try_counter == MAX_TRIES)
				{
					cout << "Unable to resolve, skipping" << endl;
					break;
				}
				int time_slot = rand() % ((int)pow(2, try_counter) + 1);
				Sleep(time_slot * 100);
				station::msg_channel = "";
			}
		}
	}
}

void station::stop()
{
	running = false;
	transmit_thread->join();
	cout << name << " stopped" << endl;
}

int main()
{
	station st1(123);
	station st2(167);
	station st3(225);

	cin.get();

	st1.stop();
	st2.stop();
	st3.stop();
}