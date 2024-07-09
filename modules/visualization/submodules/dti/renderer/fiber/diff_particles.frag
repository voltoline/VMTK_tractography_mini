#version 330

in vec4 color;

out vec4 fColor;

//const float PI = 3.141592653589793238462643383;
//const float interpolation_range = 20;

//vec4 new_3dvector_color_map_arc(vec3 vector)
//{
//    if(vector.z < 0)
//        vector = -vector;

//    vec3 origin_color = vec3(0.f, 0.f, 0.f);
//    vec3 top_color = vec3(1.f, 1.f, 1.f);

//    vec3 sector_1_color = vec3(1.f, 0.f, 0.f);
//    vec3 sector_2_color = vec3(1.f, 0.f, 1.f);
//    vec3 sector_3_color = vec3(0.f, 1.f, 0.f);
//    vec3 sector_4_color = vec3(1.f, 1.f, 0.f);
//    vec3 sector_5_color = vec3(0.f, 0.f, 1.f);
//    vec3 sector_6_color = vec3(0.f, 1.f, 1.f);

//    vec3 rgb = vec3(0.f), color_1 = vec3(0.f), color_2 = vec3(0.f);

//    float r = length(vector), t1 = 0, t1_aux, t2 = 0;
//    float azimuthal, azimuthal_deg;
//    float polar;

//    if(vector.x != 0)
//        azimuthal = atan(vector.y, vector.x);
//    else
//        azimuthal = 0;

//    azimuthal_deg = mod(degrees(azimuthal) + 360.f, 360.f);

//    if(r > 0)
//        polar = acos(vector.z / r);
//    else
//        polar = 0;

//    t1_aux = degrees(polar) / (90.f - interpolation_range);
//    t1 = degrees(polar) / (90.f - interpolation_range);

//    if(t1 > 1.f)
//    {
//        t1 = 1.f;
//    }

//    t1 = sin(pow(t1, 2.f) * (PI / 2.f));

//    if(azimuthal_deg >= 0 && azimuthal_deg < 60.f) //quadrant 1
//    {
//        t2 = azimuthal_deg / 60.f;
//        color_1 = sector_1_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_2_color * t1 + top_color * (1.f - t1);

//    } else if (azimuthal_deg >= 60.f && azimuthal_deg < 120.f){ //quadrant 2

//        t2 = (60.f - (120.f - azimuthal_deg)) / 60.f;
//        color_1 = sector_2_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_3_color * t1 + top_color * (1.f - t1);

//    } else if (azimuthal_deg >= 120.f && azimuthal_deg < 180.f){ //quadrant 3

//        t2 = (60.f - (180.f - azimuthal_deg)) / 60.f;
//        color_1 = sector_3_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_4_color * t1 + top_color * (1.f - t1);

//    } else if (azimuthal_deg >= 180.f && azimuthal_deg < 240.f){ //quadrant 4

//        t2 = (60.f - (240.f - azimuthal_deg)) / 60.f;
//        color_1 = sector_4_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_5_color * t1 + top_color * (1.f - t1);

//    } else if (azimuthal_deg >= 240.f && azimuthal_deg < 300.f){ //quadrant 4

//        t2 = (60.f - (300.f - azimuthal_deg)) / 60.f;
//        color_1 = sector_5_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_6_color * t1 + top_color * (1.f - t1);

//    } else if (azimuthal_deg >= 300.f && azimuthal_deg < 360.f){ //quadrant 4

//        t2 = (60.f - (360.f - azimuthal_deg)) / 60.f;
//        color_1 = sector_6_color * t1 + top_color * (1.f - t1);
//        color_2 = sector_1_color * t1 + top_color * (1.f - t1);
//    }

//    rgb = color_2 * t2 + color_1 * (1 - t2);
//    rgb = origin_color * (1.f - r) + rgb * r;

//    return vec4(rgb, t1_aux);
//}

//float polar(vec3 vector)
//{
//    vec3 vec3d;
//    vec3 sat_min_axis = vec3(0.f, 0.f, 1.f);
//    vec3d = normalize(vector);

//    return degrees(acos(dot(vec3d, sat_min_axis)));
//}

//float azimuthal(vec3 vector)
//{
//    vec3 vec3d;
//    vec3 sat_min_axis = vec3(1.f, 0.f, 0.f);
//    vec3d = normalize(vector);

//    float c = degrees(acos(dot(vec3(vec3d.xy, 0.f), sat_min_axis)));
//    if(vector.y < 0)
//        return 360 - c;
//    else
//        return c;
//}

//vec4 new_vector2rgb(vec3 vector)
//{
//    vec4 rgb = new_3dvector_color_map_arc(vector);

//    vec4 mirror_rgb;
//    float t;

//    if(rgb.a > 1.f)
//    {
//        if(vector.z >= 0)
//        {
//            if(polar(vector) >= (90.f - interpolation_range))
//            {
//                vec4 mirror_rgb = new_3dvector_color_map_arc(vec3(-vector.xy, vector.z));

//                float diff_ang = interpolation_range - (90.f - polar(vector));
//                t = diff_ang / (interpolation_range * 2);

//                rgb = vec4((rgb * (1.f - t) + mirror_rgb * t).rgb, 1.f);
//            }

//        } else {

//            if(polar(vector) <= (90.f + interpolation_range))
//            {
//                vec4 mirror_rgb = new_3dvector_color_map_arc(vec3(vector.xy, -vector.z));

//                float diff_ang = polar(vector) - 90.f + interpolation_range;
//                t = diff_ang / (interpolation_range * 2);

//                rgb = vec4((mirror_rgb * (1.f - t) + rgb * t).rgb, 1.f);
//            }
//        }
//    }

//    return vec4(rgb.rgb, 1.f);
//}


void main()
{
    //vec4 frag_main_diff_dir_vec = vec4(texelFetch(main_diff_direction_tex, geom_tex_position).xyz, 1.f);

    //frag_main_diff_dir_vec.a = texelFetch(main_fa_tex, geom_tex_position).r;

//    fColor = new_vector2rgb(frag_main_diff_dir_vec);
    //fColor.a = frag_main_diff_dir_vec.a;

//    fColor = vec4(1.0, 0.0, 0.0, 1.0);

    fColor = color;
}

