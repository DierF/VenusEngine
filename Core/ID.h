#pragma once

namespace VenusEngine
{
	class ID
	{
	public:
		static int generateID()
		{
			static int currentID = 1;
			return currentID++;
		}
	};
}