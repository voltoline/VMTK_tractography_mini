/**
  * @file DTI.h
  * @author Raphael Voltoline
  * @author Wu Shin-Ting
  * @date 2019
  */
#ifndef MODEL_DTI_H
#define MODEL_DTI_H

#include "model/ModelSubject.h"
#include "modules/file_reader/dicom/vmtkDiffAcquisition.h"
#include "tensor_fitter/DTI.h"
#include "tractography/FiberClusterSet.h"
#include <memory>
#include "tractography/FiberTrackingSeedCreator.h"
#include "tractography/FiberTrackingFilterCreator.h"

class FiberTrackingAlgorithm;

/**
 * @enum ModelDTIEvent
 * Scoped enumeration for DWI processing events.
 */
enum class ModelDTIEvent
{
    TENSORS_COMPUTED,                        /**< Diffusion tensors computed */
    SCALAR_INDEX_CHANGED,                    /**< Scalar indices changed */
    SCALAR_INDEX_RENDERING_STATE_CHANGED,

    FIBER_BUNDLE_NAME_CHANGED,               /**< Name of the white matter (WM) fiber bundle changed */

    CURRENT_FIBER_BUNDLE_CHANGED,            /**< Current WM fiber bundle changed */
    CURRENT_EDITABLE_FIBER_BUNDLE_CHANGED,   /**< Current editable WM fiber bundle changed */

    FIBER_BUNDLE_ADDED,                      /**< A WM fiber bundle added */
    FIBER_BUNDLE_DELETED,                    /**< A WM fiber bundle removed */

    VISIBLE_FIBER_BUNDLES_CHANGED,           /**< Visible fiber bundles chnaged */

    TRACKING_PARAMETER_CHANGED,               /**< Tensorline traking parameter changed */

    ROI_AND_FILTER_CHANGED
};

/** @defgroup DWI_Volume_Processing DWI Processing module
 * This is the group of methods to process diffusion weighted (DW) magnetic resonance scans.
 *
 * The estimation of a DTI volume from the DWI scans is performed and the interpolation of a white
 * matter fiber trajectory is carried out from the principal directions of the diffusion tensor data.
 *
 */
/**
 * @ingroup DWI_Volume_Processing
 *
 * @brief Estimate the white matter fiber from DWI scans.
 *
 * The procedure comprises two steps: (1) estimation of DTI volume from DWI scans, (2) interpolation of the
 * directions corresponding to the eigenvectors with the largest eigenvalues within each voxel (sample).
 */
class ModelDTI: public ModelSubject<ModelDTI, ModelDTIEvent>
{
public:

    /**
     * @enum ScalarIndex
     * Types of scalar maps.
     */
    enum class ScalarIndex
    {
        NONE,           /**< No action */
        FA,             /**< Fractional Anisotropy */
        COLOR_CODED_FA, /**< Fractional Anisotropy and the eigenvector with the largest eigenvalue */
        MD              /**< Mean diffusivity or trace */
    };

    /**
     * @brief Parameters required to fiber tracking from a seed.
     */
    typedef struct _fiber_tracking_data
    {
        short m_id; /**< Identifier */
        bool m_isVisible; /**< Visibility state (1=visible;0=non-visible) */
        std::string m_fiberBundleName; /**< Label */
        unsigned int m_fiberLengthMin; /**< Minimum length of a valid fiber  */
        unsigned int m_fiberLengthMax; /**< Maximum length of a valid fiber */
        unsigned short m_stoppingCriterionAngle; /**< Maximum bending angle of a valid fiber */
        float m_stoppingCriterionFA; /**< Minimum fractional anisotropy of a valid fiber */
        float m_seedCreationFA; /**< Minimum fractional anisotorpy of a valid seedig voxel */
        float m_wpunct; /**< Weight of the previous tracking direction in the next direction */
        FiberClusterSet::sPtr m_fiberClusterSet; /**< Array of tracked fibers from the seeds of a region of interest (ROI) */
        std::vector<int> m_roiSeedVvoxelIndices;
        FiberTrackingSeedCreator::SeedBuffer::sPtr m_seedBuffer;
        FiberTrackingFilterCreator::FilterBuffer::sPtr m_andFilterBuffer;
        FiberTrackingFilterCreator::FilterBuffer::sPtr m_notFilterBuffer;
        bool m_isUpdated;

        _fiber_tracking_data()
        {
            m_id = -1;
            m_isVisible = false;
            m_fiberBundleName = "";
            m_fiberLengthMin = 0;
            m_fiberLengthMax = 0;
            m_stoppingCriterionAngle = 0;
            m_stoppingCriterionFA = 0.f;
            m_seedCreationFA = 0.f;
            m_fiberClusterSet = nullptr;
            m_wpunct = 0;
            m_isUpdated = true;
            m_seedBuffer = std::make_shared<FiberTrackingSeedCreator::SeedBuffer>();
            m_andFilterBuffer = std::make_shared<FiberTrackingFilterCreator::FilterBuffer>();
            m_notFilterBuffer = std::make_shared<FiberTrackingFilterCreator::FilterBuffer>();
        }

    } FiberBundleData;

    /**
     * @brief Constructor of an object of the class ModelDTI.
     *
     */
    ModelDTI();

    /**
     * @brief Set the DWI volume.
     *
     * @param[in] getAcquisition identifier of the DWI scan.
     */
    void setAcquisition(vmtkDiffAcquisition *getAcquisition);
    /**
     * @brief Get the current DWI volume.
     *
     * @return vmtkDiffAcquisition identifier of the current DWI volume.
     */
    vmtkDiffAcquisition *getAcquisition() const;

    /**
     * @brief Get the computed DTI volume.
     *
     * @return DTI identifier of the current DTI volume.
     */
    const DTI *getDTI();
    /**
     * @brief Set the current DTI volume.
     *
     * @param dti identifier of the DTI volume.
     * @return ModelDTIEvent NONE (failed)/TENSORS_COMPUTED (succeeded).
     */
    void setDTI(ModelDTI *model_dti);

    /**
     * @brief Track the fibers from the seeds placed in every voxel.
     *
     * @param filter_and_indices array of the AND regions.
     * @param and_number_filter number of the AND regions.
     * @param filter_not_indices array of the NOT regions.
     * @param not_number_filter number of the NOT regions.
     * @return ModelDTIEvent CURRENT_FIBER_BUNDLE_CHANGED (succeeded) // Ting: what happens when it fails? (23/04/2020)
     */
    void computeGlobalTractography();

    /**
     * @brief
     *
     * @param voxel_indices
     * @param filter_and_indices
     * @param and_number_filter
     * @param filter_not_indices
     * @param not_number_filter
     * @return ModelDTIEvent
     */
    void computeTractographyFromROIs();

    /**
     * @brief
     *
     * @return bool
     */
    bool isDTIComputed() const;

    /**
     * @brief
     *
     * @param currentDiffusionVolume
     * @return ModelDTIEvent
     */
    void setScalarIndexRenderingActivated(bool state);
    /**
     * @brief
     *
     * @return DiffusionVolume
     */
    bool isScalarIndexRenderingActivated() const;

    /**
     * @brief
     *
     * @return ScalarIndex
     */
    ScalarIndex getCurrentScalarIndexMode() const;
    /**
     * @brief
     *
     * @param currentScalarIndex
     * @return ModelDTIEvent
     */
    void setCurrentScalarIndexMode(const ScalarIndex &currentScalarIndex);

    /**
     * @brief
     *
     * @param current_fiber_bundle_edition_index
     * @return ModelDTIEvent
     */
    void setCurrentFiberBundleEditionIndex(short current_fiber_bundle_edition_index);
    /**
     * @brief
     *
     * @return short
     */
    short getCurrentFiberBundleEditionIndex() const;

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @param fiber_length_min
     * @param fiber_length_max
     * @param stopping_criterion_angle
     * @param stopping_criterion_fa
     * @param seed_creation_fa
     * @param wpunct
     */
    void getFiberBundleTrackingParameters(short fiber_bundle_index,
                            unsigned int &fiber_length_min,
                            unsigned int &fiber_length_max,
                            unsigned short &stopping_criterion_angle,
                            float &stopping_criterion_fa,
                            float &seed_creation_fa, float &wpunct);

    /**
     * @brief
     *
     * @param fiber_length_min
     * @param fiber_length_max
     * @param stopping_criterion_angle
     * @param stopping_criterion_fa
     * @param seed_creation_fa
     * @param wpunct
     */
    void getCurrentTrackingParameters(unsigned int &fiber_length_min,
                               unsigned int &fiber_length_max,
                               unsigned short &stopping_criterion_angle,
                               float &stopping_criterion_fa,
                               float &seed_creation_fa, float &wpunct);

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return std::string
     */
    std::string getFiberBundleName(short fiber_bundle_index);
    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @param name
     * @return ModelDTIEvent
     */
    void setFiberBundleName(short fiber_bundle_index, std::string name);

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return bool
     */
    bool isFiberBundleVisible(short fiber_bundle_index);
    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @param visible
     * @return ModelDTIEvent
     */
    void setFiberBundleVisible(short fiber_bundle_index, bool visible);
    /**
     * @brief
     *
     * @return std::vector<std::pair<unsigned short, bool> >
     */
    std::vector<std::pair<unsigned short, bool> > getFiberBundlesVisibility();

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return bool
     */
    bool isFiberBundleReconstructed(short fiber_bundle_index);

    /**
     * @brief
     *
     * @param fiber_bundle_data
     * @return ModelDTIEvent
     */
    void addFiberBundleData(FiberBundleData fiber_bundle_data);
    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return FiberBundleData
     */
    FiberBundleData getFiberBundleData(unsigned short fiber_bundle_index);

    /**
     * @brief
     *
     * @param fiber_bundle_id
     * @return FiberBundleData
     */
    FiberBundleData getFiberBundleDataByID(unsigned short fiber_bundle_id);

    /**
     * @brief
     *
     * @param fiber_bundle_data
     * @return ModelDTIEvent
     */
    void setCurrentFiberBundleData(FiberBundleData fiber_bundle_data);

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return FiberClusterSet::sPtr
     */
    FiberClusterSet::sPtr getFiberBundleSet(short fiber_bundle_index);
    /**
     * @brief
     *
     * @return std::vector<FiberClusterSet::sPtr>
     */
    std::vector<FiberClusterSet::sPtr> getVisibleFiberBundles();

    /**
     * @brief
     *
     * @return unsigned short
     */
    unsigned short getFiberBundleCount();

    /**
     * @brief
     *
     * @param is_visible
     * @param tract_name
     * @param current_edition
     * @return ModelDTIEvent
     */
    void addFiberBundle(bool is_visible, std::string tract_name, bool current_edition);
    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return ModelDTIEvent
     */
    void deleteFiberBundleByIndex(short fiber_bundle_index);
    /**
     * @brief
     *
     * @param fiber_bundle_id
     * @return ModelDTIEvent
     */
    void deleteFiberBundleByID(short fiber_bundle_id);

    /**
     * @brief
     *
     * @param fiber_bundle_index
     * @return short
     */
    short getFiberBundleID(short fiber_bundle_index);

    /**
     * @brief
     *
     * @param fiberLengthMin
     * @return ModelDTIEvent
     */
    void setFiberLengthMin(unsigned int fiberLengthMin);
    /**
     * @brief
     *
     * @param fiberLengthMax
     * @return ModelDTIEvent
     */
    void setFiberLengthMax(unsigned int fiberLengthMax);
    /**
     * @brief
     *
     * @param stoppingCriterionAngle
     * @return ModelDTIEvent
     */
    void setStoppingCriterionAngle(unsigned short stoppingCriterionAngle);
    /**
     * @brief
     *
     * @param stoppingCriterionFA
     * @return ModelDTIEvent
     */
    void setStoppingCriterionFA(float stoppingCriterionFA);
    /**
     * @brief
     *
     * @param seedCreationFA
     * @return ModelDTIEvent
     */
    void setSeedCreationFA(float seedCreationFA);
    /**
     * @brief
     *
     * @param wpunct
     * @return ModelDTIEvent
     */
    void setWpunct(float wpunct);

    void setROIPlaneAngle(float angle);

    void setActivatedROIPlaneFilter(float value);

    /**
     * @brief
     *
     * @return short
     */
    short getDeletedFiberBundleID() const;

    void setProgressBar(ProgressDialog *progress_bar);

    void setROISeedAndFilters(const std::vector<int> roi_seed_voxel_indices,
                                       const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter,
                                       const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter);

    void setFilters(const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter,
                             const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter);

    void setAutomaticFiberTrackingEnabled(bool automaticFiberTrackingEnabled);

    void enableROIBasedFiberTracking(bool state);

    bool isAutomaticFiberTrackingEnabled() const;

    bool isFiberBundleUpdated(unsigned short fiber_bundle_index);

    void getROIPlaneFilterParameters(bool &roi_plane_filter_activated, int &roi_angle_filter);

    void loadDTI(std::string path_file);

private:

    void setROISeed(const std::vector<int> roi_seed_voxel_indices);
    void setANDFilter(const std::vector<int> and_filter_indices, const std::vector<int> and_number_filter);
    void setNOTFilter(const std::vector<int> not_filter_indices, const std::vector<int> not_number_filter);

    void computeTractography();

    DTI *m_dti; /**< TODO: describe */
    vmtkDiffAcquisition *m_acquisition; /**< TODO: describe */

    FiberTrackingAlgorithm *m_fiberTrackingAlgorithm; /**< TODO: describe */

    short m_currentFiberBundleEditionIndex; /**< TODO: describe */
    std::vector<FiberBundleData> m_fiberBundleDataVector; /**< TODO: describe */

    bool m_isScalarIndexRenderingActivated;
    ScalarIndex m_currentScalarIndex; /**< TODO: describe */

    bool m_isDTIComputed; /**< TODO: describe */

    short m_fiberBundleIDCount; /**< TODO: describe */

    unsigned int m_fiberLengthMin; /**< TODO: describe */
    unsigned int m_fiberLengthMax; /**< TODO: describe */
    unsigned short m_stoppingCriterionAngle; /**< TODO: describe */
    float m_stoppingCriterionFA; /**< TODO: describe */
    float m_seedCreationFA; /**< TODO: describe */
    float m_wpunct; /**< TODO: describe */

    float m_ROIPlaneAngle;
    bool m_isROIPlaneFilterActivated;

    short m_deletedFiberBundleID; /**< TODO: describe */

    ProgressDialog *m_dtiProgressBar; /**< progress bar widget */

    bool m_isAutomaticFiberTrackingEnabled;
    bool m_isROIBasedFiberTracking;
};

#endif // MODEL_DTI_H
