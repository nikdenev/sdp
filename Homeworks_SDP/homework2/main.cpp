#include <iostream>
#include <fstream>
#include <cassert>

#include "List.h"

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((y < x) ? x : y)

enum Direction
{
	DOWN,
	UP,
	MEANINGLESS,
	UNKNOWN
};

class Request
{
public:
	Request() :
		floor(0),
		time(0),
		direction(UNKNOWN)
	{}

	void setFloor(int floor) { this->floor = floor; }
	void setTime(int time) { this->time = time; }
	void setDirection(Direction direction) { this->direction = direction; }

	int getFloor() const { return this->floor; }
	int getTime() const { return this->time; }
	Direction getDirection() const { return this->direction; }

	void read(std::ifstream& stream)
	{
		char str[16];

		stream >> str;

		if (strcmp(str, "call") == 0)
		{
			stream >> str;

			if (strcmp(str, "up") == 0)
				this->direction = UP;
			else
				this->direction = DOWN;
		}
		else
			this->direction = MEANINGLESS;

		stream >> this->floor >> this->time;
	}

private:
	int floor;
	int time;
	Direction direction;
};

void print(int time, int floor, bool goesUp)
{
	std::cout <<time << ' ' << floor << ' ';

	if (goesUp)
		std::cout << "up";
	else
		std::cout << "down";

	std::cout << '\n';
}

void moveElevator(const List<Request>& list, int& floor, bool& goesUp, int& after)
{
	//move the elevator if neccasary
	if (!list.empty() && list.front().getFloor() != floor)
	{
		++after;
		if (floor < list.front().getFloor())
		{
			goesUp = true;
			if (after == 5)
			{
				after = 0;
				++floor;
			}
		}
		else if (floor > list.front().getFloor())
		{
			goesUp = false;
			if (after == 5)
			{
				after = 0;
				--floor;
			}
		}
	}
}

void insertInList(List<Request>& list, const Request& request, 
	int currentFloor, int after, Direction elevatorsDirection)
{
	if (list.empty())
	{
		list.push_back(request);
	}
	else
	{
		int destinationFloor = list.front().getFloor();
		int floor = request.getFloor();
		Direction direction = request.getDirection();

		if (after == 0 && (destinationFloor == floor || floor == currentFloor))
			list.push_front(request);

		else if (MIN(destinationFloor, currentFloor) < floor &&
			floor < MAX(destinationFloor, currentFloor))
		{
			if (direction == elevatorsDirection || direction == MEANINGLESS)
				list.push_front(request);
			else
				list.push_back(request);
		}
		else
		{
			list.push_back(request);
		}
	}
}

void reorganizeList(List<Request>& list, int currentFloor)
{
	if (!list.empty())
	{
		int destination = list.front().getFloor();

		List<Request>::Iterator i = list.begin();

		if (currentFloor < destination)
		{
			List<Request>::Iterator min = i;

			for (; i != list.end(); ++i)
			{
				if ((*i).getFloor() < destination && currentFloor < (*i).getFloor() &&
					(*i).getFloor() < (*min).getFloor() && (*i).getDirection() != DOWN)
					min = i;
			}

			Request temp = *min;
			list.erase(min);
			list.push_front(temp);
		}
		else
		{
			List<Request>::Iterator max = i;

			for (; i != list.end(); ++i)
			{
				if ((*i).getFloor() < currentFloor && destination < (*i).getFloor() &&
					(*max).getFloor() < (*i).getFloor() && (*i).getDirection() != UP)
					max = i;
			}
			
			Request temp = *max;
			list.erase(max);
			list.push_front(temp);
		}

		i = list.begin();
		int front = (*i).getFloor();

		while (i != list.end() && (*i).getFloor() == front) ++i;

		while (i != list.end())
		{
			if (front == (*i).getFloor())
			{
				Request temp = *i;
				list.erase(i++);
				list.push_front(temp);
			}
			else
				++i;
		}
	}
}

void mainLoop(std::ifstream& stream)
{
	int n, k;

	stream >> n >> k;

	Request currentRequest;
	currentRequest.read(stream);
	if (!stream)
	{
		std::cout << "Error! problem with the file\n";
		return;
	}
	
	List<Request> list;

	bool goesUp = false;
	int currentTime = 1;
	int currentFloor = 1;
	int afterCurrentFloor = 0;

	while (stream || !list.empty())
	{
		moveElevator(list, currentFloor, goesUp, afterCurrentFloor);

		if (stream && currentTime == currentRequest.getTime())
		{
			insertInList				//if goesUp == true direction is UP else DOWN
			(list, currentRequest, currentFloor, afterCurrentFloor, (Direction)goesUp);

			currentRequest.read(stream);
		}

		if (!list.empty())
		{
			if (list.front().getFloor() == currentFloor && afterCurrentFloor == 0)
			{
				print(currentTime, currentFloor, goesUp);

				while (!list.empty() && list.front().getFloor() == currentFloor)
					list.pop_front();

				reorganizeList(list, currentFloor);
			}
		}

		++currentTime;
	}

	if (!stream.eof())
	{
		std::cout << "Error! problem with the file\n";
		return;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Error! Problem with argoments of main\n";
		return 0;
	}

	std::ifstream stream(argv[1]);
	if (!stream)
	{
		std::cout << "Problem with file " << argv[1] << "\n";
		return 0;
	}

	mainLoop(stream);

	return 0;
}