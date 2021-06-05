#include <te.hpp>

void t3v::software_rasterizer::rasterize_triangle(
	t3v::vertex vertex1,
	t3v::vertex vertex2,
	t3v::vertex vertex3,
	render_thread_data *data)
{
	//sorting vertices along y-axis
	if(vertex1.pos.y > vertex2.pos.y)
	{
		std::swap(vertex1, vertex2);
	}
	if(vertex2.pos.y > vertex3.pos.y)
	{
		std::swap(vertex2, vertex3);
	}
	if(vertex1.pos.y > vertex2.pos.y)
	{
		std::swap(vertex1, vertex2);
	}

	//converting vertices from clip space to screen space
	glm::ivec2 vertex1_screen{vertex1.pos.x*data->resx+0.5,vertex1.pos.y*data->resy+0.5};
	glm::ivec2 vertex2_screen{vertex2.pos.x*data->resx+0.5,vertex2.pos.y*data->resy+0.5};
	glm::ivec2 vertex3_screen{vertex3.pos.x*data->resx+0.5,vertex3.pos.y*data->resy+0.5};


	//determining bounding box for the render loop
	//this includes the drawing area for this thread
	int x_bounding_start=vertex1_screen.x;
	if(vertex2_screen.x<x_bounding_start)
	{
		x_bounding_start=vertex2_screen.x;
	}
	if(vertex3_screen.x<x_bounding_start)
	{
		x_bounding_start=vertex3_screen.x;
	}
	if(x_bounding_start<0)
	{
		x_bounding_start=0;
	}
			
	int x_bounding_end=vertex1_screen.x;
	if(vertex2_screen.x>x_bounding_end)
	{
		x_bounding_end=vertex2_screen.x;
	}
	if(vertex3_screen.x>x_bounding_end)
	{
		x_bounding_end=vertex3_screen.x;
	}
	if(x_bounding_end>=data->resx)
	{
		x_bounding_end=data->resx;
	}

	int y_bounding_start=vertex1_screen.y;
	if(y_bounding_start<data->y_start)
	{
		y_bounding_start=data->y_start;
	}

	int y_bounding_end=vertex3_screen.y;
	if(y_bounding_end>data->y_end)
	{
		y_bounding_end=data->y_end;
	}

	//determining barycentric division constant for the triangle
	const float div_const=t3v::barycentric_interpolation_optimized_div(vertex1_screen, vertex2_screen, vertex3_screen);

	glm::ivec2 pixel_draw={0,0};
	float a,b,c; //barycentric coordinates
	float d_a,d_b,d_c; //barycentric coordinates

	data->color={vertex1.color.b, vertex1.color.g, vertex1.color.r, 0};

	//rasterizing loop
	for(int iy = y_bounding_start; iy < y_bounding_end; iy++)
	{
		pixel_draw.y=iy;

		bool has_drawn=false; //is used for determining the end of the line
		uint32_t *pixel_ptr=(uint32_t*)data->window_surface->pixels+x_bounding_start+pixel_draw.y*data->resx;

		pixel_draw.x=x_bounding_start;

		//barycentric coordinates line preparation
		t3v::barycentric_interpolation_line_optimized(
			vertex1_screen,
			vertex2_screen,
			vertex3_screen,
			pixel_draw,
			div_const,
			a, b, c,
			d_a, d_b, d_c);

		//z-buffering line preparation
		//INT32_MAX is my max used z-buffer value for optimal clipping
		float z_tmp_1=t3v::barycentric_interpolate_value(a,b,c,vertex1.pos.z,vertex2.pos.z,vertex3.pos.z);
		uint32_t z=z_tmp_1*INT32_MAX; //calculating line beginning

		float z_tmp_2=t3v::barycentric_interpolate_value(a+d_a,b+d_b,c+d_c,vertex1.pos.z,vertex2.pos.z,vertex3.pos.z);
		uint32_t z_tmp_int_2=z_tmp_2*INT32_MAX;

		uint32_t z_delta=z_tmp_int_2-z; //calculating difference for each following pixel in a line
		int offset=x_bounding_start+iy*data->resx; //pixel offset in the z-buffer

		for(int ix=x_bounding_start; ix<x_bounding_end; ix++)
		{
			if(a>0 && b>0 && c>0)
			{
				//z-buffer check
				if(z < data->z_buffer[offset])
				{
					//writing z-buffer value
					data->z_buffer[offset]=z;
					has_drawn=true;
					draw_pixel_fast_simple(pixel_ptr, data->color);
				}
			}
			else
			{
				//if end of line is reached then go to next line
				if(has_drawn==true)
				{
					break;
				}
			}
			//barycentric coordinates line increments
			a+=d_a;
			b+=d_b;
			c+=d_c;

			//z-buffer line increments
			z+=z_delta;
			offset++;

			//pixel draw pointer increment
			pixel_ptr++;
		}
	}
}