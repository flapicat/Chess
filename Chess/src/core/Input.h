#pragma once
#include <unordered_map>

class Input
{
public:
	static bool IsKeyPressed(int keycode);
	static bool IsKeyReleased(int keycode);
	static bool OnKeyPressed(int keycode);
	static bool OnKeyReleased(int keycode);
private:
	static std::unordered_map<int, bool> s_KeyStates;
};