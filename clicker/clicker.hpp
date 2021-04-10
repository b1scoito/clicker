#pragma once

using fl_ms = std::chrono::duration<float, std::chrono::milliseconds::period>;
using namespace hooks::mouse;

class clicker: public singleton<clicker>
{
private:
	/// <summary>
	/// Main clicking mouse function
	/// </summary>
	/// <param name="button"></param>
	/// <param name="cps"></param>
	/// <param name="is_first_click"></param>
	void send_click( bool button, float cps, bool &is_first_click );

	/// <summary>
	/// Precisely sleep for seconds
	/// </summary>
	/// <param name="seconds"></param>
	void precise_timer_sleep( double seconds )
	{
		while (seconds > 5e-3)
		{
			auto start = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
			auto end = std::chrono::high_resolution_clock::now();

			auto observed = (end - start).count() / 1e9;
			seconds -= observed;
		}

		/* ~~ Spin lock */
		auto start = std::chrono::high_resolution_clock::now();
		while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < seconds);
	}

	/// <summary>
	/// Sleep wrapper
	/// </summary>
	/// <param name="ms"></param>
	void sleep_precisely( float ms )
	{
		precise_timer_sleep( static_cast<double>(ms / 1000.f) );
	}

	std::string translate_button( bool button )
	{
		switch (button)
		{
			case 0:
				return "right mouse button";
			case 1:
				return "left mouse button";
		}

		return {};
	}

	std::string translate_boolean( bool b )
	{
		switch (b)
		{
			case 0:
				return "false";
			case 1:
				return "true";
		}

		return {};
	}

	/// <summary>
	/// Delay for the sleep
	/// </summary>
	float delay = 0.f;

	/// <summary>
	/// Random cps values to add up to current cps
	/// </summary>
	float random = 0.f;

	/// <summary>
	/// Has blockhitted at the time
	/// </summary>
	bool blockhitted = false;

	/* ~~ Easy to read variables */
	static constexpr auto right_mouse_button = false;
	static constexpr auto left_mouse_button = true;

	static constexpr auto input_down = true;
	static constexpr auto input_up = false;

	bool is_left_clicking = false;
	bool is_right_clicking = false;
public:

	/// <summary>
	/// Initializes main loop
	/// </summary>
	void init();

	/// <summary>
	/// Separate randomization thread for more accurate randomization algorithm
	/// </summary>
	/// <param name="delay"></param>
	void update_thread( float delay );

	~clicker() = default;
	clicker() = default;
};