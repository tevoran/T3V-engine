#include <te.hpp>

void t3v::software_rasterizer::vertex_shader(t3v::vertex& vertex)
{
	glm::vec4 vertex4 = glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0);


	vertex4=m_projection_mat * vertex4;


	//perspective divide
	vertex4.x=vertex4.x/vertex4.w;
	vertex4.y=vertex4.y/vertex4.w;
	

	vertex.pos=glm::vec4(vertex4.x+0.5, -vertex4.y+0.5, vertex4.z, vertex4.w); //0.5 for having nicer axes

}