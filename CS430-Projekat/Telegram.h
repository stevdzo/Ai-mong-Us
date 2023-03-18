#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>

class Telegram {

	int sender;
	int receiver;
	int msg;

public:

	Telegram() : sender(-1),
				receiver(-1),
				msg(-1) {}

	Telegram(int sender,
			int receiver,
			int msg) :
			sender(sender),
			receiver(receiver),
			msg(msg)
	{}
};
#endif