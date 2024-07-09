#version 330
in vec3 verCoord;
in vec2 texCoord;
in vec3 normal;
out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vColor;
uniform mat4 ProjectionMatrix;
uniform samplerBuffer modelViewMatrixTexture;
uniform samplerBuffer colorTexture;

void main(void) {
vec4 position;
vColor = vec4(0.0,1.0,1.0,0.5);

// As matrices en formato de colunas" //
vec4 col1 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4);
vec4 col2 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 1);
vec4 col3 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 2);
vec4 col4 = texelFetch(modelViewMatrixTexture, gl_InstanceID * 4 + 3);

// montamos a matrix e obtemos a posição dos vértices da geometria.
mat4 model_matrix = mat4(col1, col2, col3, col4);
vec4 pos = vec4(verCoord ,1.0);
position = ProjectionMatrix * ( model_matrix * pos );

// Cor independente para cada geometria.
vColor = texelFetch(colorTexture, gl_InstanceID);

// a cordenada de testura e as normis estõ normalizadas.
vTexCoord = texCoord;
vNormal = normal;
gl_Position = position;
};
