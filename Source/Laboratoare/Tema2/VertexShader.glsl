#version 330

#define PI (3.1415926535897932384626433832795)

#define DEFORMATION_NONE            (0)
#define DEFORMATION_LOSE_LIFE_POINT (1)
#define DEFORMATION_OVERDRIVE       (2)
#define DEFORMATION_LOSE_FUEL       (3)
#define DEFORMATION_INVULNERABLE    (4)
#define DEFORMATION_GET_FUEL        (5)

#define DEFORMATION_LOSE_LIFE_POINT_AMPLITUDE   (0.6f)
#define DEFORMATION_LOSE_LIFE_POINT_ZIGZAGS     (8)

#define DEFORMATION_OVERDRIVE_SCALE     (5.f)

#define DEFORMATION_LOSE_FUEL_AMPLITUDE (2.f)
#define DEFORMATION_LOSE_FUEL_FREQUENCY (20.f)

#define DEFORMATION_INVULNERABLE_AMPLITUDE  (1.f)
#define DEFORMATION_INVULNERABLE_PULSATIONS (5)

#define DEFORMATION_GET_FUEL_AMPLITUDE  (2.f)
#define DEFORMATION_GET_FUEL_FREQUENCY  (20.f)

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform bool is_hud_element;

uniform int deformation_type;
uniform float deformation_delta;
uniform float deformation_duration;

// Uniforms for light properties
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
	return mod289(((x * 34.0) + 1.0) * x);
}

vec4 taylorInvSqrt(vec4 r) {
	return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float perlin(vec3 P, vec3 rep) {
	vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
	vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
	Pi0 = mod289(Pi0);
	Pi1 = mod289(Pi1);
	vec3 Pf0 = fract(P); // Fractional part for interpolation
	vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
	vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
	vec4 iy = vec4(Pi0.yy, Pi1.yy);
	vec4 iz0 = Pi0.zzzz;
	vec4 iz1 = Pi1.zzzz;

	vec4 ixy = permute(permute(ix) + iy);
	vec4 ixy0 = permute(ixy + iz0);
	vec4 ixy1 = permute(ixy + iz1);

	vec4 gx0 = ixy0 * (1.0 / 7.0);
	vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
	gx0 = fract(gx0);
	vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
	vec4 sz0 = step(gz0, vec4(0.0));
	gx0 -= sz0 * (step(0.0, gx0) - 0.5);
	gy0 -= sz0 * (step(0.0, gy0) - 0.5);

	vec4 gx1 = ixy1 * (1.0 / 7.0);
	vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
	gx1 = fract(gx1);
	vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
	vec4 sz1 = step(gz1, vec4(0.0));
	gx1 -= sz1 * (step(0.0, gx1) - 0.5);
	gy1 -= sz1 * (step(0.0, gy1) - 0.5);

	vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
	vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
	vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
	vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
	vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
	vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
	vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
	vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

	vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
	g000 *= norm0.x;
	g010 *= norm0.y;
	g100 *= norm0.z;
	g110 *= norm0.w;
	vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
	g001 *= norm1.x;
	g011 *= norm1.y;
	g101 *= norm1.z;
	g111 *= norm1.w;

	float n000 = dot(g000, Pf0);
	float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
	float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
	float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
	float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
	float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
	float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
	float n111 = dot(g111, Pf1);

	vec3 fade_xyz = fade(Pf0);
	vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
	vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
	float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
	return 2.2 * n_xyz;
}

float noise(vec3 p) {
	float w = 100.0;
	float t = -.5;

	for (float f = 1.0 ; f <= 10.0 ; f++ ){
		float power = pow(2.0, f);
		t += abs(perlin(vec3(power * p), vec3(10.0, 10.0, 10.0)) / power);
	}

	return abs(t);
}

vec3 GetLoseLifePointDeformation(vec3 original_position) {
	// zig-zag on X
	float amplitude = DEFORMATION_LOSE_LIFE_POINT_AMPLITUDE;
	int zigzags = DEFORMATION_LOSE_LIFE_POINT_ZIGZAGS;

	float x = amplitude * asin(sin(1.f * zigzags * PI / deformation_duration * deformation_delta));
	float x_final = x * noise(original_position);

	return original_position + vec3(x_final, 0.0, 0.0);
}

vec3 GetOverdriveDeformation(vec3 original_position) {
	// scale on Z
	float scale = DEFORMATION_OVERDRIVE_SCALE;
	float a = 8 * scale / (deformation_duration * deformation_duration);
	float b = deformation_duration / 2;
	float c = 2 * scale;

	float z = min(scale, -a * (deformation_delta - b) * (deformation_delta - b) + c);
	float z_final = z * noise(original_position);

	return original_position * vec3(1.0, 1.0, 1.0 + z_final);
}

vec3 GetLoseFuelDeformation(vec3 original_position) {
	// inside scale bounce
	float amplitude = DEFORMATION_LOSE_FUEL_AMPLITUDE;
	float frequency = DEFORMATION_LOSE_FUEL_FREQUENCY;

	float control = max(0.0, 1.0 - deformation_delta / deformation_duration);
	float scale = -amplitude * sin(frequency * deformation_delta) * control;

	float scale_final = scale * noise(original_position);

	return original_position * vec3(1.0 + scale_final);
}

vec3 GetInvulnerableDeformation(vec3 original_position) {
	// pulse scale
	float amplitude = DEFORMATION_INVULNERABLE_AMPLITUDE;
	int pulsations = DEFORMATION_INVULNERABLE_PULSATIONS;
		
	float scale = amplitude * sin(1.f * pulsations * PI / deformation_duration * deformation_delta);
	float scale_final = scale * noise(original_position);

	return original_position * vec3(1.0 + scale_final);
}

vec3 GetGetFuelDeformation(vec3 original_position) {
	// outside scale bounce
	float amplitude = DEFORMATION_GET_FUEL_AMPLITUDE;
	float frequency = DEFORMATION_GET_FUEL_FREQUENCY;

	float control = max(0.0, 1.0 - deformation_delta / deformation_duration);
	float scale = amplitude * sin(frequency * deformation_delta) * control;

	float scale_final = scale * noise(original_position);

	return original_position * vec3(1.0 + scale_final); 
}

vec3 GetDeformation(vec3 original_position) {
		
	switch (deformation_type) {
	case DEFORMATION_LOSE_LIFE_POINT:
		return GetLoseLifePointDeformation(original_position);
	case DEFORMATION_OVERDRIVE:
		return GetOverdriveDeformation(original_position);
	case DEFORMATION_LOSE_FUEL:
		return GetLoseFuelDeformation(original_position);
	case DEFORMATION_INVULNERABLE:
		return GetInvulnerableDeformation(original_position);
	case DEFORMATION_GET_FUEL:
		return GetGetFuelDeformation(original_position);
	default:
		return original_position;
	}

}

void main() {
	// send color light output to fragment shader
	color = object_color;

	if (is_hud_element) {
			gl_Position = Model * vec4(v_position, 1.0);
	} else {
			gl_Position = Projection * View * Model * vec4(GetDeformation(v_position), 1.0);
	}
}
