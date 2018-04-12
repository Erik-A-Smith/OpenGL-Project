#version 330 core
out vec4 FragColor;

//////////////////////////////////
//			Inputs
//////////////////////////////////
in vec3 Normal;
in vec3 FragPos;
in vec2 uv;

//////////////////////////////////
//		Generic uniforms
//////////////////////////////////
uniform vec3 viewPos;

//////////////////////////////////
//	  Light inputs uniforms
//////////////////////////////////
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform float ambientStrength;

//////////////////////////////////
//	Texture coloring uniforms
//////////////////////////////////
uniform vec3 objectColor;
uniform bool enabledTextures;
uniform sampler2D aTexture;

void main()
{
	//////////////////////////////////
	//		Ambient Lighting
	//////////////////////////////////
	//float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	if (enabledTextures) {
		vec4 txture = texture(aTexture, uv);
		vec4 result = vec4(ambient + diffuse + specular,1);
		FragColor = vec4(result * txture);
	}
	else {
		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, 1.0);
	}
}