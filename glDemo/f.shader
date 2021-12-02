#version 330 core

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    vec3 ambient;
    vec3 Kd;
    vec3 Ks;
    sampler2D diffuse;
//    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
//uniform vec3 lightPos;

uniform DirLight light;
uniform Material material;

void main()
{
    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, light.direction), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-light.direction, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse;
    if (TexCoords.s < 0 || TexCoords.t < 0) {
        diffuse = light.diffuse * diff * material.Kd;
    } else {
        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    }
    //ec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    //gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor = vec4(ambient + diffuse, 1.0);
}