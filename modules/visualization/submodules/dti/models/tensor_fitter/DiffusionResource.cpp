//#include "DiffusionResource.h"
//#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"

//std::vector<DiffusionResource::DiffusionDataRecord> DiffusionResource::m_diff_record;
//bool DiffusionResource::m_is_grayscale = true;

//DiffusionResource::DiffusionResource()
//{

//}

//DiffusionResource::~DiffusionResource()
//{

//}

//DiffusionResource::DiffusionDataRecord* DiffusionResource::getRecord(unsigned short acq_id)
//{
//    for(unsigned short i = 0; i < m_diff_record.size(); i++)
//        if(m_diff_record[i].m_res_diff_acq->getId() == acq_id)
//            return &m_diff_record[i];

//    return NULL;
//}

//DiffusionResource::DiffusionDataRecord* DiffusionResource::getRecordByIndex(unsigned short index)
//{
//    if(index < m_diff_record.size())
//        return &m_diff_record[index];

//    return NULL;
//}
//bool DiffusionResource::isGrayscale()
//{
//    return m_is_grayscale;
//}

//void DiffusionResource::setIsGrayscale(bool _isGrayscale)
//{
//    m_is_grayscale = _isGrayscale;
//}


//void DiffusionResource::addRecord(vmtkDiffAcquisition *acq)
//{
//    std::lock_guard<std::mutex> guard(m_mutex);
    
//    DiffusionDataRecord *rec = getRecord(acq->getId());
//    if(rec == NULL)
//    {
//        DiffusionDataRecord new_rec;
//        new_rec.m_res_diff_acq = acq;
//        m_diff_record.push_back(new_rec);
//    }
//}

//vmtkDiffAcquisition* DiffusionResource::getAcq(short acq_id)
//{
//    DiffusionDataRecord *rec = getRecord(acq_id);
//    if(rec != NULL)
//        return rec->m_res_diff_acq;

//    std::cerr << "DiffusionResource::getAcq -> Record not found!" << std::endl;
//    return NULL;
//}

//void DiffusionResource::renderGlyphsFlag(short acq_id, bool render_glyph_flag)
//{
//    DiffusionDataRecord *rec = getRecord(acq_id);
//    if(rec!= NULL)
//    {
//        rec->m_render_glyphs_flag = render_glyph_flag;
//    }
//}

//bool DiffusionResource::getRenderGlyphsFlag(short acq_id)
//{
//    DiffusionDataRecord *rec = getRecord(acq_id);
//    if(rec != NULL)
//        return rec->m_render_glyphs_flag;

//    std::cerr << "DiffusionResource::getRenderGlyphsFlag -> Record not found!" << std::endl;
//    return false;
//}

//unsigned short DiffusionResource::recordSize()
//{
//    return m_diff_record.size();
//}
