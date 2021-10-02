#include <chrono>
#include <iostream>
#include <Windows.h>
#include <random>
#include <string>

constexpr auto VK_KEY_W = 0x57;
constexpr auto VK_KEY_A = 0x41;
constexpr auto VK_KEY_S = 0x53;
constexpr auto VK_KEY_D = 0x44;

void KeyEvent(WORD key, bool press) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = key;
	if (press) {
		//For key press Flag=0
		ip.ki.dwFlags = 0;
	} else {
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
	}
	SendInput(1, &ip, sizeof(INPUT));
}

int random(int min, int max) //range : [min, max]
{
	static bool first = true;
	if (first)
	{
		srand((unsigned int)(time(NULL))); //seeding for the first time only!
		first = false;
	}
	return min + rand() % (max + 1 - min);
}

void MouseMove() {
	INPUT ip;
	ip.type = INPUT_MOUSE;

	ip.mi.mouseData = 0;
	ip.mi.time = 0;
	ip.mi.dwFlags = MOUSEEVENTF_MOVE;
	ip.mi.dx = random(-100, 100);
	ip.mi.dy = random(-100, 100);
	SendInput(1, &ip, sizeof(INPUT));
}

int main() {
	auto t_last = std::chrono::high_resolution_clock::now();
	double timer = 0.0f;

	int nextMoveTime = random(5000, 20000);
	int nextMoveDuration = random(500, 1500);

	int lastKey = 0;
	int direction = 0;

	std::string lastLine = "";
	std::string nextLine = "";

	bool doOnce = false;

	while (true) {
		auto t_now = std::chrono::high_resolution_clock::now();

		double delta_time = std::chrono::duration<double, std::milli>(t_now - t_last).count();
		t_last = t_now;
		timer += delta_time;

		if (timer < nextMoveTime){
			int waitTime = (int)(nextMoveTime / 1000) - (int)(timer / 1000);
			nextLine = "Waiting " + std::to_string(waitTime) + " seconds to move.";
		} else if (timer > nextMoveTime && timer < (double)(nextMoveTime) +(double)(nextMoveDuration)){
			if (!doOnce) {
				doOnce = true;

				//pick a direction and move in it
				direction = random(0, 3);
				switch (direction) {
					case 0:
						lastKey = VK_KEY_W;
						nextLine = "Move up";
						break;

					case 1:
						lastKey = VK_KEY_S;
						nextLine = "Move down";
						break;

					case 2:
						lastKey = VK_KEY_A;
						nextLine = "Move left";
						break;

					case 3:
						lastKey = VK_KEY_D;
						nextLine = "Move right";
						break;
				}

				KeyEvent(lastKey, true);
			}
			MouseMove();
		} else if (timer > (double)(nextMoveTime) +(double)(nextMoveDuration)) {
			doOnce = false;
			timer = 0;
			KeyEvent(lastKey, false);
			nextMoveTime = random(30000, 60000);
			nextMoveDuration = random(1000, 2000);
		}

		if (lastLine != nextLine) {
			if (nextLine.length() < lastLine.length()) {
				int diff = (int)(lastLine.length() - nextLine.length());
				nextLine.append(diff, ' ');
			}

			//system("cls");
			std::cout << nextLine << "\r";

			lastLine = nextLine;
		}
		
		Sleep(16);

	}
}
