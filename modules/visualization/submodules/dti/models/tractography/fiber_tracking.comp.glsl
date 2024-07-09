#version 440 core

layout (local_size_x = 8, local_size_y = 8) in;

layout (rgba32f, binding = 1) uniform imageBuffer seed_position_buffer;
layout (r32i, binding = 2) uniform iimageBuffer fiber_buffer_init_index;
layout (rgba32f, binding = 3) uniform imageBuffer fiber_buffer_result;
layout (rgba32f, binding = 4) uniform imageBuffer fiber_direction_buffer_result;

uniform sampler3D tensorCoeffPart01;
uniform sampler3D tensorCoeffPart02;
uniform vec3 phy_dimension;

uniform float w_punct;
uniform float fiberStep;
uniform float stoppingCriterionFA;
uniform float stoppingCriterionAngleCos;
uniform int numSeed;
uniform int maxSteps;

vec3 getMainDiffusionDirection(in vec3 position, inout vec3 eigenvalues, inout mat3 tensor)
{
    float m_pi = 3.141592653589f;
    vec3 tensor_coeff_part01 = texture(tensorCoeffPart01, position).xyz;
    vec3 tensor_coeff_part02 = texture(tensorCoeffPart02, position).xyz;

    tensor[0][0] = tensor_coeff_part01.x;
    tensor[1][1] = tensor_coeff_part01.y;
    tensor[2][2] = tensor_coeff_part01.z;

    tensor[1][0] = tensor_coeff_part02.x;
    tensor[2][0] = tensor_coeff_part02.y;
    tensor[2][1] = tensor_coeff_part02.z;

    tensor[0][1] = tensor_coeff_part02.x;
    tensor[0][2] = tensor_coeff_part02.y;
    tensor[1][2] = tensor_coeff_part02.z;

    float I_1 = tensor[0][0] + tensor[1][1] + tensor[2][2];
    float I_2 = (tensor[0][0] * tensor[1][1] + tensor[0][0] * tensor[2][2] + tensor[1][1] * tensor[2][2]) -
                (tensor[0][1] * tensor[0][1] + tensor[0][2] * tensor[0][2] + tensor[1][2] * tensor[1][2]);
    float I_3 = (tensor[0][0] * tensor[1][1] * tensor[2][2]) +
                2 * (tensor[0][1] * tensor[0][2] * tensor[1][2]) -
                ((tensor[2][2] * tensor[0][1] * tensor[0][1]) + (tensor[1][1] * tensor[0][2] * tensor[0][2]) + (tensor[0][0] * tensor[1][2] * tensor[1][2]));

    float I_1_third = I_1 / 3.f;
    float one_third = 1.f / 3.f;
    float v = I_1_third * I_1_third - (I_2 * one_third);
    float s = I_1_third * I_1_third * I_1_third - I_1 * (I_2 / 6.f) + (I_3 / 2.f);
    float one_div_v = 1.f / v;
    float phi = acos((s * one_div_v) * sqrt(one_div_v)) * one_third;

    vec3 my_eigenvalue;
    float _2_x_sqrt_v = 2.f * sqrt(v);
    my_eigenvalue[0] = I_1_third + (_2_x_sqrt_v * cos(phi));
    my_eigenvalue[1] = I_1_third - (_2_x_sqrt_v * cos((m_pi * one_third) + phi));
    my_eigenvalue[2] = I_1 - my_eigenvalue[0] - my_eigenvalue[1];

    mat3 my_eigenvector;
    for(int t = 0; t < 1; t++)
    {
        float A, B, C;
        A = tensor[0][0] - my_eigenvalue[t];
        B = tensor[1][1] - my_eigenvalue[t];
        C = tensor[2][2] - my_eigenvalue[t];

        my_eigenvector[t][0] = (tensor[0][1] * tensor[1][2] - B * tensor[0][2]) * (tensor[0][2] * tensor[1][2] - C * tensor[0][1]);
        my_eigenvector[t][1] = (tensor[0][2] * tensor[1][2] - C * tensor[0][1]) * (tensor[0][2] * tensor[0][1] - A * tensor[1][2]);
        my_eigenvector[t][2] = (tensor[0][1] * tensor[1][2] - B * tensor[0][2]) * (tensor[0][2] * tensor[0][1] - A * tensor[1][2]);

        float eigenvector_lenght = sqrt(my_eigenvector[t][0] * my_eigenvector[t][0] + my_eigenvector[t][1] * my_eigenvector[t][1] + my_eigenvector[t][2] * my_eigenvector[t][2]);
        my_eigenvector[t][0] = my_eigenvector[t][0] / eigenvector_lenght;
        my_eigenvector[t][1] = my_eigenvector[t][1] / eigenvector_lenght;
        my_eigenvector[t][2] = my_eigenvector[t][2] / eigenvector_lenght;
    }

    eigenvalues = my_eigenvalue;
    return my_eigenvector[0];
}

float fractionalAnisotropy(vec3 eigenvalues)
{
    float eigenvalue_01 = abs(eigenvalues[0]);
    float eigenvalue_02 = abs(eigenvalues[1]);
    float eigenvalue_03 = abs(eigenvalues[2]);
    float d = (eigenvalue_01 + eigenvalue_02 + eigenvalue_03) / 3.f;
    float term00 = pow(eigenvalue_01 - d, 2);
    float term01 = pow(eigenvalue_02 - d, 2);
    float term02 = pow(eigenvalue_03 - d, 2);
    float term03 = pow(eigenvalues[0], 2) + pow(eigenvalues[1], 2) + pow(eigenvalues[2], 2);

    if(abs(term03) > 0 && term03 > 0.f) //Ting (2/9/2021 - floating point comparisons)
        return sqrt((3.f * (term00 + term01 + term02)) / (2.f * term03));

    return 0.f;
}

vec3 my_norm(vec3 v)
{
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return v / len;
}

vec3 computerTensorline(in vec3 position, inout vec3 eigenvalues, in vec3 v_in)
{
    mat3 interpolated_tensor;
    vec3 v_1 = getMainDiffusionDirection(position, eigenvalues, interpolated_tensor);

    if(abs(dot(v_1, v_in)) > 0.00001 && dot(v_1, v_in) < 0.f)
        v_1 = -v_1;

    vec3 v_out = my_norm(interpolated_tensor * v_in);
    float cl = (eigenvalues[0] - eigenvalues[1]) / (eigenvalues[0] + eigenvalues[1] + eigenvalues[2]);

    return ( cl * v_1 + (1 - cl) * ((1 - w_punct) * v_in + w_punct * v_out) );
}

void main()
{
    int thread_global_index = int((gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x) * (gl_WorkGroupSize.x * gl_WorkGroupSize.y) + gl_LocalInvocationIndex);

    if(thread_global_index < numSeed)
    {
        vec4 seed_position = imageLoad(seed_position_buffer, thread_global_index);
        int fiber_init_index = imageLoad(fiber_buffer_init_index, thread_global_index).r;

        vec3 eigenvalues;
        mat3 interpolated_tensor;
        float direction_inverter = seed_position.w;
        precise vec3 current_position = seed_position.xyz;
        precise vec3 current_direction = getMainDiffusionDirection(current_position, eigenvalues, interpolated_tensor);

        float fa = fractionalAnisotropy(eigenvalues);
        current_direction = direction_inverter * current_direction;

        for(int j = 0; j < maxSteps; j++)
        {
            imageStore(fiber_buffer_result, fiber_init_index + j, vec4(current_position, 1.f));
            imageStore(fiber_direction_buffer_result, fiber_init_index + j, vec4(current_direction, 1.f));

               if(abs(fa - stoppingCriterionFA) > 0.00001 && fa < stoppingCriterionFA)
                break;

            current_position = current_position + my_norm(current_direction * vec3(1.0/phy_dimension.x, 1.0/phy_dimension.y, 1.0/phy_dimension.z)) * fiberStep;

            precise vec3 new_direction = my_norm(computerTensorline(current_position.xyz, eigenvalues, current_direction));

            if(isnan(new_direction.x + new_direction.y + new_direction.z))
                new_direction = current_direction;

              if(abs(dot(new_direction, current_direction) - stoppingCriterionAngleCos) > 0.00001 &&
                      dot(new_direction, current_direction) < stoppingCriterionAngleCos)
                 break;

            current_direction = new_direction;
            fa = fractionalAnisotropy(eigenvalues);
        }
    }

}
