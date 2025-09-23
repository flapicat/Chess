#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out int v_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
	v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = int(a_TexIndex);
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);
    FragColor = texColor * v_Color;
}