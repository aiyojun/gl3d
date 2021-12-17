# GLSL

## vertex shader

1. matrix transform(position)
2. normal transform
3. shadow transform(light source view)

## fragment shader

1. entity color
2. parallel light source
3. point light source
4. diffuse(reflection)
5. ambient
6. specular
7. diffuse texture
8. normal texture
9. specular texture
10. specular reflection(like mirror)
11. mix(blend) transparent(alpha component)
12. shadow(depth comparison from light source view)
13. cube texture
14. post-production(image processing, like floodlight/flood)

## phong light model

ambient + diffuse + specular

## basic types

- function return type: void;
- basic types: bool, int, float;
- bool vectors: bvec2, bvec3, bvec4;
- int vectors: ivec2, ivec3, ivec4;
- float vectors: vec2, vec3, vec4;
- mat2/mat2x2, mat3/mat3x3, mat4/mat4x4;
- sampler1D, sampler2D, sampler3D
- samplerCube
- 1/2 dimensions depth texture handles: sampler1DShadow, sampler2DShadow

## structure

- The nested 'struct' is invalid;
- Assign: = ;
- Comparison: ==/!= ;

```glsl
struct outer {
    int id;
    vec3 points[3];
};
```

### watch out

- initializing: use vec3(0) to declare an empty vector, mat(1.0) means [1 0 0, 0 1 0, 0 0 1]
- assign: vec4(v3.xyz, 1.0) or vec4(v3, 1.0)
- calculating: dot(vec), cross(vec), mul between matrix and vector.

## qualifier

- const 
- attribute
- uniform
- varying
- centorid varying
- invariant
- in
- out
- inout

## buildin variables

in vertex shader:

| Vairables | Type | Summary |
| ---- | ---- | ---- |
| gl_Color | vec4 | - |
| gl_SecondaryColor | vec4 | - |
| gl_Normal | vec3 | - |
| gl_Vertex | vec4 | - |
| gl_MultiTexCoordn | vec4 | - |
| gl_FogCoord | float | - |
| gl_Position | vec4 | - |
| gl_ClipVertex | vec4 | - |
| gl_PointSize | float | - |
| gl_FrontColor | vec4 | - |
| gl_BackColor | vec4 | - |
| gl_FrontSecondaryColor | vec4 | - |
| gl_BackSecondaryColor | vec4 | - |
| gl_TexCoord[] | vec4 | - |
| gl_FogFragCoord | vec4 | - |

in fragment shader:

| Vairables | Type | Summary |
| ---- | ---- | ---- |
| gl_Color | vec4 | - |
| gl_SecondaryColor | vec4 | - |
| gl_TexCoord[] | vec4 | - |
| gl_FogFragCoord | vec4 | - |
| gl_FragCoord | vec4 | - |
| gl_FrontFacing | float | - |
| gl_PointCoord | vec2 | - |
| gl_FragData[] | vec4 | - |
| gl_FragColor | vec4 | - |
| gl_FragDepth | float | - |

## A special KEYWORD in fragment shader: discard;

## buildin functions

| Function | Summary |
| ---- | ---- |
| texture | - |
| cos sin tan | - |
| acos asin atan | - |
| radians degrees | - |
| pow | - |
| sqrt inversesqrt | - |
| exp exp2 | - |
| log | - |
| abs sign floor ceil mod fract min max clamp mix | - |
| step | - |
| smoothstep | - |
| length distance dot cross normalize | - |
| faceforward | - |
| reflect | - |
| matrixCompMult | M * M |
| lessThan lessThanEqual | vector |
| greaterThan greaterThanEqual | - |
| equal notEqual | - |
| any all not | - |
| texture2D texture2DProj texture2DLod texture2DProjLod | - |
| textureCube textureCubeLod | - |

