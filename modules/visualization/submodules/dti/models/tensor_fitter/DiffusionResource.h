#ifndef DIFFUSION_RESOURCE_H
#define DIFFUSION_RESOURCE_H

//#include <iostream>
//#include <vector>
//#include <mutex>

//class vmtkDiffAcquisition;

//class DiffusionResource
//{
//public:
//    DiffusionResource();
//    ~DiffusionResource();

//    void addRecord(vmtkDiffAcquisition *acq);
//    vmtkDiffAcquisition* getAcq(short acq_id);

//    void renderGlyphsFlag(short acq_id, bool render_glyph_flag);
//    bool getRenderGlyphsFlag(short acq_id);

//    unsigned short recordSize();
//    static bool isGrayscale();
//    static void setIsGrayscale(bool _isGrayscale);

//private:

//    typedef struct diffusion_data_record
//    {
//        bool m_render_glyphs_flag;
//        vmtkDiffAcquisition *m_res_diff_acq;

//        diffusion_data_record ()
//        {
//            m_render_glyphs_flag = false;
//            m_res_diff_acq = NULL;
//        }

//    } DiffusionDataRecord;

//    DiffusionDataRecord *getRecord(unsigned short acq_id);
//    DiffusionDataRecord* getRecordByIndex(unsigned short index);

//    short m_acq_id;
//    static std::vector<DiffusionDataRecord> m_diff_record;
//    std::mutex m_mutex;

//    static bool m_is_grayscale;
//};

#endif // DIFFUSION_RESOURCE_H
