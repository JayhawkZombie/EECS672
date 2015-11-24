#version 410 core

in vec4 colorToFS;

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(colorToFS);
}
