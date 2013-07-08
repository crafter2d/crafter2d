
uniform sampler2D diffuseMap;

in Data
{
	vec2 texCoord;
} DataIn;

out vec4 frag;

void main()
{
	vec4 color = texture2D(diffuseMap, DataIn.texCoord);
	frag = color;
}
