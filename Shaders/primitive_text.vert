#version 330
//uniform bool StateInstancing;
uniform mat4 MVP;
//uniform mat4 mvpInstanced[23];

in int Character;
out int vCharacter;
out int vPosition;
out mat4 vMVP;
//flat out int InstanceID;

out Data
{
  vec3 pos;
}Vdataout;

void main()
{
    vCharacter = Character;
    vPosition = gl_VertexID;
//    if(StateInstancing==true){
//        vMVP = mvpInstanced[gl_InstanceID];
//        gl_Position = mvpInstanced[gl_InstanceID] * vec4(0, 0, 0, 1);
//        InstanceID = gl_InstanceID;
//    }
//    else{
        gl_Position = MVP * vec4(0, 0, 0, 1);
//        gl_Position =  vec4(0, 0, 0, 1);
//    }
    Vdataout.pos = (vec3(gl_Position.x/gl_Position.w, gl_Position.y/gl_Position.w, gl_Position.z/gl_Position.w)+1.0)/2.0;
}
