#include "renderer.h"


void glUniformMat4fWrapper(GLint location, GLsizei count, GLboolean transpose, void *matrix)
{
	glUniformMatrix4fv(location, count, transpose, &(*(mat4*)matrix)[0]);
}

void glUniformMat3fWrapper(GLint location, GLsizei count, GLboolean transpose, void *matrix)
{
	glUniformMatrix3fv(location, count, transpose, &(*(mat3*)matrix)[0]);
}

Renderer::Renderer()
{
	camera.position = vec3(0.f, 0.f, 2.f);
	camera.rotation = vec3(0.f, 0.f, 0.f);
	camera.direction = vec3(0.f, 0.f, -1.f);

}

void Renderer::submit(Layer* l)
{
	layers.push_back(l);
}

void Renderer::set_accept_light()
{
	for (deque<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it) {
		(*it)->set_accept_light(true);
	}
}

void Renderer::flush()
{
	if (animated) {
		static double timer = 0;
		static double start = glfwGetTime();
		if (current_frame < frame_count) {
			timer = glfwGetTime() - start;
			if (timer > current_frame * (2.0 / 30.0)) {
				//camera.position[2] += 0.1;
				current_frame += 1;
			}
		}
		else {
			current_frame = 0;
			timer = 0;
			start = glfwGetTime();
		}
	}

	mat4 vw = mat4::look_at(camera.position, camera.position + camera.direction, vec3(0, 1, 0));
	for (deque<Layer*>::iterator it = layers.begin(); it != layers.end(); ++it) {
		glUniform1i(use_texture_loc, (*it)->textures!=NULL);
		glUniform1i(use_light_loc, (*it)->accept_light);
		mat4 uploaded;
		if (!(*it)->key_position.empty()) {
			vec3 pos = (*it)->key_position[0].second * (current_frame / (float)frame_count);
			uploaded = vw * mat4::translation_matrix(pos);
		}
		else {
			uploaded = vw * mat4::identity_matrix();
		}
		//cout << it - layers.begin() << ": " << ((*it)->get_transformation_matrix()) << endl;
		uploaded = uploaded * (*it)->get_transformation_matrix();
		//cout << it - layers.begin() << ": " << vw << endl;
		matrices[0].matrix = &uploaded;
		matrices[0].upload_matrix();

		mat3 un = mat3::from_mat4(uploaded);
		un = (un.transpose()).invert();
		matrices[1].matrix = &un;
		matrices[1].upload_matrix();

		(*it)->draw();
	}
}

void Renderer::register_camera(const Camera& c)
{

}

void Renderer::move(int move_from, int move_to)
{
	deque<Layer*>::iterator it = layers.begin() + move_from;
	Layer *cur = *it;
	layers.erase(it);
	it = layers.begin() + move_to;
	layers.insert(it, cur);
}

Renderer::~Renderer()
{
	/*for (unsigned int i = 0; i < matrices.size(); i++) {
		delete matrices[i].matrix;
	}*/
	for (unsigned int i = 0; i < layers.size(); i++) {
		delete layers[i];
	}
}

void Renderer::init_shader(const char *vert_fp, const char *frag_fp)
{
	shader_id = shader.load_shaders(vert_fp, frag_fp);

	float ratio = 1280.f / 720.f;
	mat4 pr = mat4::perspective_matrix(degreeToRadius(45), ratio, .1f, 500.f);
	glUniformMatrix4fv(glGetUniformLocation(shader_id, "pr_matrix"), 1, GL_FALSE, &pr.elements[0]);


	ShaderMatrix view(shader_id, "vw_matrix");
	view.myf = &glUniformMat4fWrapper;
	ShaderMatrix model(shader_id, "un_matrix");
	model.myf = &glUniformMat3fWrapper;

	matrices.push_back(view);
	matrices.push_back(model);
}

void Renderer::init_camera(vec3 pos = vec3(0.f, 0.f, 1.f))
{
	camera.move(pos[0], pos[1], pos[2]);
}

void Renderer::init_uniform_locations()
{
	use_texture_loc = glGetUniformLocation(shader_id, "use_texture");
	use_light_loc = glGetUniformLocation(shader_id, "use_light");
}