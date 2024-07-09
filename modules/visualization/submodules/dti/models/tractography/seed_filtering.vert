#version 400

in vec4 seed_position;

uniform samplerBuffer filter_and_position;
uniform samplerBuffer filter_not_position;

uniform int number_filter_and;
uniform int number_filter_not;

uniform vec3 phy_dimension;

uniform sampler3D tensorCoeffPart01;
uniform sampler3D tensorCoeffPart02;

uniform float roiPlaneAngle;
uniform bool isROIPlaneFilterActivated;

uniform float w_punct;
uniform float fiberStep;
uniform float stoppingCriterionFA;
uniform float stoppingCriterionAngleCos;
uniform int maxSteps;

uniform int dim_x;
uniform int dim_y;
uniform int dim_z;

out vec4 fiberSeedPos;
out int fiberFinalLength;
out int fiberEnable;

vec3 getMainDiffusionDirection(in vec3 position, inout vec3 eigenvalues, inout mat3 tensor)
{
    float m_pi = 3.141592653589f;
    vec4 tensor_coeff_part01_general = texture(tensorCoeffPart01, position).xyzw;
    vec3 tensor_coeff_part01 = tensor_coeff_part01_general.xyz;
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

    if(abs(term03) > 0 && term03 > 0.f)
        return sqrt((3.f * (term00 + term01 + term02)) / (2.f * term03));

    return 0.f;
}

vec3 my_norm(vec3 v)
{
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return v / len;
}

vec3 computeTensorline(in vec3 position, inout vec3 eigenvalues, in vec3 v_in)
{
    mat3 interpolated_tensor;
    vec3 v_1 = getMainDiffusionDirection(position, eigenvalues, interpolated_tensor);

    if(abs(dot(v_1, v_in)) > 0.00001 && dot(v_1, v_in) < 0.f)
        v_1 = -v_1;

    vec3 v_out = my_norm(interpolated_tensor * v_in);
    float cl = (eigenvalues[0] - eigenvalues[1]) / (eigenvalues[0] + eigenvalues[1] + eigenvalues[2]);

    return ( cl * v_1 + (1 - cl) * ((1 - w_punct) * v_in + w_punct * v_out) );
}

int flagSpatialRestriction(in vec3 position)
{
    ivec3 index;
    index[0] = int(floor(position.x * dim_x));
    index[1] = int(floor(position.y * dim_y));
    index[2] = int(floor(position.z * dim_z));

    if(index[0] == dim_x)
        index[0] = index[0] - 1;
    if(index[1] == dim_y)
        index[1] = index[1] - 1;
    if(index[2] == dim_z)
        index[2] = index[2] - 1;

    //Check if any of Voxels belong to plane where a Filter is defined
    float value = texelFetch(tensorCoeffPart01, index, 0).w;

    return int(value);
}

void main()
{
    vec3 eigenvalues;
    mat3 interpolated_tensor;
    float direction_inverter = seed_position.w;
    int flag = 0;

    vec3 normal_plane;

    if(direction_inverter < -2.5 || direction_inverter > 2.5) // sagittal ROI
        normal_plane = vec3(1.0, 0.0, 0.0);

    else if(direction_inverter < -1.5 || direction_inverter > 1.5) // coronal ROI
        normal_plane = vec3(0.0, 1.0, 0.0);

//    else if(direction_inverter < -0.5 || direction_inverter > 0.5) // axial ROI
    else
        normal_plane = vec3(0.0, 0.0, 1.0);

    if(direction_inverter < 0)
        direction_inverter = -1;
    else
        direction_inverter = 1;

#ifdef GL_ARB_shader_precision
    precise vec3 current_position = seed_position.xyz;
//    precise vec3 phy_current_position;
    precise vec3 current_direction = getMainDiffusionDirection(seed_position.xyz, eigenvalues, interpolated_tensor);
#else
    vec3 current_position = seed_position.xyz;
    vec3 current_direction = getMainDiffusionDirection(seed_position.xyz, eigenvalues, interpolated_tensor);
#endif

    float fa = fractionalAnisotropy(eigenvalues);
    current_direction = direction_inverter * current_direction;

    //Always enable all fibers
    fiberEnable = 0xffffffff;
    if(number_filter_and > 0) {
        fiberEnable = 0xffffffff << (number_filter_and + 1);
        fiberEnable = fiberEnable | 1;    //Bit 0 corresponding to NOT restriction
    }

    int j;
    bool plane_roi_tested = false;

    for(j = 1; j < maxSteps; j++)
    {
        if(isROIPlaneFilterActivated && !plane_roi_tested)
        {
            plane_roi_tested = true;
            if(dot(normal_plane, current_direction) > 0.f)
            {
                if(abs(dot(normal_plane, current_direction) - roiPlaneAngle) > 0.00001 && dot(normal_plane, current_direction) < roiPlaneAngle)
                    break;

            }else{

                if(abs(dot(normal_plane, -current_direction) - roiPlaneAngle) > 0.00001 && dot(normal_plane, -current_direction) < roiPlaneAngle)
                    break;
            }
        }

        if(abs(fa - stoppingCriterionFA) > 0.00001 && fa < stoppingCriterionFA)
            break;

        flag = flagSpatialRestriction(current_position);

        if(flag == 1)
        {
            fiberEnable = 0;  //Filtered out
            break;
        }
        if(flag > 1)
        {
            fiberEnable = fiberEnable | flag; //Set corresponding AND bit
        }

        current_position = current_position + my_norm(vec3(1.0/phy_dimension.x, 1.0/phy_dimension.y, 1.0/phy_dimension.z) * current_direction) * fiberStep;

#ifdef GL_ARB_shader_precision
        precise vec3 new_direction = my_norm(computeTensorline(current_position.xyz, eigenvalues, current_direction));
#else
        vec3 new_direction = my_norm(computeTensorline(current_position.xyz, eigenvalues, current_direction));
#endif

        if(isnan(new_direction.x + new_direction.y + new_direction.z))
            new_direction = current_direction;

        if(abs(dot(new_direction, current_direction) - stoppingCriterionAngleCos) > 0.00001 &&
                dot(new_direction, current_direction) < stoppingCriterionAngleCos)
            break;

        current_direction = new_direction;
        fa = fractionalAnisotropy(eigenvalues);
    }

    fiberFinalLength = j;
    fiberSeedPos = vec4(seed_position.xyz, direction_inverter);
}

