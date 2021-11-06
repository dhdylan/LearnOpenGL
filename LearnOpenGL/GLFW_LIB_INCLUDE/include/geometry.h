#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace engine {
	class Geometry
	{
	public:
		#pragma region setters/getters
		void set_vao(unsigned int _vao)
		{
			vao = _vao;
		}
		unsigned int get_vao()
		{
			return vao;
		}
		#pragma endregion

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

	private:
		unsigned int vao;
	};
}
#endif
