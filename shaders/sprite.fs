#version 410 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
//    aColor = texture(image, TexCoords);
//    if (aColor.a < 1.0)
//        discard;
//    color = aColor;
} 
