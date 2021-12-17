#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);

    normal = vec3(transform * vec4(aNormal, 1.0)) - vec3(transform * (vec4(0.0, 0.0, 0.0, 1.0)));
    FragPos = aPos;
    TexCoords = aTexCoords;
}

// --
#version 330 core

in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Ray { float ia, id, is; vec3 dir, pos, color; };
struct Material { vec3 Ka, Kd, Ks, color; sampler2D map_Ka, map_Kd, map_Ks; float shininess; };

uniform int rendering; // [Material] 100: color; 200: map diffuse;
uniform Ray ray;
uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec3 vd_norm  = normalize(normal);
    vec3 vd_view  = normalize(viewPos - FragPos);
    vec3 vd_refl  = reflect(normalize(ray.dir), vd_norm);
    float diff    = max(dot(normalize(-ray.dir), vd_norm), 0.0);
    float spec    = pow(max(dot(vd_view, vd_refl), 0.0), material.shininess);
    vec3 ambient = vec3(0.0), diffuse = vec3(0.0), specular = vec3(0.0);
    if (rendering == 200) {
        diffuse  = ray.id * ray.color * diff * vec3(texture(material.map_Kd, TexCoords));
    } else if (rendering == 201) {
        ambient  = ray.ia * ray.color * vec3(texture(material.map_Ka, TexCoords));
        diffuse  = ray.id * ray.color * diff * vec3(texture(material.map_Kd, TexCoords));
    } else if (rendering == 210) {
        diffuse  = ray.id * ray.color * diff * vec3(texture(material.map_Kd, TexCoords));
        specular = ray.is * ray.color * spec * vec3(texture(material.map_Ks, TexCoords));
    } else if (rendering == 211) {
        ambient  = ray.ia * ray.color * vec3(texture(material.map_Ka, TexCoords));
        diffuse  = ray.id * ray.color * diff * vec3(texture(material.map_Kd, TexCoords));
        specular = ray.is * ray.color * spec * vec3(texture(material.map_Ks, TexCoords));
    } else { // default rendering == 100
        ambient  = ray.ia * ray.color * material.Ka;
        diffuse  = ray.id * ray.color * diff * material.Kd;
        specular = ray.is * ray.color * spec * material.Ks;
    }
    gl_FragColor  = vec4(ambient + diffuse + specular, 1.0);
}