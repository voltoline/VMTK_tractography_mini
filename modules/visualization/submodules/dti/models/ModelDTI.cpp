#include "ModelDTI.h"

#include "tractography/FiberTrackingAlgorithmComputeShader.h"
#include "tractography/FiberTrackingAlgorithmGeometryShader.h"
#include "utilities/QCheckingOpenGL.h"

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
#include <chrono>
#endif

ModelDTI::ModelDTI()
{
    m_fiberTrackingAlgorithm = nullptr;
    m_dti = nullptr;
    m_isDTIComputed = false;

    m_currentScalarIndex = ScalarIndex::NONE;

    m_currentFiberBundleEditionIndex = -1;

    m_fiberBundleIDCount = 0;

    m_fiberLengthMin = 40;
    m_fiberLengthMax = 1000;
    m_stoppingCriterionAngle = 11;
    m_stoppingCriterionFA = 0.15;
    m_seedCreationFA = 0.6f;
    m_wpunct = 0.5;

    m_deletedFiberBundleID = -1;

    m_isAutomaticFiberTrackingEnabled = true;
    m_isROIBasedFiberTracking = true;

    m_isScalarIndexRenderingActivated = false;

    m_dtiProgressBar = nullptr;
    m_acquisition = nullptr;

    m_ROIPlaneAngle = 40;
    m_isROIPlaneFilterActivated = true;
}

void ModelDTI::setAcquisition(vmtkDiffAcquisition *acquisition)
{
    m_acquisition = acquisition;
}

vmtkDiffAcquisition *ModelDTI::getAcquisition() const
{
    return m_acquisition;
}

const DTI *ModelDTI::getDTI()
{
    return m_dti;
}

void ModelDTI::setProgressBar(ProgressDialog *progress_bar)
{
    m_dtiProgressBar = progress_bar;
}

void ModelDTI::setROISeed(const std::vector<int> roi_seed_voxel_indices)
{
    FiberTrackingSeedCreator seed_creator;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedBuffer = seed_creator.createSeedsFromVoxelIndices(m_dti, m_seedCreationFA, roi_seed_voxel_indices);
}

void ModelDTI::setANDFilter(const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter)
{
    FiberTrackingFilterCreator and_creator;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer = and_creator.createFilterFromVoxelIndices(*m_acquisition, and_filter_indices, and_number_filter);
}

void ModelDTI::setNOTFilter(const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter)
{
    FiberTrackingFilterCreator not_creator;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer = not_creator.createFilterFromVoxelIndices(*m_acquisition, not_filter_indices, not_number_filter);
}

void ModelDTI::setROISeedAndFilters(const std::vector<int> roi_seed_voxel_indices,
                                             const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter,
                                             const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter)
{
    if(m_currentFiberBundleEditionIndex >= 0)
    {
        m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_roiSeedVvoxelIndices = roi_seed_voxel_indices;

        setROISeed(roi_seed_voxel_indices);
        setANDFilter(and_filter_indices, and_number_filter);
        setNOTFilter(not_filter_indices, not_number_filter);

        if(m_isAutomaticFiberTrackingEnabled)
            computeTractography();

        else {
            m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isUpdated = false;
            addEvent(ModelDTIEvent::ROI_AND_FILTER_CHANGED);
        }

    } else {

        std::cerr << "ModelDTI::setROISeedAndFilters -->> Attemp to edit an invalid fiber index = " << m_currentFiberBundleEditionIndex << "!" << std::endl;
    }

}

void ModelDTI::setFilters(const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter, const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter)
{
    if(m_currentFiberBundleEditionIndex >= 0)
    {
        setANDFilter(and_filter_indices, and_number_filter);
        setNOTFilter(not_filter_indices, not_number_filter);

        if(m_isAutomaticFiberTrackingEnabled)
            computeTractography();

        else {
            m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isUpdated = false;
            addEvent(ModelDTIEvent::ROI_AND_FILTER_CHANGED);
        }

    } else {

        std::cerr << "ModelDTI::setFilters -->> Attemp to edit an invalid fiber index = " << m_currentFiberBundleEditionIndex << "!" << std::endl;
    }
}

void ModelDTI::setAutomaticFiberTrackingEnabled(bool automaticFiberTrackingEnabled)
{
    m_isAutomaticFiberTrackingEnabled = automaticFiberTrackingEnabled;
}

void ModelDTI::enableROIBasedFiberTracking(bool state)
{
    m_isROIBasedFiberTracking = state;
}

void ModelDTI::computeTractography()
{
    if(m_isROIBasedFiberTracking)
        computeTractographyFromROIs();
    else
        computeGlobalTractography();
}

bool ModelDTI::isAutomaticFiberTrackingEnabled() const
{
    return m_isAutomaticFiberTrackingEnabled;
}

bool ModelDTI::isFiberBundleUpdated(unsigned short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index].m_isUpdated;

    std::cerr << "ModelDTI::isFiberBundleUpdated -->> Index out of range!" << std::endl;
    return true;
}

void ModelDTI::getROIPlaneFilterParameters(bool &roi_plane_filter_activated, int &roi_angle_filter)
{
    roi_plane_filter_activated = m_isROIPlaneFilterActivated;
    roi_angle_filter = m_ROIPlaneAngle;
}

void ModelDTI::loadDTI(std::string path_file)
{
    if(m_dti == nullptr)
        m_dti = new DTI();

    m_dti->loadTensorCoefficientsBinary(path_file);

    m_isDTIComputed = true;

    addEvent(ModelDTIEvent::TENSORS_COMPUTED);
}

void ModelDTI::setDTI(ModelDTI *model_dti)
{
    if(model_dti->m_dti == nullptr)
        return;

    m_dti = model_dti->m_dti;

    m_isDTIComputed = true;

    addEvent(ModelDTIEvent::TENSORS_COMPUTED);
}

void ModelDTI::computeGlobalTractography(/*std::vector<int> filter_and_indices,
                                                  std::vector<int> and_number_filter,
                                                  std::vector<int> filter_not_indices,
                                                  std::vector<int> not_number_filter*/
                                                  )
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::computeTractography -->> A notification is in progress!" << std::endl;
    }

    if(m_currentFiberBundleEditionIndex == -1)
    {
        std::cerr << "ModelDTI::computeTractography -->> Fiber Bundle not selected for edition!" << std::endl;
        return;
    }

    if(m_fiberTrackingAlgorithm == nullptr)
    {
        int major;
        int minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::cout << "OpenGL Version ModelDTI:  " << major <<   "." << minor << std::endl;

        if(major >= 4 && minor >= 3)
            m_fiberTrackingAlgorithm = new FiberTrackingAlgorithmComputeShader(m_dti);
        else
            m_fiberTrackingAlgorithm = new FiberTrackingAlgorithmGeometryShader(m_dti);

        m_fiberTrackingAlgorithm->initialize();
    }

    FiberTrackingSeedCreator seed_creator;
    FiberTrackingSeedCreator::SeedBuffer seed_buffer = seed_creator.createSeedsFromWholeBrain(m_dti, m_seedCreationFA);

    m_fiberTrackingAlgorithm->setSeeds(seed_buffer.mSeedBuffer, seed_buffer.mNumSeeds);

    m_fiberTrackingAlgorithm->setFilterOnTexture(m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mFilterBuffer,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mSizeFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mFilterIndex,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mNumFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mFilterBuffer,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mSizeFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mFilterIndex,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mNumFilter);


    m_fiberTrackingAlgorithm->setMinSteps(m_fiberLengthMin);
    m_fiberTrackingAlgorithm->setMaxSteps(m_fiberLengthMax);
    m_fiberTrackingAlgorithm->setStoppingCriterionFA(m_stoppingCriterionFA);
    m_fiberTrackingAlgorithm->setStoppingCriterionAngle(m_stoppingCriterionAngle);
    m_fiberTrackingAlgorithm->setWpunct(m_wpunct);

    m_fiberTrackingAlgorithm->setROIPlaneAngle(m_ROIPlaneAngle);
    m_fiberTrackingAlgorithm->setActivatedROIPlaneFilter(m_isROIPlaneFilterActivated);

    FiberClusterSet::sPtr fiber_cluster_set = m_fiberTrackingAlgorithm->startFiberTracking();
    fiber_cluster_set->setOutputFileOrientation(FiberClusterSet::Orientation::RAS);

    delete[] seed_buffer.mSeedBuffer;

    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberClusterSet = fiber_cluster_set;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMin = m_fiberLengthMin;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMax = m_fiberLengthMax;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionAngle = m_stoppingCriterionAngle;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionFA = m_stoppingCriterionFA;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedCreationFA = m_seedCreationFA;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_wpunct = m_wpunct;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isVisible = true;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isUpdated = true;

    addEvent(ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED);
}

void ModelDTI::computeTractographyFromROIs()
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::computeTractographyFromROIs -->> A notification is in progress!" << std::endl;
    }

    if(m_currentFiberBundleEditionIndex == -1)
    {
        std::cerr << "ModelDTI::computeTractography -->> Fiber Bundle not selected for edition!" << std::endl;
        return;
    }

    if(m_fiberTrackingAlgorithm == nullptr)
    {
        int major;
        int minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::cout << "OpenGL Version ModelDTI:  " << major <<   "." << minor << std::endl;

        if(major >= 4 && minor >= 3)
            m_fiberTrackingAlgorithm = new FiberTrackingAlgorithmComputeShader(m_dti);
        else
            m_fiberTrackingAlgorithm = new FiberTrackingAlgorithmGeometryShader(m_dti);

        m_fiberTrackingAlgorithm->initialize();
    }

    m_fiberTrackingAlgorithm->setSeeds(m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedBuffer->mSeedBuffer,
                                       m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedBuffer->mNumSeeds);

    m_fiberTrackingAlgorithm->setFilterOnTexture(m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mFilterBuffer,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mSizeFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mFilterIndex,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_notFilterBuffer->mNumFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mFilterBuffer,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mSizeFilter,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mFilterIndex,
                                                 m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_andFilterBuffer->mNumFilter);

    m_fiberTrackingAlgorithm->setMinSteps(m_fiberLengthMin);
    m_fiberTrackingAlgorithm->setMaxSteps(m_fiberLengthMax);
    m_fiberTrackingAlgorithm->setStoppingCriterionFA(m_stoppingCriterionFA);
    m_fiberTrackingAlgorithm->setStoppingCriterionAngle(m_stoppingCriterionAngle);
    m_fiberTrackingAlgorithm->setWpunct(m_wpunct);

    m_fiberTrackingAlgorithm->setROIPlaneAngle(m_ROIPlaneAngle);
    m_fiberTrackingAlgorithm->setActivatedROIPlaneFilter(m_isROIPlaneFilterActivated);

    FiberClusterSet::sPtr fiber_cluster_set = nullptr;

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
    double times_reg[10], preproc[10], track_time[10], query_time[10];
    double acc = 0, acc_preproc = 0, acc_track = 0, acc_query = 0;

    for(unsigned int i = 0; i < 10; i++)
    {
        fiber_cluster_set = nullptr;
        auto start = std::chrono::steady_clock::now();
#endif

        fiber_cluster_set = m_fiberTrackingAlgorithm->startFiberTracking();

#ifdef TRACTOGRAPHY_RENDER_TIME_MEASURE
        auto end = std::chrono::steady_clock::now();

        times_reg[i] = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        preproc[i] = m_fiberTrackingAlgorithm->preprocessing_time;
        track_time[i] = m_fiberTrackingAlgorithm->track_time;
        query_time[i] = m_fiberTrackingAlgorithm->query_time;

        acc += times_reg[i];
        acc_preproc += preproc[i];
        acc_track += track_time[i];
        acc_query += query_time[i];
    }

    for(unsigned int i = 0; i < 10; i++)
        std::cout << "Elapsed time in milliseconds: " << times_reg[i] << " ms - " << preproc[i] << " ms " << track_time[i] << " ms - q " << query_time[i] << " ms" << std::endl;

    std::cout << "mean Elapsed time in milliseconds: " << (acc / 10.0) << " ms - " << (acc_preproc / 10.0) << " ms " << (acc_track / 10.0) << " ms - q" << (acc_query / 10.0) << " ms" << std::endl;
#endif

    fiber_cluster_set->setOutputFileOrientation(FiberClusterSet::Orientation::RAS);

    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberClusterSet = fiber_cluster_set;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMin = m_fiberLengthMin;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMax = m_fiberLengthMax;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionAngle = m_stoppingCriterionAngle;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionFA = m_stoppingCriterionFA;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedCreationFA = m_seedCreationFA;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_wpunct = m_wpunct;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isVisible = true;
    m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_isUpdated = true;

    addEvent(ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED);
}

bool ModelDTI::isDTIComputed() const
{
    return m_isDTIComputed;
}

FiberClusterSet::sPtr ModelDTI::getFiberBundleSet(short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index].m_fiberClusterSet;

    else {

        std::cerr << "ModelDTI::getFiberBundle -->> fiber_bundle_index out of index!" << std::endl;
        return nullptr;
    }
}

void ModelDTI::setScalarIndexRenderingActivated(bool state)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setCurrentDiffusionVolumeMode -->> A notification is in progress!" << std::endl;
    }

    if(state != m_isScalarIndexRenderingActivated)
    {
        if(m_isDTIComputed)
        {
            m_isScalarIndexRenderingActivated = state;
            addEvent(ModelDTIEvent::SCALAR_INDEX_RENDERING_STATE_CHANGED);
        }
    }
}

bool ModelDTI::isScalarIndexRenderingActivated() const
{
    return m_isScalarIndexRenderingActivated;
}

ModelDTI::ScalarIndex ModelDTI::getCurrentScalarIndexMode() const
{
    return m_currentScalarIndex;
}

void ModelDTI::setCurrentScalarIndexMode(const ScalarIndex &currentScalarIndex)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setCurrentScalarIndexMode -->> A notification is in progress!" << std::endl;
    }

    if(m_isScalarIndexRenderingActivated)
    {
        if(m_isDTIComputed)
        {
            m_currentScalarIndex = currentScalarIndex;
            addEvent(ModelDTIEvent::SCALAR_INDEX_CHANGED);
            return ;
        }
    }
}

void ModelDTI::setCurrentFiberBundleEditionIndex(short current_fiber_bundle_edition_index)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setCurrentFiberBundleEditionIndex -->> A notification is in progress!" << std::endl;
    }

    if(current_fiber_bundle_edition_index >= -1 && current_fiber_bundle_edition_index < (int)m_fiberBundleDataVector.size())
    {
        if(m_currentFiberBundleEditionIndex != current_fiber_bundle_edition_index)
        {
            for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
            {
                if(current_fiber_bundle_edition_index == i && isFiberBundleReconstructed(i))
                    m_fiberBundleDataVector[i].m_isVisible = true;
                else
                    m_fiberBundleDataVector[i].m_isVisible = false;
            }

            m_currentFiberBundleEditionIndex = current_fiber_bundle_edition_index;

            m_fiberLengthMin = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMin;
            m_fiberLengthMax = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_fiberLengthMax;
            m_stoppingCriterionAngle = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionAngle;
            m_stoppingCriterionFA = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_stoppingCriterionFA;
            m_seedCreationFA = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_seedCreationFA;
            m_wpunct = m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_wpunct;

            addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

            addEvent(ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED);

            addEvent(ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
            return;
        }

    } else {

        std::cerr << "ModelDTI::setCurrentFiberBundleEditionIndex -->> Index out of range!" << std::endl;

        m_currentFiberBundleEditionIndex = -1;
        addEvent(ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
        return;
    }
}

short ModelDTI::getCurrentFiberBundleEditionIndex() const
{
    return m_currentFiberBundleEditionIndex;
}

void ModelDTI::getFiberBundleTrackingParameters(short fiber_bundle_index,
                                    unsigned int &fiber_length_min, unsigned int &fiber_length_max,
                                    unsigned short &stopping_criterion_angle, float &stopping_criterion_fa, float &seed_creation_fa, float &wpunct)
{
    if(fiber_bundle_index < (int)m_fiberBundleDataVector.size())
    {
        FiberBundleData &data = m_fiberBundleDataVector[fiber_bundle_index];

        fiber_length_min = data.m_fiberLengthMin;
        fiber_length_max = data.m_fiberLengthMax;
        stopping_criterion_angle = data.m_stoppingCriterionAngle;
        stopping_criterion_fa = data.m_stoppingCriterionFA;
        seed_creation_fa = data.m_seedCreationFA;
        wpunct = data.m_wpunct;

    } else {

        std::cerr << "ModelDTI::getFiberTrackingData -->> fiber_bundle_index out of index!" << std::endl;

        fiber_length_min = m_fiberLengthMin;
        fiber_length_max = m_fiberLengthMax;
        stopping_criterion_angle = m_stoppingCriterionAngle;
        stopping_criterion_fa = m_stoppingCriterionFA;
        seed_creation_fa = m_seedCreationFA;
        wpunct = m_wpunct;
    }
}

void ModelDTI::getCurrentTrackingParameters(unsigned int &fiber_length_min, unsigned int &fiber_length_max,
                                            unsigned short &stopping_criterion_angle, float &stopping_criterion_fa,
                                            float &seed_creation_fa, float &wpunct)
{
    fiber_length_min = m_fiberLengthMin;
    fiber_length_max = m_fiberLengthMax;
    stopping_criterion_angle = m_stoppingCriterionAngle;
    stopping_criterion_fa = m_stoppingCriterionFA;
    seed_creation_fa = m_seedCreationFA;
    wpunct = m_wpunct;
}

std::string ModelDTI::getFiberBundleName(short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index].m_fiberBundleName;

    else {

        std::cerr << "ModelDTI::getFiberBundleName -->> fiber_bundle_index out of index!" << std::endl;
        return std::string("");
    }
}

void ModelDTI::setFiberBundleName(short fiber_bundle_index, std::string name)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setFiberBundleName -->> A notification is in progress!" << std::endl;
    }

    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
    {
        m_fiberBundleDataVector[fiber_bundle_index].m_fiberBundleName = name;
        addEvent(ModelDTIEvent::FIBER_BUNDLE_NAME_CHANGED);

    } else {

        std::cerr << "ModelDTI::setFiberBundleName -->> fiber_bundle_index out of index!" << std::endl;
    }
}

bool ModelDTI::isFiberBundleVisible(short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index].m_isVisible;

    else {

        std::cerr << "ModelDTI::isFiberBundleVisible -->> fiber_bundle_index out of index!" << std::endl;
        return false;
    }
}

void ModelDTI::setFiberBundleVisible(short fiber_bundle_index, bool visible)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setFiberBundleVisible -->> A notification is in progress!" << std::endl;
    }

    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
    {
        if(m_fiberBundleDataVector[fiber_bundle_index].m_isVisible == visible)
            return;

        m_fiberBundleDataVector[fiber_bundle_index].m_isVisible = visible;

        addEvent(ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED);

        for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
        {
            if(m_fiberBundleDataVector[fiber_bundle_index].m_isVisible && fiber_bundle_index != i)
            {
                m_currentFiberBundleEditionIndex = -1;
                addEvent(ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
            }
        }

    } else {

        std::cerr << "ModelDTI::setFiberBundleVisible -->> fiber_bundle_index out of index!" << std::endl;
    }
}

std::vector<std::pair<unsigned short, bool> > ModelDTI::getFiberBundlesVisibility()
{
    std::vector<std::pair<unsigned short, bool>> indices;
    for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
        indices.push_back({i, m_fiberBundleDataVector[i].m_isVisible});

    return indices;
}

bool ModelDTI::isFiberBundleReconstructed(short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
        return (m_fiberBundleDataVector[fiber_bundle_index].m_fiberClusterSet != nullptr);

    else {

        std::cerr << "ModelDTI::isFiberBundleReconstructed -->> fiber_bundle_index out of index!" << std::endl;
        return false;
    }
}

void ModelDTI::addFiberBundleData(FiberBundleData fiber_bundle_data)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::addFiberBundleData -->> A notification is in progress!" << std::endl;
    }

    if(fiber_bundle_data.m_id >= m_fiberBundleIDCount)
        m_fiberBundleIDCount = fiber_bundle_data.m_id + 1;

    m_fiberBundleDataVector.insert(m_fiberBundleDataVector.begin(), fiber_bundle_data);

    addEvent(ModelDTIEvent::FIBER_BUNDLE_ADDED);

    if(fiber_bundle_data.m_isVisible)
    {
        addEvent(ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED);
    }
}

ModelDTI::FiberBundleData ModelDTI::getFiberBundleData(unsigned short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index];

    else {

        std::cerr << "ModelDTI::getFiberBundleData -->> index out of range! fiber_bundle_index = " << fiber_bundle_index << std::endl;
        return FiberBundleData();
    }
}

ModelDTI::FiberBundleData ModelDTI::getFiberBundleDataByID(unsigned short fiber_bundle_id)
{
    for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
    {
        if(m_fiberBundleDataVector[i].m_id == fiber_bundle_id)
            return m_fiberBundleDataVector[i];
    }

    std::cerr << "ModelDTI::getFiberBundleDataByID -->> ID " << fiber_bundle_id << " not found!" << std::endl;
    return FiberBundleData();
}

void ModelDTI::setCurrentFiberBundleData(ModelDTI::FiberBundleData fiber_bundle_data)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::setCurrentFiberBundleData -->> A notification is in progress!" << std::endl;
    }

    if(m_currentFiberBundleEditionIndex >= 0)
    {
        m_fiberBundleDataVector[m_currentFiberBundleEditionIndex] = fiber_bundle_data;
        addEvent(ModelDTIEvent::CURRENT_FIBER_BUNDLE_CHANGED);

    } else {

        std::cerr << "ModelDTI::setCurrentFiberBundleData -->> index out of range!" << std::endl;
    }
}

std::vector<FiberClusterSet::sPtr> ModelDTI::getVisibleFiberBundles()
{
    std::vector<FiberClusterSet::sPtr> visible_fibers;

    for(FiberBundleData &data: m_fiberBundleDataVector)
        if(data.m_isVisible)
            visible_fibers.push_back(data.m_fiberClusterSet);

    return visible_fibers;
}

unsigned short ModelDTI::getFiberBundleCount()
{
    return m_fiberBundleDataVector.size();
}

void ModelDTI::addFiberBundle(bool is_visible, std::string tract_name, bool current_edition)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::addFiberBundle -->> A notification is in progress!" << std::endl;
    }

    FiberBundleData data;
    data.m_isVisible = is_visible;
    data.m_fiberBundleName = tract_name;
    data.m_id = m_fiberBundleIDCount;
    data.m_fiberLengthMin = m_fiberLengthMin;
    data.m_fiberLengthMax = m_fiberLengthMax;
    data.m_stoppingCriterionAngle = m_stoppingCriterionAngle;
    data.m_stoppingCriterionFA = m_stoppingCriterionFA;
    data.m_seedCreationFA = m_seedCreationFA;
    data.m_wpunct = m_wpunct;

    m_fiberBundleIDCount++;

    m_fiberBundleDataVector.insert(m_fiberBundleDataVector.begin(), data);

    bool visibility_changed = false;
    for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
    {
        if(m_fiberBundleDataVector[i].m_isVisible)
            visibility_changed = true;

        m_fiberBundleDataVector[i].m_isVisible = false;
    }

    addEvent(ModelDTIEvent::FIBER_BUNDLE_ADDED);

    if(visibility_changed)
    {
        addEvent(ModelDTIEvent::VISIBLE_FIBER_BUNDLES_CHANGED);
    }

    if(current_edition)
    {
        m_currentFiberBundleEditionIndex = 0;
        addEvent(ModelDTIEvent::CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED);
    }
}


void ModelDTI::deleteFiberBundleByIndex(short fiber_bundle_index)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::deleteFiberBundleByIndex -->> A notification is in progress!" << std::endl;
    }

    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
    {
        m_deletedFiberBundleID = m_fiberBundleDataVector[fiber_bundle_index].m_id;

        m_fiberBundleDataVector.erase(m_fiberBundleDataVector.begin() + fiber_bundle_index);

        if(fiber_bundle_index == m_currentFiberBundleEditionIndex)
            m_currentFiberBundleEditionIndex = -1;

        addEvent(ModelDTIEvent::FIBER_BUNDLE_DELETED);

    } else {

        std::cerr << "ModelDTI::deleteFiberBundle -->> fiber_bundle_index out of index!" << std::endl;
    }
}

void ModelDTI::deleteFiberBundleByID(short fiber_bundle_id)
{
    if(isNotifying())
    {
        std::cerr << "ModelDTI::deleteFiberBundleByID -->> A notification is in progress!" << std::endl;
    }

    for(unsigned short i = 0; i < m_fiberBundleDataVector.size(); i++)
    {
        if(m_fiberBundleDataVector[i].m_id == fiber_bundle_id)
        {
            m_deletedFiberBundleID = m_fiberBundleDataVector[i].m_id;
            m_fiberBundleDataVector.erase(m_fiberBundleDataVector.begin() + i);

            if(i == m_currentFiberBundleEditionIndex)
                m_currentFiberBundleEditionIndex = -1;

            addEvent(ModelDTIEvent::FIBER_BUNDLE_DELETED);
            return;
        }
    }

    std::cerr << "ModelDTI::deleteFiberBundle -->> fiber_bundle_id not found!" << std::endl;
}

short ModelDTI::getFiberBundleID(short fiber_bundle_index)
{
    if(fiber_bundle_index >= 0 && fiber_bundle_index < (int)m_fiberBundleDataVector.size())
        return m_fiberBundleDataVector[fiber_bundle_index].m_id;

    else {

        std::cerr << "ModelDTI::getFiberBundleID -->> fiber_bundle_index out of index!" << std::endl;
        return -1;
    }
}

void ModelDTI::setFiberLengthMin(unsigned int fiberLengthMin)
{
    m_fiberLengthMin = fiberLengthMin;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setFiberLengthMax(unsigned int fiberLengthMax)
{
    m_fiberLengthMax = fiberLengthMax;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setStoppingCriterionAngle(unsigned short stoppingCriterionAngle)
{
    m_stoppingCriterionAngle = stoppingCriterionAngle;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setStoppingCriterionFA(float stoppingCriterionFA)
{
    m_stoppingCriterionFA = stoppingCriterionFA;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setSeedCreationFA(float seedCreationFA)
{
    m_seedCreationFA = seedCreationFA;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_currentFiberBundleEditionIndex >= 0)
        if(m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_roiSeedVvoxelIndices.size() > 0)
            setROISeed(m_fiberBundleDataVector[m_currentFiberBundleEditionIndex].m_roiSeedVvoxelIndices);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setWpunct(float wpunct)
{
    m_wpunct = wpunct;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setROIPlaneAngle(float angle)
{
    m_ROIPlaneAngle = angle;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

void ModelDTI::setActivatedROIPlaneFilter(float value)
{
    m_isROIPlaneFilterActivated = value;
    addEvent(ModelDTIEvent::TRACKING_PARAMETER_CHANGED);

    if(m_isAutomaticFiberTrackingEnabled && m_currentFiberBundleEditionIndex >= 0)
        computeTractography();
}

short ModelDTI::getDeletedFiberBundleID() const
{
    return m_deletedFiberBundleID;
}
