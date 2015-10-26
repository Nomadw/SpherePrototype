#version 110

uniform sampler2D tex0;

uniform float alpha;

uniform float red;
uniform float green;
uniform float blue;

uniform float shine;

varying vec3 v;
varying vec3 N;

void main()
{	
	vec4	ambient = vec4(red, green, blue, alpha);
	vec4	diffuse = vec4(0.9, 0.9, 0.9, alpha);
	vec4	specular = vec4(1.0, 1.0, 1.0, alpha);
	float shinyness = shine;
	
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	vec3 E = normalize(-v); 
	vec3 R = normalize(-reflect(L,N));  

	// ambient term 
	vec4 Iamb = ambient;    

	// diffuse term
	vec4 Idiff = texture2D( tex0, gl_TexCoord[0].st) * diffuse; 
	Idiff *= max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     

	// specular term
	vec4 Ispec = specular; 
	Ispec *= pow(max(dot(R,E),0.0), shinyness);
	Ispec = clamp(Ispec, 0.0, 1.0); 

	// final color 
	gl_FragColor = Iamb + Idiff + Ispec;	
}
