precision mediump float;
varying vec2 passthrough_texcoord;
uniform sampler2D input_texture;

void main()
{
	gl_FragColor = texture2D(input_texture,passthrough_texcoord);

}

