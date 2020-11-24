#include "windows.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include "TickerPlant.h" // contains struct Tick
const char KEY_ESC = 27;

struct Bar
{
	double open = 0;
	double close = 0;
	double high = 0;
	double low = DBL_MAX;
};

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Invalid arguments" << std::endl;
		return -1;
	}
	const time_t timeInterval = atoi(argv[1]);

	bool isOpenTick = true;
	tm openTickTIme;
	Bar bar;
	MSG msg;
	Tick* tick;

	StartFeed();
	while (true) //ESC to quit
	{
		GetMessage(&msg, 0, 0, 0);
		tick = reinterpret_cast<Tick*>(msg.lParam);

		if (mktime(&tick->tickTime) - mktime(&openTickTIme) >= timeInterval && !isOpenTick)
		{
			isOpenTick = true;
			std::cout << "Bar closed [" << bar.open << "; " << bar.high << "; "
				<< bar.low << "; " << bar.close << "]" << std::endl;
			bar.high = 0;
			bar.low = DBL_MAX;
		}

		if (isOpenTick)
		{
			isOpenTick = false;
			openTickTIme = tick->tickTime;
			bar.open = tick->tickPrice;
		}

		bar.high = bar.high < tick->tickPrice ? tick->tickPrice : bar.high;
		bar.low = bar.low > tick->tickPrice ? tick->tickPrice : bar.low;
		bar.close = tick->tickPrice;

		std::cout << tick->tickPrice << " @ " << std::put_time(&tick->tickTime, "%H:%M:%S") << std::endl;
		delete tick;

		if (_kbhit() && (_getch() == KEY_ESC))
		{
			StopFeed();
			break;
		}
	}

	return 0;
}
