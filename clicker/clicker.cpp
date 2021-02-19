#include "clicker.hpp"

void clicker::work( )
{
	while ( true )
	{
		// Please PR if you found a better way to do this. CPU Usage is high without.
		sleep( 1 );

		if ( vars::b_hotkey_enabled ) // if hotkey is enabled, and selected window is active
		{
			if ( util::get_active_window_title( ).find( config.clicker.window_title ) != std::string::npos )
			{
				if ( config.clicker.left_enabled && vars::b_l_mouse_down && !vars::b_r_mouse_down ) // if left is enabled, left mouse is down and right mouse isn't down start clicking.
				{
					g_clicker->click( left_mb, config.clicker.l_cps, vars::b_l_first_click ); // click!
				}

				if ( config.clicker.right_enabled && vars::b_r_mouse_down ) // if right is enabled, right mouse is down start clicking.
				{
					g_clicker->click( right_mb, config.clicker.r_cps, vars::b_r_first_click ); // click!
				}
			}
		}
	}
}

void clicker::click( bool button, int cps, bool &is_first_click )
{
	if ( cps <= 0 ) // return if our cps is 0. let's not divide it by 0.
		return;

	if ( !config.clicker.blatant ) // if blatant is not enabled randomize values.
	{
		random = util::random_int( -( cps / 5 ), ( cps / 5 ) ); // random int between negative 1 / 5 of our cps and 1 / 5 of our cps
		cps += random; // add our values to the i_cps variable.

		// TODO:
		/*
		if ( config.clicker.cps_drop_chance && config.clicker.cps_drop_chance_val > 0 && std::rand( ) % ( 100 / config.clicker.cps_drop_chance_val ) == 0 )
		{
			// cps drop code
		}

		if ( config.clicker.cps_spike_chance && config.clicker.cps_spike_chance_val > 0 && std::rand( ) % ( 100 / config.clicker.cps_spike_chance_val ) == 0 )
		{
			// cps spike code
		}
		*/
	}

	delay = ( 1000 / cps ) / 2; // delay is half because we'll be calling it two times, on mouse down and up.

	if ( is_first_click ) // if it's our first click, delay and send a up input to our function.
	{
		sleep( delay );
		g_clicker->click_mouse( input_up, button );
		is_first_click = false;
		_log( LDEBUG, "[ clicker ] first click." );
	}

	sleep( delay ); // sleep on input down
	g_clicker->click_mouse( input_down, button ); // the actual input down call

	if ( config.clicker.blockhit && config.clicker.blockhit_chance > 0 && std::rand( ) % ( 100 / config.clicker.blockhit_chance ) == 0 ) // if blockhit is enabled and blockhit chance is higher than 0 and blockhit chance matches
	{
		blockhitted = true; // set blockhitted bool to true
		g_clicker->click_mouse( input_down, right_mb ); // right click
	}

	sleep( delay ); // sleep on input up
	g_clicker->click_mouse( input_up, button ); // the actual input up call

	if ( blockhitted ) // check if blockhitted bool was true
	{
		g_clicker->click_mouse( input_up, right_mb ); // right click up
		blockhitted = false; // set blockhitted variable back to false.
	}


	_log( LDEBUG, "[ clicker ] cps: %d delay %d.", cps, delay );
}

void clicker::click_mouse( bool is_down, bool button )
{
	is_down ? ( button ? g_mouse->left_down( ) : g_mouse->right_down( ) ) : ( button ? g_mouse->left_up( ) : g_mouse->right_up( ) );
	++vars::i_clicks_this_session;
}