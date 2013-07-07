
uniform sampler2D diffuseMap;

void main()
{
	vec4 color = texture2D(diffuseMap, gl_TexCoord[0].st);
	gl_FragColor = color;
}
