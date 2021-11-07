#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace engine {
	class Geometry
	{
	public:

		unsigned int vao;

		#pragma region constructors
		Geometry()
		{
			vao = 0;
		}
		Geometry(unsigned int _vao)
		{
			vao = _vao;
		}
		#pragma endregion
	};
}
#endif
