#pragma once

namespace VenusEngine
{
	class ID
	{
	public:
		static unsigned generateID()
		{
			static unsigned currentID = 0;
			return currentID++;
		}
	};
}