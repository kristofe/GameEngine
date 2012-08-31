attribute vec4 input_position;
attribute vec2 input_texcoord;
varying vec2 passthrough_texcoord;

void main()
{
	gl_position = input_position;
	passthrough_texcoord = input_texcoord;
}
