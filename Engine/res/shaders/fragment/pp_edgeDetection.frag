#version 460 core

const float OFFSET = 1.0 / 300.0;

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D u_colorTexture;

void main()
{
vec2 offsets[9] = vec2[](
        vec2(-OFFSET,  OFFSET), // top-left
        vec2( 0.0f,    OFFSET), // top-center
        vec2( OFFSET,  OFFSET), // top-right
        vec2(-OFFSET,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( OFFSET,  0.0f),   // center-right
        vec2(-OFFSET, -OFFSET), // bottom-left
        vec2( 0.0f,   -OFFSET), // bottom-center
        vec2( OFFSET, -OFFSET)  // bottom-right    
    );

    float kernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_colorTexture, texCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    fragColor = vec4(col, 1.0);
}