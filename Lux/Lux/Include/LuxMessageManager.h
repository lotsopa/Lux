#ifndef LUX_MESSAGE_MANAGER_H
#define LUX_MESSAGE_MANAGER_H

namespace Lux
{
	namespace Core
	{
		class MessageManager
		{
		public:
			MessageManager();
			~MessageManager();

			enum MessageType
			{
				MSG_MESH_SET = 0,
				MSG_PHYS_MAT_SET
			};

		private:
		};
	}
}

#endif