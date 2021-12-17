#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);

    normal = vec3(transform * vec4(aNormal, 1.0)) - vec3(transform * (vec4(0.0, 0.0, 0.0, 1.0)));
    FragPos = aPos;
}

// --
#version 330 core

in vec3 normal;
in vec3 FragPos;

struct Ray { float ia, id, is; vec3 dir, pos, color; };
// struct Material { vec3 Ka, Kd, Ks, color; sampler2D map_Ka, map_Kd, map_Ks; float shininess; };

// uniform int rendering; // [Material] 100: color; 200: map diffuse;
uniform Ray ray;
uniform vec3 viewPos;
// uniform Material material;

void main()
{
    vec3 vd_norm  = normalize(normal);
    vec3 vd_view  = normalize(viewPos - FragPos);
    vec3 vd_refl  = reflect(normalize(ray.dir), vd_norm);
    float diff    = max(dot(normalize(-ray.dir), vd_norm), 0.0);
    // vec3 vd_refl  = reflect(normalize(vec3(0.0, 0.0, -1.0)), vd_norm);
    // float diff    = max(dot(normalize(-vec3(0.0, 0.0, -1.0)), vd_norm), 0.0);
    float spec    = pow(max(dot(vd_view, vd_refl), 0.0), 2);//material.shininess);
    vec3 ambient = vec3(0.0), diffuse = vec3(0.0), specular = vec3(0.0);


    // ambient  = 0.5 * vec3(1.0, 1.0, 1.0) * vec3(1.0, 1.0, 1.0);//material.Ka;
    // diffuse  = 0.5 * vec3(1.0, 1.0, 1.0) * diff * vec3(0.8, 0.8, 0.8);//material.Kd;
    // specular = 0.5 * vec3(1.0, 1.0, 1.0) * spec * vec3(0.0);//material.Ks;
    ambient  = ray.ia * ray.color * vec3(1.0, 1.0, 1.0);//material.Ka;
    diffuse  = ray.id * ray.color * diff * vec3(0.8, 0.8, 0.8);//material.Kd;
    specular = ray.is * ray.color * spec * vec3(0.0);//material.Ks;

    gl_FragColor  = vec4(ambient + diffuse + specular, 1.0);
    // gl_FragColor  = vec4(1.0, 1.0, 1.0, 1.0);
}