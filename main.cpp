#include <chrono>
#include <iostream>
#include <Windows.h>
#include <string>

constexpr auto VK_KEY_A = 0x41;
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

int main() {
	auto t_last = std::chrono::high_resolution_clock::now();
	bool running = true;
	double timer = 0.0f;

	std::string lastLine = "";
	std::string nextLine = "";

	bool left_once = false;
	bool right_once = false;

	while (running) {
		auto t_now = std::chrono::high_resolution_clock::now();

		double delta_time = std::chrono::duration<double, std::milli>(t_now - t_last).count();
		t_last = t_now;
		timer += delta_time;

		if (timer < 10000) {
			//waiting
			int waitTime = 60 - (int)(timer/1000);
			nextLine = "Waiting " + std::to_string(waitTime) + " seconds to shuffle.";
		} else if (timer > 10000 && timer < 11000) {
			if (!left_once) {
				left_once = true;
				KeyEvent(VK_KEY_A, true);
				nextLine = "Shuffle left";
			}
		} else if (timer > 11000 && timer < 13000) {
			if (!right_once) {
				left_once = false;
				right_once = true;
				KeyEvent(VK_KEY_A, false);
				KeyEvent(VK_KEY_D, true);
				nextLine = "Shuffle right";
			}
		} else if (timer > 13000 && timer < 14000) {
			if (!left_once) {
				right_once = false;
				left_once = true;
				KeyEvent(VK_KEY_D, false);
				KeyEvent(VK_KEY_A, true);
				nextLine = "Shuffle center";
			}
		} else if (timer > 14000) {
			KeyEvent(VK_KEY_A, false);
			left_once = false;
			timer = 0;
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
