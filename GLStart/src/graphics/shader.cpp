#include "shader.h"

Shader::Shader(const char * vertex_fp, const char * fragment_fp):
	vert_path(vertex_fp), frag_path(fragment_fp)
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//shader = load_shaders();
}

GLuint Shader::load_shaders()
{
	string vs = read_file(vert_path);
	char *vert_shader_text = new char[vs.size()];
	strcpy(vert_shader_text, vs.c_str());

	shader_id = glCreateProgram();
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


	GLint info_log_length;

	glShaderSource(vertexShaderID, 1, &vert_shader_text, NULL);
	glCompileShader(vertexShaderID);

	//glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		vector<char> vertex_shader_errors(info_log_length + 1);
		glGetShaderInfoLog(vertexShaderID, info_log_length, NULL, &vertex_shader_errors[0]);
		printf("%s\n", &vertex_shader_errors[0]);
	}

	string fs = read_file(frag_path);
	char *frag_shader_text = new char[fs.size()];
	strcpy(frag_shader_text, fs.c_str());

	glShaderSource(fragmentShaderID, 1, &frag_shader_text, NULL);
	glCompileShader(fragmentShaderID);

	//glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &info_log_length);

	if (info_log_length > 0) {
		vector<char> fragment_shader_errors(info_log_length + 1);
		glGetShaderInfoLog(fragmentShaderID, info_log_length, NULL, &fragment_shader_errors[0]);
		printf("%s\n", &fragment_shader_errors[0]);
	}

	glAttachShader(shader_id, vertexShaderID);
	glAttachShader(shader_id, fragmentShaderID);
	glLinkProgram(shader_id);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	delete[] vert_shader_text;
	delete[] frag_shader_text;
	return shader_id;
}

int Shader::init_matrices(const char * pr, const char * vw, const char * ml)
{
	pr_matrix_id = glGetUniformLocation(shader_id, pr);
	vw_matrix_id = glGetUniformLocation(shader_id, vw);
	ml_matrix_id = glGetUniformLocation(shader_id, ml);
	return 0;
}

void Shader::upload_pr_matrix(const mat4& m) {
	glUniformMatrix4fv(pr_matrix_id, 1, GL_FALSE, &m.elements[0]);
}

void Shader::upload_vw_matrix(const mat4& m) {
	glUniformMatrix4fv(vw_matrix_id, 1, GL_FALSE, &m.elements[0]);
}

void Shader::upload_ml_matrix(const mat4& m) {
	glUniformMatrix4fv(ml_matrix_id, 1, GL_FALSE, &m.elements[0]);
}

Shader::~Shader()
{

}
