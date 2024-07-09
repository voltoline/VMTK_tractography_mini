#include "vmtkReorientImageVolumeToLPS.h"
#include <iostream>
#include <cmath>
#include <cstring>

bool vmtkReorientImageVolumeToLPS::reorient(vmtkAcquisition *acquisition)
{
    unsigned int ix, iy, iz, idx, idy, idz;
    unsigned int dimx, dimy, dimz;
    float sx, sy, sz;
    unsigned char flag;
    unsigned char code;

    unsigned char *tmp = new unsigned char[2*acquisition->m_dimension[0]*acquisition->m_dimension[1]];

    std::cout << "Reorient the samples in LPS-reference ..." << std::endl;

    /*
     * Orientation code: code
     * X-> RL: code += 0; PA: code += 16; FH: code += 32;
     * Y-> RL: code += 0; PA: code +=  4; FH: code +=  8;
     * Z-> RL: code += 0; PA: code +=  1; FH: code +=  2;
     */


    code = 0;

    // Reflection
    // X-axis (bits [4:5]=> LR = 0b00, AP = 0b01; FH = 0b10)
    flag = 0;
    if (fabs(acquisition->m_dircos[0][0]) > fabs(acquisition->m_dircos[0][1]) &&
            fabs(acquisition->m_dircos[0][0]) > fabs(acquisition->m_dircos[0][2])) {

        if (acquisition->m_dircos[0][0] < 0) {  // RL (21/05/2013 - Ting)
            flag = 1;
            acquisition->m_dircos[0][0] *= (-1);  // LR
            acquisition->m_dircos[0][1] *= (-1);
            acquisition->m_dircos[0][2] *= (-1);
        }
        code += 0;
    } else if (fabs(acquisition->m_dircos[0][1]) > fabs(acquisition->m_dircos[0][0])
               && fabs(acquisition->m_dircos[0][1]) > fabs(acquisition->m_dircos[0][2])) {
        if (acquisition->m_dircos[0][1] < 0) {  // PA
            flag = 1;
            acquisition->m_dircos[0][0] *= (-1);  // AP
            acquisition->m_dircos[0][1] *= (-1);
            acquisition->m_dircos[0][2] *= (-1);
        }
        code += 16;
    } else {
        if (acquisition->m_dircos[0][2] < 0) {  // HF
            flag = 1;
            acquisition->m_dircos[0][0] *= (-1);  // FH
            acquisition->m_dircos[0][1] *= (-1);
            acquisition->m_dircos[0][2] *= (-1);
        }
        code += 32;
    }
    if (flag) {
        switch (acquisition->m_bits_allocated) {
        case 8:
            for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                for (idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (ix=0,idx = acquisition->m_dimension[0]-1; ix < idx; idx--,ix++) {
                        ///Swap the elements
                        tmp[0]=acquisition->m_image_buffer[
                                idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                idy * acquisition->m_dimension[0] + idx];
                        acquisition->m_image_buffer[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                idy * acquisition->m_dimension[0] + idx] =
                                acquisition->m_image_buffer[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                idy * acquisition->m_dimension[0] + ix];
                        acquisition->m_image_buffer[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                idy * acquisition->m_dimension[0] + ix] = tmp[0];
                    }
                }
            }
            break;
        case 16:
            for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                for (idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (ix=0,idx = acquisition->m_dimension[0]-1; ix < idx; idx--,ix++) {
//                        for (ix=0,idx = acquisition->m_dimension[0]-1; ix < idx; idx=idx--,ix=ix++) {
                        ///Swap the elements
                        memcpy (tmp,
                                (*acquisition)(idx, idy, idz, 2),
                                sizeof(unsigned char)*2);

                        memcpy ((*acquisition)(idx, idy, idz, 2),
                                (*acquisition)(ix, idy, idz, 2),
                                sizeof(unsigned char)*2);

                        memcpy ((*acquisition)(ix, idy, idz, 2),
                                tmp,
                                sizeof(unsigned char)*2);
                    }
                }
            }
            break;
        }
    }

    /// Y-axis (bits [2:3]=> LR = 0b00, AP = 0b01; FH = 0b10)
    flag = 0;
    if (fabs(acquisition->m_dircos[1][0]) > fabs(acquisition->m_dircos[1][1])
            &&
            fabs(acquisition->m_dircos[1][0]) > fabs(acquisition->m_dircos[1][2])) {
        if (acquisition->m_dircos[1][0] < 0) {   // RL (21/05/2013 - Ting)
            flag = 1;
            acquisition->m_dircos[1][0] *= (-1);   // LR
            acquisition->m_dircos[1][1] *= (-1);
            acquisition->m_dircos[1][2] *= (-1);
        }
        code += 0;
    } else if (fabs(acquisition->m_dircos[1][1]) > fabs(acquisition->m_dircos[1][0])
               &&
               fabs(acquisition->m_dircos[1][1]) > fabs(acquisition->m_dircos[1][2])) {
        if (acquisition->m_dircos[1][1] < 0) {   // PA
            flag = 1;
            acquisition->m_dircos[1][0] *= (-1);   // AP
            acquisition->m_dircos[1][1] *= (-1);
            acquisition->m_dircos[1][2] *= (-1);
        }
        code += 4;
    } else {
        if (acquisition->m_dircos[1][2] < 0) {   // HF
            flag = 1;
            acquisition->m_dircos[1][0] *= (-1);   // FH
            acquisition->m_dircos[1][1] *= (-1);
            acquisition->m_dircos[1][2] *= (-1);
        }
        code += 8;
    }
    if (flag) {
        switch (acquisition->m_bits_allocated) {
        case 8:
            for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                for (iy=0,idy = acquisition->m_dimension[1]-1; iy < idy; idy--,iy++) {
                    memcpy (tmp,
                            (*acquisition)(0, idy, idz, 1),
                            sizeof(unsigned char) * acquisition->m_dimension[0]);

                    memcpy((*acquisition)(0, idy, idz, 1),
                            (*acquisition)(0, iy, idz, 1),
                            sizeof(unsigned char) * acquisition->m_dimension[0]);

                    memcpy((*acquisition)(0, iy, idz, 1),
                           tmp,
                           sizeof(unsigned char) * acquisition->m_dimension[0]);
                }
            }
            break;
        case 16:
            for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                for (iy=0,idy = acquisition->m_dimension[1]-1; iy < idy; idy--,iy++) {
                    memcpy (tmp,
                            (*acquisition)(0, idy, idz, 2),
                            sizeof(unsigned char) * 2 * acquisition->m_dimension[0]);

                    memcpy((*acquisition)(0, idy, idz, 2),
                            (*acquisition)(0, iy, idz, 2),
                            sizeof(unsigned char) * 2 * acquisition->m_dimension[0]);

                    memcpy((*acquisition)(0, iy, idz, 2),
                           tmp,
                           sizeof(unsigned char) * 2 * acquisition->m_dimension[0]);
                }
            }
            break;
        }
    }

    /// Depth (bits [0:1]=> LR = 0b00, AP = 0b01; FH = 0b10)
    flag = 0;
    // Recompute the normal direction vector
    //Ting: It is important to do the check on the basis of the position of the origin
    //Ting: We consider that negative value implies in the expected orientation
    if (fabs(acquisition->m_dircos[2][0]) > fabs(acquisition->m_dircos[2][1])
            &&
            fabs(acquisition->m_dircos[2][0]) > fabs(acquisition->m_dircos[2][2])) {
        if (acquisition->m_dircos[2][0] < 0 && acquisition->m_origin[0] > 0) {   // scanned in LR direction  (10/07/2017 - Ting)
            flag = 1;
            acquisition->m_dircos[2][0] *= (-1);   // LR
            acquisition->m_dircos[2][1] *= (-1);
            acquisition->m_dircos[2][2] *= (-1);
        }
        code += 0;
    } else if (fabs(acquisition->m_dircos[2][1]) > fabs(acquisition->m_dircos[2][0])
               &&
               fabs(acquisition->m_dircos[2][1]) > fabs(acquisition->m_dircos[2][2])) {
        if (acquisition->m_dircos[2][1] < 0 && acquisition->m_origin[1] > 0) {   // scanned in PA direction (10/07/2017 - Ting)
            flag = 1;
            acquisition->m_dircos[2][0] *= (-1);   // AP
            acquisition->m_dircos[2][1] *= (-1);
            acquisition->m_dircos[2][2] *= (-1);
        }
        code += 1;
    } else {
        if (acquisition->m_dircos[2][2] < 0 && acquisition->m_origin[2] > 0) {   // scanned in HF direction (10/07/2017 - Ting)
            flag = 1;
            acquisition->m_dircos[2][0] *= (-1);   // FH
            acquisition->m_dircos[2][1] *= (-1);
            acquisition->m_dircos[2][2] *= (-1);
        }
        code += 2;
    }
    if (flag) {
        switch (acquisition->m_bits_allocated) {
        case 8:
            for (iz=0,idz = acquisition->m_dimension[2]-1; iz < idz; idz-=1,iz+=1) {
                memcpy (tmp,
                        (*acquisition)(0, 0, idz, 1),
                        sizeof(unsigned char) * (acquisition->m_dimension[0] * acquisition->m_dimension[1]));

                memcpy ((*acquisition)(0, 0, idz, 1),
                        (*acquisition)(0, 0, iz, 1),
                        sizeof(unsigned char) * (acquisition->m_dimension[0] * acquisition->m_dimension[1]));

                memcpy ((*acquisition)(0, 0, iz, 1),
                        tmp,
                        sizeof(unsigned char) * (acquisition->m_dimension[0] * acquisition->m_dimension[1]));
            }
            break;
        case 16:
            for (iz=0,idz = acquisition->m_dimension[2]-1; iz < idz; idz-=1,iz+=1) {
                memcpy (tmp,
                        (*acquisition)(0, 0, idz, 2),
                        sizeof(unsigned char)*2*(acquisition->m_dimension[0] * acquisition->m_dimension[1]));

                memcpy ((*acquisition)(0, 0, idz, 2),
                        (*acquisition)(0, 0, iz, 2),
                        sizeof(unsigned char)*2*(acquisition->m_dimension[0] * acquisition->m_dimension[1]));

                memcpy ((*acquisition)(0, 0, iz, 2),
                        tmp,
                        sizeof(unsigned char)*2*(acquisition->m_dimension[0] * acquisition->m_dimension[1]));
            }
            break;
        }
    }

    delete[] tmp;

    if (code == 6) return true;

    // Rotation to LPS ([0][1][2])
    {
        double dir[3][3];

        switch (acquisition->m_bits_allocated) {
        case 8:
            tmp = new unsigned char[acquisition->m_dimension[0]*acquisition->m_dimension[1]*acquisition->m_dimension[2]];
            memcpy (tmp,acquisition->m_image_buffer,
                    sizeof(unsigned char)*(acquisition->m_dimension[0] * acquisition->m_dimension[1] * acquisition->m_dimension[2]));
            memset(acquisition->m_image_buffer, 0x00, acquisition->m_dimension[0]*acquisition->m_dimension[1]*acquisition->m_dimension[2]);

            if (code == 24) { // PSL
                std::cout << "PSL" << std::endl;
                // Bug fixed: scan order (Ting - 28/09/2015)
                for (ix = 0, idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                        for (idz=0; idz < acquisition->m_dimension[2]; idz++) {
                            acquisition->m_image_buffer[ix++] =
                                    tmp[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                    idy * acquisition->m_dimension[0] + idx];
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[2]; sy = acquisition->m_pixel_spacing[0]; sz = acquisition->m_pixel_spacing[1];
                dimx = acquisition->m_dimension[2]; dimy = acquisition->m_dimension[0]; dimz = acquisition->m_dimension[1];
                dir[0][0] = acquisition->m_dircos[2][0];
                dir[0][1] = acquisition->m_dircos[2][1];
                dir[0][2] = acquisition->m_dircos[2][2];
                dir[1][0] = acquisition->m_dircos[0][0];
                dir[1][1] = acquisition->m_dircos[0][1];
                dir[1][2] = acquisition->m_dircos[0][2];
                dir[2][0] = acquisition->m_dircos[1][0];
                dir[2][1] = acquisition->m_dircos[1][1];
                dir[2][2] = acquisition->m_dircos[1][2];
            } else if (code == 33) { // SLP
                std::cout << "SLP" << std::endl;
                for (ix = 0, idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                    for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                        for (idy=0; idy < acquisition->m_dimension[1]; idy++) {
                            acquisition->m_image_buffer[ix++] =
                                    tmp[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                    idy * acquisition->m_dimension[0] + idx];
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[1]; sy = acquisition->m_pixel_spacing[2]; sz = acquisition->m_pixel_spacing[0];
                dimx = acquisition->m_dimension[1]; dimy = acquisition->m_dimension[2]; dimz = acquisition->m_dimension[0];
                dir[0][0] = acquisition->m_dircos[1][0];
                dir[0][1] = acquisition->m_dircos[1][1];
                dir[0][2] = acquisition->m_dircos[1][2];
                dir[1][0] = acquisition->m_dircos[2][0];
                dir[1][1] = acquisition->m_dircos[2][1];
                dir[1][2] = acquisition->m_dircos[2][2];
                dir[2][0] = acquisition->m_dircos[0][0];
                dir[2][1] = acquisition->m_dircos[0][1];
                dir[2][2] = acquisition->m_dircos[0][2];
            } else if (code == 9) { // LSP
                std::cout << "LSP" << std::endl;
                for (ix = 0, idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                        for (idx=0; idx < acquisition->m_dimension[0]; idx++) {
                            acquisition->m_image_buffer[ix++] =
                                    tmp[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                    idy * acquisition->m_dimension[0] + idx];

                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[0]; sy = acquisition->m_pixel_spacing[2]; sz = acquisition->m_pixel_spacing[1];
                dimx = acquisition->m_dimension[0]; dimy = acquisition->m_dimension[2]; dimz = acquisition->m_dimension[1];
                dir[0][0] = acquisition->m_dircos[0][0];
                dir[0][1] = acquisition->m_dircos[0][1];
                dir[0][2] = acquisition->m_dircos[0][2];
                dir[1][0] = acquisition->m_dircos[2][0];
                dir[1][1] = acquisition->m_dircos[2][1];
                dir[1][2] = acquisition->m_dircos[2][2];
                dir[2][0] = acquisition->m_dircos[1][0];
                dir[2][1] = acquisition->m_dircos[1][1];
                dir[2][2] = acquisition->m_dircos[1][2];
            } else if (code == 36) { // SPL
                std::cout << "SPL" << std::endl;
                for (ix = 0, idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                    for (idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                        for (idz=0; idz < acquisition->m_dimension[2]; idz++) {
                            acquisition->m_image_buffer[ix++] =
                                    tmp[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                    idy * acquisition->m_dimension[0] + idx];
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[2]; sy = acquisition->m_pixel_spacing[1]; sz = acquisition->m_pixel_spacing[0];
                dimx = acquisition->m_dimension[2]; dimy = acquisition->m_dimension[1]; dimz = acquisition->m_dimension[0];
                dir[0][0] = acquisition->m_dircos[2][0];
                dir[0][1] = acquisition->m_dircos[2][1];
                dir[0][2] = acquisition->m_dircos[2][2];
                dir[1][0] = acquisition->m_dircos[1][0];
                dir[1][1] = acquisition->m_dircos[1][1];
                dir[1][2] = acquisition->m_dircos[1][2];
                dir[2][0] = acquisition->m_dircos[0][0];
                dir[2][1] = acquisition->m_dircos[0][1];
                dir[2][2] = acquisition->m_dircos[0][2];
            } else if (code == 18) { // PLS
                std::cout << "PLS" << std::endl;
                for (ix = 0, idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                    for (idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                        for (idy=0; idy < acquisition->m_dimension[1]; idy++) {
                            acquisition->m_image_buffer[ix++] =
                                    tmp[idz * acquisition->m_dimension[0] * acquisition->m_dimension[1] +
                                    idy * acquisition->m_dimension[0] + idx];
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[1]; sy = acquisition->m_pixel_spacing[0]; sz = acquisition->m_pixel_spacing[2];
                dimx = acquisition->m_dimension[1]; dimy = acquisition->m_dimension[0]; dimz = acquisition->m_dimension[2];
                dir[0][0] = acquisition->m_dircos[1][0];
                dir[0][1] = acquisition->m_dircos[1][1];
                dir[0][2] = acquisition->m_dircos[1][2];
                dir[1][0] = acquisition->m_dircos[0][0];
                dir[1][1] = acquisition->m_dircos[0][1];
                dir[1][2] = acquisition->m_dircos[0][2];
                dir[2][0] = acquisition->m_dircos[2][0];
                dir[2][1] = acquisition->m_dircos[2][1];
                dir[2][2] = acquisition->m_dircos[2][2];
            }
            break;
        case 16:
            tmp = new unsigned char[2*acquisition->m_dimension[0]*acquisition->m_dimension[1]*acquisition->m_dimension[2]];
            memcpy (tmp,acquisition->m_image_buffer,
                    sizeof(unsigned char)*(2*acquisition->m_dimension[0]*acquisition->m_dimension[1]*acquisition->m_dimension[2]));
            memset(acquisition->m_image_buffer, 0x00, 2*acquisition->m_dimension[0]*acquisition->m_dimension[1]*acquisition->m_dimension[2]);
            if (code == 24) { // PSL
                std::cout << "PSL" << std::endl;
                for (ix=0, idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                        for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                            memcpy(&(acquisition->m_image_buffer[2*ix]),&(tmp[2*(idz*acquisition->m_dimension[0]*acquisition->m_dimension[1]+idy*acquisition->m_dimension[0]+idx)]), sizeof(unsigned short));
                            ix++;
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[2]; sy = acquisition->m_pixel_spacing[0]; sz = acquisition->m_pixel_spacing[1];
                dimx = acquisition->m_dimension[2]; dimy = acquisition->m_dimension[0]; dimz = acquisition->m_dimension[1];
                dir[0][0] = acquisition->m_dircos[2][0];
                dir[0][1] = acquisition->m_dircos[2][1];
                dir[0][2] = acquisition->m_dircos[2][2];
                dir[1][0] = acquisition->m_dircos[0][0];
                dir[1][1] = acquisition->m_dircos[0][1];
                dir[1][2] = acquisition->m_dircos[0][2];
                dir[2][0] = acquisition->m_dircos[1][0];
                dir[2][1] = acquisition->m_dircos[1][1];
                dir[2][2] = acquisition->m_dircos[1][2];
            } else if (code == 33) { // SLP
                std::cout << "SLP" << std::endl;
                for (ix = 0, idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                    for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                        for (idy=0; idy < acquisition->m_dimension[1]; idy++) {
                            memcpy(&(acquisition->m_image_buffer[2*ix]),&(tmp[2*(idz*acquisition->m_dimension[0]*acquisition->m_dimension[1]+idy*acquisition->m_dimension[0]+idx)]), sizeof(unsigned short));
                            ix++;
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[1]; sy = acquisition->m_pixel_spacing[2]; sz = acquisition->m_pixel_spacing[0];
                dimx = acquisition->m_dimension[1]; dimy = acquisition->m_dimension[2]; dimz = acquisition->m_dimension[0];
                dir[0][0] = acquisition->m_dircos[1][0];
                dir[0][1] = acquisition->m_dircos[1][1];
                dir[0][2] = acquisition->m_dircos[1][2];
                dir[1][0] = acquisition->m_dircos[2][0];
                dir[1][1] = acquisition->m_dircos[2][1];
                dir[1][2] = acquisition->m_dircos[2][2];
                dir[2][0] = acquisition->m_dircos[0][0];
                dir[2][1] = acquisition->m_dircos[0][1];
                dir[2][2] = acquisition->m_dircos[0][2];
            } else if (code == 9) { // LSP
                std::cout << "LSP" << std::endl;
                // Bug fixed: scan order (Ting - 28/09/2015)
                for (ix = 0, idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                    for (idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                        for (idx=0; idx < acquisition->m_dimension[0]; idx++) {
                            memcpy(&(acquisition->m_image_buffer[2*ix]),&(tmp[2*(idz*acquisition->m_dimension[0]*acquisition->m_dimension[1]+idy*acquisition->m_dimension[0]+idx)]), sizeof(unsigned short));
                            ix++;
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[0]; sy = acquisition->m_pixel_spacing[2]; sz = acquisition->m_pixel_spacing[1];
                dimx = acquisition->m_dimension[0]; dimy = acquisition->m_dimension[2]; dimz = acquisition->m_dimension[1];
                dir[0][0] = acquisition->m_dircos[0][0];
                dir[0][1] = acquisition->m_dircos[0][1];
                dir[0][2] = acquisition->m_dircos[0][2];
                dir[1][0] = acquisition->m_dircos[2][0];
                dir[1][1] = acquisition->m_dircos[2][1];
                dir[1][2] = acquisition->m_dircos[2][2];
                dir[2][0] = acquisition->m_dircos[1][0];
                dir[2][1] = acquisition->m_dircos[1][1];
                dir[2][2] = acquisition->m_dircos[1][2];
            } else if (code == 36) { // SPL
                std::cout << "SPL" << std::endl;
                for (ix = 0, idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                    for (idy = 0; idy < acquisition->m_dimension[1]; idy++) {
                        for (idz=0; idz < acquisition->m_dimension[2]; idz++) {
                            memcpy(&(acquisition->m_image_buffer[2*ix]),&(tmp[2*(idz*acquisition->m_dimension[0]*acquisition->m_dimension[1]+idy*acquisition->m_dimension[0]+idx)]), sizeof(unsigned short));
                            ix++;
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[2]; sy = acquisition->m_pixel_spacing[1]; sz = acquisition->m_pixel_spacing[0];
                dimx = acquisition->m_dimension[2]; dimy = acquisition->m_dimension[1]; dimz = acquisition->m_dimension[0];
                dir[0][0] = acquisition->m_dircos[2][0];
                dir[0][1] = acquisition->m_dircos[2][1];
                dir[0][2] = acquisition->m_dircos[2][2];
                dir[1][0] = acquisition->m_dircos[1][0];
                dir[1][1] = acquisition->m_dircos[1][1];
                dir[1][2] = acquisition->m_dircos[1][2];
                dir[2][0] = acquisition->m_dircos[0][0];
                dir[2][1] = acquisition->m_dircos[0][1];
                dir[2][2] = acquisition->m_dircos[0][2];
            } else if (code == 18) { // PLS
                std::cout << "PLS" << std::endl;
                for (ix = 0, idz = 0; idz < acquisition->m_dimension[2]; idz++) {
                    for (idx = 0; idx < acquisition->m_dimension[0]; idx++) {
                        for (idy=0; idy < acquisition->m_dimension[1]; idy++) {
                            memcpy(&(acquisition->m_image_buffer[2*ix]),&(tmp[2*(idz*acquisition->m_dimension[0]*acquisition->m_dimension[1]+idy*acquisition->m_dimension[0]+idx)]), sizeof(unsigned short));
                            ix++;
                        }
                    }
                }
                sx = acquisition->m_pixel_spacing[1]; sy = acquisition->m_pixel_spacing[0]; sz = acquisition->m_pixel_spacing[2];
                dimx = acquisition->m_dimension[1]; dimy = acquisition->m_dimension[0]; dimz = acquisition->m_dimension[2];
                dir[0][0] = acquisition->m_dircos[1][0];
                dir[0][1] = acquisition->m_dircos[1][1];
                dir[0][2] = acquisition->m_dircos[1][2];
                dir[1][0] = acquisition->m_dircos[0][0];
                dir[1][1] = acquisition->m_dircos[0][1];
                dir[1][2] = acquisition->m_dircos[0][2];
                dir[2][0] = acquisition->m_dircos[2][0];
                dir[2][1] = acquisition->m_dircos[2][1];
                dir[2][2] = acquisition->m_dircos[2][2];
            }
            break;
        }

        // Update the dimensions, spacings and directions
        acquisition->m_pixel_spacing[0]=sx; acquisition->m_pixel_spacing[1]=sy; acquisition->m_pixel_spacing[2]=sz;
        acquisition->m_dimension[0]=dimx; acquisition->m_dimension[1]=dimy; acquisition->m_dimension[2]=dimz;
        acquisition->m_dircos[0][0] = dir[0][0]; acquisition->m_dircos[0][1] = dir[0][1]; acquisition->m_dircos[0][2] = dir[0][2];
        acquisition->m_dircos[1][0] = dir[1][0]; acquisition->m_dircos[1][1] = dir[1][1]; acquisition->m_dircos[1][2] = dir[1][2];
        acquisition->m_dircos[2][0] = dir[2][0]; acquisition->m_dircos[2][1] = dir[2][1]; acquisition->m_dircos[2][2] = dir[2][2];

        delete [] tmp;
    }

    return true;
}

vmtkReorientImageVolumeToLPS::vmtkReorientImageVolumeToLPS()
{

}
