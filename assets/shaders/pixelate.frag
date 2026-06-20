#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float pixels;

out vec4 finalColor;

void main() {
//    float pixels = 2048.0;
    float dx = 15.0 * (1.0 / pixels);
    float dy = 15.0 * (1.0 / pixels);
    vec2 coord = vec2(dx * floor(fragTexCoord.x / dx), dy * floor(fragTexCoord.y / dy));

    finalColor = texture(texture0, coord);
}