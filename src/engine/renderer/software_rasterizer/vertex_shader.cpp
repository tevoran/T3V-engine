#include <te.hpp>

void t3v::software_rasterizer::vertex_shader(t3v::vertex& vertex)
{
//	std::cout << vertex.pos.x << std::endl;

	glm::vec4 vertex4 = glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 0);
	
	//perspective divide
	vertex4.x=vertex4.x/vertex4.z;
	vertex4.y=vertex4.y/vertex4.z;

	vertex4=m_projection_mat * vertex4;

	vertex.pos=glm::vec3(vertex4.x+0.5, vertex4.y+0.5, vertex4.z); //0.5 for having nicer axes

}