#pragma once

template<typename T>
class singleton
{
protected:
	singleton( ) = default;
	~singleton( ) = default;

	singleton( const singleton & ) = delete;
	singleton &operator=( const singleton & ) = delete;

	singleton( singleton && ) = delete;
	singleton &operator=( singleton && ) = delete;
public:
	static T &get( )
	{
		static T inst {};
		return inst;
	}
};
