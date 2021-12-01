#version 330 core

in vec3 normal;
in vec3 objectColor;

void main()
{
  vec3 lightDir = normalize(vec3(10.0, 10.0, 10.0));
  if (normal.x + normal.y + normal.z >= 3.0) {
    gl_FragColor = vec4(objectColor, 1.0);
  } else {
    float ambientStrength = 0.3;
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);

    // float diff = max(dot(norm, normalize(vec3(0.0, -20.0, 0.0))), 0.0);
    vec3 diffuse = diff * lightColor;

    // vec3 objectColor = vec3(0.3, 0.3, 0.3);

    vec3 result = (ambient + diffuse) * objectColor;
    // vec3 result = ambient * objectColor;
    gl_FragColor = vec4(result, 1.0);
  }
//  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}