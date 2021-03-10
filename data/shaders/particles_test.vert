#version 450 core

// --vv-- I/O data --vv--

in int gl_VertexID;

uniform mat4 vp_transform;

struct Transform
{
	vec3 pos;
	vec4 rot;
	vec3 scale;
};

layout(std430, binding = 3) buffer Transforms
{
	Transform trasforms[];
};

varying vec4 vertex_color;

// --^^-- I/O data --^^--


// --vv-- Static data --vv--

#define PARTICLE_RADIUS 0.01f
#define POLYGON_SIZE 6

#define COLOR_RED vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE vec4(0.0f, 0.0f, 1.0f, 1.0f)

vec4 vertices_color[POLYGON_SIZE] = vec4[](COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_BLUE, COLOR_GREEN, COLOR_RED);
float triangle_angles[POLYGON_SIZE] = float[](0.0f, 90.0f, 180.0f, 180.0f, 270.0f, 0.0f);
//vec2 uv_coord[POLYGON_SIZE] = vec2[](vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f), vec2(0.0f, 0.0f));

// --^^-- Static data --^^--


// --vv-- Quaternion functions --vv--

vec4 quat_from_axis_angle(vec3 axis, float angle)
{
	vec4 qr;
	float half_angle = (angle * 0.5) * 3.14159 / 180.0;
	qr.x = axis.x * sin(half_angle);
	qr.y = axis.y * sin(half_angle);
	qr.z = axis.z * sin(half_angle);
	qr.w = cos(half_angle);
	return qr;
}

vec4 quat_conj(vec4 q)
{
	return vec4(-q.x, -q.y, -q.z, q.w); 
}

vec4 quat_mult(vec4 q1, vec4 q2)
{
	vec4 qr;
	qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
	qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
	qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
	qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{
	vec4 qr = quat_from_axis_angle(axis, angle);
	vec4 qr_conj = quat_conj(qr);
	vec4 q_pos = vec4(position.x, position.y, position.z, 0);
	
	vec4 q_tmp = quat_mult(qr, q_pos);
	qr = quat_mult(q_tmp, qr_conj);
	
	return vec3(qr.x, qr.y, qr.z);
}

// --^^-- Quaternion functions --^^--


void main()
{
	uint particle_index = gl_VertexID / POLYGON_SIZE;
	uint vertex_polygon_index = gl_VertexID - particle_index * POLYGON_SIZE;
	vec3 particle_position = trasforms[particle_index].pos;

	vec3 rotation_axis = vec3(0.0f, 0.0f, 1.0f);
	vec3 vertex_position = particle_position 
		+ rotate_vertex_position(vec3(PARTICLE_RADIUS, 0.0f, 0.0f), rotation_axis, triangle_angles[vertex_polygon_index]);

	gl_Position = vp_transform * vec4(vertex_position, 1.0f);
	vertex_color = vertices_color[vertex_polygon_index];
}