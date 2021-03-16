#pragma once

// For timeBeginPeriod
#include <timeapi.h>
#pragma comment(lib, "Winmm.lib")

using namespace std::chrono_literals;

class clicker
{
private:
	/// <summary>
	/// Delay for the sleep
	/// </summary>
	int delay = 0;

	/// <summary>
	/// Random cps values
	/// </summary>
	int random = 0;

	/// <summary>
	/// Has blockhitted at the time
	/// </summary>
	bool blockhitted = false;

	bool right_mb = false;
	bool left_mb = true;

	bool input_down = true;
	bool input_up = false;
public:
	/// <summary>
	/// Initializes main loop
	/// </summary>
	void init( );

	/// <summary>
	/// Clicks mouse with randomization
	/// </summary>
	/// <param name="button"></param>
	/// <param name="cps"></param>
	/// <param name="is_first_click"></param>
	void click( bool button, int cps, bool &is_first_click );

	/// <summary>
	/// Clicks a mouse button
	/// </summary>
	/// <param name="is_down"></param>
	/// <param name="button"></param>
	void click_mouse( bool is_down, bool button );

	/// <summary>
	/// Separate randomization thread for more accurate randomization algorithm
	/// </summary>
	/// <param name="delay"></param>
	void randomization_thread( int delay );

	~clicker( ) = default;
	clicker( ) = default;
};

inline auto g_clicker = std::make_unique<clicker>( );