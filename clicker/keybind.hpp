#pragma once

enum keybind_state_t: int
{
	always = 0,
	hold,
	toggle
};

class keybind
{
public:
	bool b_state = false;
	bool b_is_down = false;

	int i_mode = 0;
	int i_key = 0;

	keybind() = default;
	explicit keybind( bool state, int mode = 0, int key = 0 )
	{
		b_state = state;
		i_mode = mode;
		i_key = key;
	}

	inline bool get()
	{
		if ( i_key == 0 )
			return b_state;

		switch ( i_mode )
		{
			case keybind_state_t::always:
				b_state = true;
				break;
			case keybind_state_t::hold:
				b_state = GetAsyncKeyState( i_key ) & 0x8000;
				break;

			case keybind_state_t::toggle:
				auto h_state = GetAsyncKeyState( i_key );
				if ( h_state & 0x8000 )
					b_is_down = true;

				if ( b_is_down && !( h_state & 0x8000 ) )
				{
					b_state = !b_state;
					b_is_down = false;
				}
				break;
		}

		return b_state;
	}

	inline std::string get_mode_as_string() const
	{
		switch ( i_mode )
		{
			case keybind_state_t::always: return std::string( "on" );
			case keybind_state_t::hold: return std::string( "hold" );
			case keybind_state_t::toggle: return std::string( "toggle" );
		}

		return {};
	}
};