#ifndef CDM_HPP
#define CDM_HPP
#include "tipl/numerical/basic_op.hpp"
#include "tipl/numerical/dif.hpp"
#include "tipl/filter/gaussian.hpp"
#include "tipl/filter/filter_model.hpp"
#include "tipl/utility/multi_thread.hpp"
#include "tipl/numerical/resampling.hpp"
#include "tipl/numerical/statistics.hpp"
#include "tipl/numerical/window.hpp"
#include <iostream>
#include <limits>
#include <vector>

namespace tipl
{
namespace reg
{

template<class pixel_type,size_t dimension>
void cdm_average_img(const std::vector<image<pixel_type,dimension> >& Ji, image<pixel_type,dimension>& J0)
{
    J0 = Ji[0];
    for(unsigned int index = 1;index < Ji.size();++index)
        add(J0.begin(),J0.end(),Ji[index].begin());
    divide_constant(J0.begin(),J0.end(),float(Ji.size()));
}

template<class pixel_type,size_t dimension>
float cdm_img_dif(const image<pixel_type,dimension>& I0,
                    const image<pixel_type,dimension>& I1)
{
    float value = 0;
    for (int index = 0; index < I0.size(); ++index)
    {
        pixel_type tmp = I0[index]-I1[index];
        value += tmp*tmp;
    }
    return value;
}

template<class pixel_type,size_t dimension>
float cdm_img_dif(const std::vector<image<pixel_type,dimension> >& Ji,const image<pixel_type,dimension>& J0)
{
    float next_dif = 0;
    for(unsigned int index = 0;index < Ji.size();++index)
        next_dif += cdm_img_dif(J0,Ji[index]);
    return next_dif;
}


template<class pixel_type,size_t dimension>
float cdm_contrast(const image<pixel_type,dimension>& J0,
                     const image<pixel_type,dimension>& Ji)
{
    float value1 = 0,value2 = 0;
    for (int index = 0; index < J0.size(); ++index)
    {
        float tmp = Ji[index];
        value1 += tmp*J0[index];
        value2 += tmp*tmp;
    }
    if(value2 == 0.0f)
        return 1.0f;
    return value1/value2;
}

template<class pixel_type,size_t dimension>
void cdm_update_contrast(const std::vector<image<pixel_type,dimension> >& Ji,
                          std::vector<float>& contrast)
{
    image<pixel_type,dimension> J0;
    cdm_average_img(Ji,J0);
    contrast.resize(Ji.size());
    for (unsigned int index = 0; index < Ji.size(); ++index)
        contrast[index] = cdm_contrast(J0,Ji[index]);
    float sum_contrast = std::accumulate(contrast.begin(),contrast.end(),0.0f);
    sum_contrast /= Ji.size();
    divide_constant(contrast.begin(),contrast.end(),sum_contrast);
}


// trim the image size to uniform
template<class pixel_type,unsigned int dimension,class crop_type>
void cdm_trim_images(std::vector<image<pixel_type,dimension> >& I,
                      crop_type& crop_from,crop_type& crop_to)
{
    crop_from.resize(I.size());
    crop_to.resize(I.size());
    vector<dimension,int> min_from,max_to;
    for(int index = 0;index < I.size();++index)
    {
        crop(I[index],crop_from[index],crop_to[index]);
        if(index == 0)
        {
            min_from = crop_from[0];
            max_to = crop_to[0];
        }
        else
            for(int dim = 0;dim < dimension;++dim)
            {
                min_from[dim] = std::min(crop_from[index][dim],min_from[dim]);
                max_to[dim] = std::max(crop_to[index][dim],max_to[dim]);
            }
    }
    max_to -= min_from;
    int safe_margin = std::accumulate(max_to.begin(),max_to.end(),0.0f)/float(dimension)/2.0f;
    max_to += safe_margin;
    geometry<dimension> geo(max_to.begin());
    // align with respect to the min_from
    for(int index = 0;index < I.size();++index)
    {
        image<float,dimension> new_I(geo);
        vector<dimension,int> pos(crop_from[index]);
        pos -= min_from;
        pos += safe_margin/2;
        draw(I[index],new_I,pos);
        new_I.swap(I[index]);
        crop_from[index] -= pos;
        crop_to[index] = crop_from[index];
        crop_to[index] += geo.begin();
    }

}

template<class value_type,size_t dimension>
class poisson_equation_solver;

template<class value_type>
class poisson_equation_solver<value_type,2>
{
    typedef typename filter::pixel_manip<value_type>::type manip_type;
public:
    template<class image_type>
    void operator()(image_type& src)
    {
        image_type dest(src.geometry());
        int w = src.width();
        int shift[4];
        shift[0] = 1;
        shift[1] = -1;
        shift[2] = w;
        shift[3] = -w;
        for(int i = 0;i < 4;++i)
        if (shift[i] >= 0)
        {
            auto iter1 = dest.begin() + shift[i];
            auto iter2 = src.begin();
            auto end = dest.end();
            for (;iter1 < end;++iter1,++iter2)
                *iter1 += *iter2;
        }
        else
        {
            auto iter1 = dest.begin();
            auto iter2 = src.begin() + (-shift[i]);
            auto end = src.end();
            for (;iter2 < end;++iter1,++iter2)
                *iter1 += *iter2;
        }
        dest.swap(src);
    }
};
template<class value_type>
class poisson_equation_solver<value_type,3>
{
    typedef typename filter::pixel_manip<value_type>::type manip_type;
public:
    template<class image_type>
    void operator()(image_type& src)
    {
        image_type dest(src.geometry());
        int w = src.width();
        int wh = src.width()*src.height();
        int shift[6];
        shift[0] = 1;
        shift[1] = -1;
        shift[2] = w;
        shift[3] = -w;
        shift[4] = wh;
        shift[5] = -wh;
        for(int i = 0;i < 6;++i)
        {
            if (shift[i] >= 0)
            {
                int s = shift[i];
                par_for(dest.size()-s,[&dest,&src,s](int index){
                    dest[index+s] += src[index];
                });
            }
            else
            {
                int s = -shift[i];
                par_for(dest.size()-s,[&dest,&src,s](int index){
                    dest[index] += src[index+s];
                });
            }
        }
        dest.swap(src);
    }
};

template<class pixel_type,class vtor_type,unsigned int dimension,class terminate_type>
void cdm_group(const std::vector<image<pixel_type,dimension> >& I,// original images
          std::vector<image<vtor_type,dimension> >& d,// displacement field
          float theta,float reg,terminate_type& terminated)
{
    if (I.empty())
        return;
    size_t n = I.size();
    geometry<dimension> geo = I[0].geometry();

    d.resize(n);
    // preallocated memory so that there will be no memory re-allocation in upsampling
    for (size_t index = 0;index < n;++index)
        d[index].resize(geo);

    // multi resolution
    if (*std::min_element(geo.begin(),geo.end()) > 16)
    {
        //downsampling
        std::vector<image<pixel_type,dimension> > rI(n);
        for (size_t index = 0;index < n;++index)
            downsample_with_padding(I[index],rI[index]);
        cdm_group(rI,d,theta/2,reg,terminated);
        // upsampling deformation
        for (size_t index = 0;index < n;++index)
        {
            upsample_with_padding(d[index],d[index],geo);
            d[index] * 2.0f;
        }
    }
    std::cout << "dimension:" << geo[0] << "x" << geo[1] << "x" << geo[2] << std::endl;
    image<pixel_type,dimension> J0(geo);// the potential template
    std::vector<image<pixel_type,dimension> > Ji(n);// transformed I
    std::vector<image<vtor_type,dimension> > new_d(n);// new displacements
    std::vector<float> contrast(n);
    float current_dif = std::numeric_limits<float>::max();
    for (float dis = 0.5;dis > theta;)
    {
        // calculate Ji
        for (unsigned int index = 0;index < n;++index)
            compose_displacement(I[index],d[index],Ji[index]);


        // calculate contrast
        cdm_update_contrast(Ji,contrast);

        // apply contrast
        multiply(Ji.begin(),Ji.end(),contrast.begin());

        cdm_average_img(Ji,J0);

        float next_dif = cdm_img_dif(Ji,J0);
        if(next_dif >= current_dif)
        {
            dis *= 0.5f;
            std::cout << "detail=(" << dis << ")" << std::flush;
            // roll back
            d.swap(new_d);
            current_dif = std::numeric_limits<float>::max();
            continue;
        }
        std::cout << next_dif;
        current_dif = next_dif;
        // initialize J0
        float max_d = 0;
        for (unsigned int index = 0;index < n && !terminated;++index)
        {
            std::cout << "." << std::flush;

            // gradient of Ji
            gradient_sobel(J0,new_d[index]);

            // dJi*sign(Ji-J0)
            for(unsigned int i = 0;i < Ji[index].size();++i)
                if(Ji[index][i] < J0[i])
                    new_d[index][i] = -new_d[index][i];

            {
                image<pixel_type,dimension> temp;
                jacobian_determinant_dis(d[index],temp);
                compose_displacement(temp,d[index],Ji[index]);
            }

            for(unsigned int i = 0;i < Ji[index].size();++i)
                new_d[index][i] *= Ji[index][i];

            //io::nifti header;
            //header << Ji[index];
            //header.save_to_file("c:/1.nii");

            // solving the poisson equation using Jacobi method
            {
                image<vtor_type,dimension> solve_d(new_d[index].geometry());
                for(int iter = 0;iter < 20;++iter)
                {
                    poisson_equation_solver<vtor_type,dimension>()(solve_d);
                    add_mt(solve_d,new_d[index]);
                    divide_constant_mt(solve_d,dimension*2);
                }
                new_d[index].swap(solve_d);
                minus_constant(new_d[index].begin(),new_d[index].end(),new_d[index][0]);
            }

            for (unsigned int i = 0; i < geo.size(); ++i)
                if (new_d[index][i]*new_d[index][i] > max_d)
                    max_d = std::sqrt(new_d[index][i]*new_d[index][i]);
        }
        // calculate the lambda
        float lambda = -dis/max_d;
        for (unsigned int index = 0;index < n && !terminated;++index)
        {
            new_d[index] *= lambda;
            add(new_d[index].begin(),new_d[index].end(),d[index].begin());
        }
        d.swap(new_d);
    }
    std::cout << std::endl;
}

// calculate dJ(cJ-I)
template<typename image_type,typename dis_type>
float cdm_get_gradient(const image_type& Js,const image_type& It,dis_type& new_d)
{
    float accumulated_r2 = 0.0f;
    unsigned int r_num = 0;
    const unsigned int window_size = 3;
    gradient_sobel(Js,new_d);
    Js.for_each_mt([&](typename image_type::value_type,pixel_index<image_type::dimension>& index){
        if(It[index.index()] == 0.0 ||
           Js[index.index()] == 0.0 ||
           It.geometry().is_edge(index))
        {
            new_d[index.index()] = typename dis_type::value_type();
            return;
        }
        std::vector<typename image_type::value_type> Itv,Jv;
        get_window(index,It,window_size,Itv);
        get_window(index,Js,window_size,Jv);
        float a,b,r2;
        linear_regression(Jv.begin(),Jv.end(),Itv.begin(),a,b,r2);
        if(a <= 0.0f)
            new_d[index.index()] = typename dis_type::value_type();
        else
        {
            new_d[index.index()] *= r2*(Js[index.index()]*a+b-It[index.index()]);
            accumulated_r2 += r2;
            ++r_num;
        }
    });
    return accumulated_r2/float(r_num);
}

// calculate dJ(cJ-I)
template<typename image_type,typename dis_type>
float cdm_get_gradient_abs_dif(const image_type& Js,const image_type& It,dis_type& new_d)
{
    float accumulated_r2 = 0.0f;
    unsigned int r_num = 0;
    gradient_sobel(Js,new_d);
    Js.for_each_mt([&](typename image_type::value_type,pixel_index<image_type::dimension>& index){
        if(It[index.index()] == 0.0 ||
           Js[index.index()] == 0.0 ||
           It.geometry().is_edge(index))
        {
            new_d[index.index()] = typename dis_type::value_type();
            return;
        }
        auto dif = Js[index.index()]-It[index.index()];
        new_d[index.index()] *= dif;
        accumulated_r2 += dif*dif;
        ++r_num;
    });
    return accumulated_r2/float(r_num);
}


/*
template<typename dis_type,typename terminated_type>
void cdm_solve_poisson(dis_type& new_d,terminated_type& terminated)
{
    float inv_d2 = 0.5f/dis_type::dimension;
    const unsigned int window_size = 3;
    dis_type solve_d(new_d);
    multiply_constant_mt(solve_d,-inv_d2);

    int shift[dis_type::dimension]={0};
    shift[0] = 1;
    for(int i = 1;i < dis_type::dimension;++i)
        shift[i] = shift[i-1]*new_d.geometry()[i-1];

    for(int iter = 0;iter < window_size*2 && !terminated;++iter)
    {
        dis_type new_solve_d(new_d.geometry());
        par_for(solve_d.size(),[&](int pos)
        {
            for(int d = 0;d < dis_type::dimension;++d)
            {
                int p1 = pos-shift[d];
                int p2 = pos+shift[d];
                if(p1 >= 0)
                   new_solve_d[pos] += solve_d[p1];
                if(p2 < solve_d.size())
                   new_solve_d[pos] += solve_d[p2];
            }
            new_solve_d[pos] -= new_d[pos];
            new_solve_d[pos] *= inv_d2;
        });
        solve_d.swap(new_solve_d);
    }
    minus_constant_mt(solve_d,solve_d[0]);
    new_d.swap(solve_d);
}
*/

template<typename dis_type,typename terminated_type>
void cdm_solve_poisson(tipl::image<dis_type,3>& new_d,terminated_type& terminated)
{
    float inv_d2 = 0.5f/3.0f;
    tipl::image<dis_type,3> solve_d(new_d);
    multiply_constant_mt(solve_d,-inv_d2);

    int w = new_d.width();
    int wh = new_d.plane_size();
    for(int iter = 0;iter < 6 && !terminated;++iter)
    {
        tipl::image<dis_type,3> new_solve_d(new_d.geometry());
        tipl::par_for(solve_d.size(),[&](int pos)
        {
            auto v = new_solve_d[pos];
            {
                int p1 = pos-1;
                int p2 = pos+1;
                if(p1 >= 0)
                   v += solve_d[p1];
                if(p2 < solve_d.size())
                   v += solve_d[p2];
            }
            {
                int p1 = pos-w;
                int p2 = pos+w;
                if(p1 >= 0)
                   v += solve_d[p1];
                if(p2 < solve_d.size())
                   v += solve_d[p2];
            }
            {
                int p1 = pos-wh;
                int p2 = pos+wh;
                if(p1 >= 0)
                   v += solve_d[p1];
                if(p2 < solve_d.size())
                   v += solve_d[p2];
            }
            v -= new_d[pos];
            v *= inv_d2;
            new_solve_d[pos] = v;
        });
        solve_d.swap(new_solve_d);
    }
    minus_constant_mt(solve_d,solve_d[0]);
    new_d.swap(solve_d);
}

template<typename dist_type,typename value_type>
void cdm_accumulate_dis(dist_type& d,dist_type& new_d,value_type& theta,float cdm_smoothness)
{
    value_type cdm_smoothness2 = value_type(1.0)-cdm_smoothness;
    if(theta == 0.0)
    {
        par_for(new_d.size(),[&](int i)
        {
           value_type l = new_d[i].length();
           if(l > theta)
               theta = l;
        });
    }
    multiply_constant_mt(new_d,0.5f/theta);
    add(new_d,d);

    dist_type new_ds(new_d);
    filter::gaussian2(new_ds);
    par_for(new_d.size(),[&](int i){
       new_ds[i] *= cdm_smoothness;
       new_d[i] *= cdm_smoothness2;
       new_d[i] += new_ds[i];
    });
    new_d.swap(d);
}

template<typename r_type>
bool cdm_improved(r_type& r,r_type& iter)
{
    if(r.size() > 5)
    {
        float a,b,r2;
        linear_regression(iter.begin(),iter.end(),r.begin(),a,b,r2);
        if(a < 0.0f)
            return false;
        if(r.size() > 7)
        {
            r.pop_front();
            iter.pop_front();
        }
    }
    return true;
}

/*
 * cdm_smoothness 0.1: more smooth 0.9: less smooth
 */
template<typename image_type,typename dist_type,typename terminate_type>
float cdm(const image_type& It,
            const image_type& Is,
            dist_type& d,// displacement field
            terminate_type& terminated,
            float resolution = 2.0f,
            float cdm_smoothness = 0.3f,
            unsigned int iterations = 60)
{
    if(It.geometry() != Is.geometry())
        throw "Inconsistent image dimension";
    auto geo = It.geometry();
    d.resize(It.geometry());

    // multi resolution
    if (*std::min_element(geo.begin(),geo.end()) > 16)
    {
        //downsampling
        image_type rIs,rIt;
        downsample_with_padding(It,rIt);
        downsample_with_padding(Is,rIs);
        float r = cdm(rIt,rIs,d,terminated,resolution/2.0,cdm_smoothness,iterations*2);
        upsample_with_padding(d,d,geo);
        d *= 2.0f;
        if(resolution > 1.0f)
            return r;
    }
    image_type Js;// transformed I
    dist_type new_d(d.geometry());// new displacements
    float theta = 0.0;

    std::deque<float> r,iter;
    for (unsigned int index = 0;index < iterations && !terminated;++index)
    {
        compose_displacement(Is,d,Js);
        // dJ(cJ-I)
        r.push_back(cdm_get_gradient(Js,It,new_d));
        iter.push_back(index);
        if(!cdm_improved(r,iter))
            break;
        // solving the poisson equation using Jacobi method
        cdm_solve_poisson(new_d,terminated);
        cdm_accumulate_dis(d,new_d,theta,cdm_smoothness);
    }
    return r.front();
}

template<typename image_type>
void cdm_pre(image_type& I)
{
    if(I.empty())
        return;
    float mean = float(tipl::mean(I));
    if(mean != 0.0f)
        I *= 1.0f/mean;
}
template<typename image_type>
void cdm_pre(image_type& It,image_type& It2,
             image_type& Is,image_type& Is2)
{
    std::thread t1([&](){cdm_pre(It);});
    std::thread t2([&](){cdm_pre(It2);});
    std::thread t3([&](){cdm_pre(Is);});
    std::thread t4([&](){cdm_pre(Is2);});
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
template<typename image_type,typename dist_type,typename terminate_type>
float cdm2(const image_type& It,const image_type& It2,
            const image_type& Is,const image_type& Is2,
            dist_type& d,// displacement field
            terminate_type& terminated,
            float resolution = 2.0f,
            float cdm_smoothness = 0.3f,
            unsigned int iterations = 60)
{
    if(It.geometry() != It2.geometry() ||
       It.geometry() != Is.geometry() ||
       It.geometry() != Is2.geometry())
        throw "Inconsistent image dimension";
    auto geo = It.geometry();
    d.resize(It.geometry());
    // multi resolution
    if (*std::min_element(geo.begin(),geo.end()) > 16)
    {
        //downsampling
        image_type rIs,rIt,rIs2,rIt2;
        downsample_with_padding(It,rIt);
        downsample_with_padding(Is,rIs);
        downsample_with_padding(It2,rIt2);
        downsample_with_padding(Is2,rIs2);
        float r = cdm2(rIt,rIt2,rIs,rIs2,d,terminated,resolution/2.0f,cdm_smoothness+0.05,iterations*2);
        upsample_with_padding(d,d,geo);
        d *= 2.0f;
        if(resolution > 1.0f)
            return r;
    }
    image_type Js,Js2;// transformed I
    dist_type new_d(d.geometry()),new_d2(d.geometry());// new displacements
    float theta = 0.0;

    std::deque<float> r,iter;
    for (unsigned int index = 0;index < iterations && !terminated;++index)
    {
        compose_displacement(Is,d,Js);
        compose_displacement(Is2,d,Js2);
        // dJ(cJ-I)
        r.push_back((cdm_get_gradient(Js,It,new_d)+cdm_get_gradient(Js2,It2,new_d2))*0.5f);
        iter.push_back(index);
        if(!cdm_improved(r,iter))
            break;
        add(new_d,new_d2);
        // solving the poisson equation using Jacobi method
        cdm_solve_poisson(new_d,terminated);
        cdm_accumulate_dis(d,new_d,theta,cdm_smoothness);
    }
    return r.front();
}


}// namespace reg
}// namespace image
#endif // DMDM_HPP
