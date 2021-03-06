#version 330

uniform vec3			iResolution;			// viewport resolution (in pixels)
uniform float			iGlobalTime;			// shader playback time (in seconds)
uniform float			iChannelTime[4];		// channel playback time (in seconds)
uniform vec3			iChannelResolution[4];          // channel resolution (in pixel)
uniform vec4			iMouse;				// mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2D		iChannel0;			// input channel. XX = 2D/Cube
uniform sampler2D		iChannel1;			// input channel. XX = 2D/Cube
uniform sampler2D		iChannel2;			// input channel. XX = 2D/Cube
uniform sampler2D		iChannel3;			// input channel. XX = 2D/Cube
uniform vec4			iDate;				// (year, month, day, time in seconds)

out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
