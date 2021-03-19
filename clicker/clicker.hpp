#pragma once

using namespace std::chrono_literals;
using floating_point_ms = std::chrono::duration<float, std::chrono::milliseconds::period>;

class clicker
{
private:
	/// <summary>
	/// Delay for the sleep
	/// </summary>
	float delay = 0.f;

	/// <summary>
	/// Random cps values to add up to current cps
	/// </summary>
	float random = 0.f;

	/// <summary>
	/// Global definition of our current cps
	/// </summary>
	/// float cps = 0.f;

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
	void init();

	/// <summary>
	/// Clicks mouse with randomization
	/// </summary>
	/// <param name="button"></param>
	/// <param name="cps"></param>
	/// <param name="is_first_click"></param>
	void click( bool button, float cps, bool &is_first_click );

	/// <summary>
	/// Separate randomization thread for more accurate randomization algorithm
	/// </summary>
	/// <param name="delay"></param>
	void randomization_thread( float delay );

	~clicker() = default;
	clicker() = default;
};

inline auto g_clicker = std::make_unique<clicker>();